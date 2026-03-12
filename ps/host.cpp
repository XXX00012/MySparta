#include "TopGraph.h"
#include "./ProcessUnit/include.h"

#include <adf/adf_api/XRTConfig.h>
#include <experimental/xrt_device.h>
#include <experimental/xrt_kernel.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace adf;

extern TopStencilGraph topStencil;

namespace {

constexpr int NUM_INPUTS = 5;
constexpr int DEFAULT_ITER = 2;   // 与现有 data/hdiff_in*_stream.txt 的 2x256 对齐
constexpr int PREVIEW = 16;

bool load_stream_file(const std::string& path, int32_t* buf, int elems) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::fprintf(stderr, "[warn] cannot open %s\n", path.c_str());
        return false;
    }

    long long v = 0;
    int cnt = 0;
    while (fin >> v) {
        if (cnt >= elems) break;
        buf[cnt++] = static_cast<int32_t>(v);
    }

    if (cnt != elems) {
        std::fprintf(stderr,
                     "[warn] %s element count mismatch: got %d, expect %d\n",
                     path.c_str(), cnt, elems);
        return false;
    }
    return true;
}

void fill_ramp_inputs(std::array<int32_t*, NUM_INPUTS>& inbuf, int elems_per_input) {
    for (int k = 0; k < NUM_INPUTS; ++k) {
        for (int i = 0; i < elems_per_input; ++i) {
            inbuf[k][i] = static_cast<int32_t>(k * 10000 + i);
        }
    }
}

void zero_output(int32_t* out, int elems) {
    for (int i = 0; i < elems; ++i) out[i] = 0;
}

void dump_output_matrix(const std::string& path, const int32_t* out, int iter_cnt) {
    std::ofstream fout(path);
    if (!fout.is_open()) {
        std::fprintf(stderr, "[warn] cannot open %s for write\n", path.c_str());
        return;
    }

    for (int it = 0; it < iter_cnt; ++it) {
        for (int c = 0; c < COL; ++c) {
            if (c) fout << ' ';
            fout << out[it * COL + c];
        }
        fout << '\n';
    }
}

void print_preview(const char* tag, const int32_t* p, int n) {
    std::printf("%s", tag);
    for (int i = 0; i < n; ++i) {
        std::printf(" %d", p[i]);
    }
    std::printf("\n");
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr,
                     "Usage: %s <xclbin> [iter_cnt] [input_prefix] [output_txt]\n",
                     argv[0]);
        std::fprintf(stderr,
                     "Example: %s ./hw_emu.xclbin 2 ./data/hdiff ./data/aie_out_gmio.txt\n",
                     argv[0]);
        return EXIT_FAILURE;
    }

    const std::string xclbin_path = argv[1];
    const int iter_cnt            = (argc >= 3) ? std::atoi(argv[2]) : DEFAULT_ITER;
    const std::string in_prefix   = (argc >= 4) ? argv[3] : "./data/hdiff";
    const std::string out_path    = (argc >= 5) ? argv[4] : "./data/aie_out_gmio.txt";

    if (iter_cnt <= 0) {
        std::fprintf(stderr, "[error] iter_cnt must be > 0\n");
        return EXIT_FAILURE;
    }

    const int elems_per_input = iter_cnt * COL;
    const int out_elems       = iter_cnt * COL;
    const std::size_t bytes_per_input = elems_per_input * sizeof(int32_t);
    const std::size_t out_bytes       = out_elems * sizeof(int32_t);

    std::printf("test start.\n");
    std::printf("xclbin      : %s\n", xclbin_path.c_str());
    std::printf("iter_cnt    : %d\n", iter_cnt);
    std::printf("input_prefix: %s\n", in_prefix.c_str());
    std::printf("output_txt  : %s\n", out_path.c_str());

    // ------------------------------------------------------------------
    // 1) open device + load xclbin
    // ------------------------------------------------------------------
    auto dhdl = xrtDeviceOpen(0);
    if (!dhdl) {
        std::fprintf(stderr, "[error] xrtDeviceOpen failed\n");
        return EXIT_FAILURE;
    }

    int ret = xrtDeviceLoadXclbinFile(dhdl, xclbin_path.c_str());
    if (ret) {
        std::fprintf(stderr, "[error] xrtDeviceLoadXclbinFile failed\n");
        xrtDeviceClose(dhdl);
        return EXIT_FAILURE;
    }

    xuid_t uuid;
    xrtDeviceGetXclbinUUID(dhdl, uuid);

    // ADF host must register XRT before graph API / GMIO API
    adf::registerXRT(dhdl, uuid);

    // ------------------------------------------------------------------
    // 2) allocate GMIO buffers in DDR-visible space
    // ------------------------------------------------------------------
    std::array<int32_t*, NUM_INPUTS> inbuf{};
    for (int i = 0; i < NUM_INPUTS; ++i) {
        inbuf[i] = reinterpret_cast<int32_t*>(GMIO::malloc(bytes_per_input));
        if (!inbuf[i]) {
            std::fprintf(stderr, "[error] GMIO::malloc failed for input %d\n", i);
            xrtDeviceClose(dhdl);
            return EXIT_FAILURE;
        }
    }

    int32_t* outbuf = reinterpret_cast<int32_t*>(GMIO::malloc(out_bytes));
    if (!outbuf) {
        std::fprintf(stderr, "[error] GMIO::malloc failed for output\n");
        for (int i = 0; i < NUM_INPUTS; ++i) GMIO::free(inbuf[i]);
        xrtDeviceClose(dhdl);
        return EXIT_FAILURE;
    }

    // ------------------------------------------------------------------
    // 3) initialize input buffers
    //    默认读取:
    //      ./data/hdiff_in0_stream.txt
    //      ...
    //      ./data/hdiff_in4_stream.txt
    // ------------------------------------------------------------------
    bool ok = true;
    for (int i = 0; i < NUM_INPUTS; ++i) {
        const std::string path = in_prefix + "_in" + std::to_string(i) + "_stream.txt";
        if (!load_stream_file(path, inbuf[i], elems_per_input)) {
            ok = false;
        }
    }

    if (!ok) {
        std::fprintf(stderr, "[warn] input files incomplete, fallback to ramp input\n");
        fill_ramp_inputs(inbuf, elems_per_input);
    }

    zero_output(outbuf, out_elems);

    print_preview("input0 preview:", inbuf[0], PREVIEW);

    // ------------------------------------------------------------------
    // 4) run graph + GMIO transfer
    //    对照原论文 test.cpp 的思路：
    //      before start
    //      start / timing
    //      wait
    //      print output
    // ------------------------------------------------------------------
    std::printf("before graph run.\n");

    auto t0 = std::chrono::high_resolution_clock::now();

    topStencil.run(iter_cnt);

    // issue input transfers
    for (int i = 0; i < NUM_INPUTS; ++i) {
        topStencil.in_gmio[i].gm2aie_nb(inbuf[i], bytes_per_input);
    }

    // issue output transfer
    topStencil.out_gmio.aie2gm_nb(outbuf, out_bytes);

    // wait for output transfer + graph finish
    topStencil.out_gmio.wait();
    topStencil.wait();

    auto t1 = std::chrono::high_resolution_clock::now();
    topStencil.end();

    const auto dur_us =
        std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    std::printf("after graph run.\n");
    std::printf("End-to-end time: %lld us\n", static_cast<long long>(dur_us));

    print_preview("output preview:", outbuf, PREVIEW);
    dump_output_matrix(out_path, outbuf, iter_cnt);

    std::printf("output written to %s\n", out_path.c_str());

    // ------------------------------------------------------------------
    // 5) cleanup
    // ------------------------------------------------------------------
    for (int i = 0; i < NUM_INPUTS; ++i) {
        GMIO::free(inbuf[i]);
    }
    GMIO::free(outbuf);
    xrtDeviceClose(dhdl);

    std::printf("test done.\n");
    return EXIT_SUCCESS;
}