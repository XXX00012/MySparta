#include "TopGraph.h"

#include <adf.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

TopStencilGraph topStencil("hdiff");

#if defined(__AIESIM__) || defined(__X86SIM__)

namespace {

constexpr int NUM_INPUTS = 5;
constexpr int DEFAULT_ITER = 2;   // 和你现在生成的 2x256 输入一致
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
    const int iter_cnt = (argc >= 2) ? std::atoi(argv[1]) : DEFAULT_ITER;
    const std::string in_prefix  = (argc >= 3) ? argv[2] : "./data/hdiff";
    const std::string out_path   = (argc >= 4) ? argv[3] : "./data/aie_out_gmio.txt";

    if (iter_cnt <= 0) {
        std::fprintf(stderr, "[error] iter_cnt must be > 0\n");
        return -1;
    }

    const int elems_per_input = iter_cnt * COL;
    const int out_elems       = iter_cnt * COL;
    const std::size_t bytes_per_input = elems_per_input * sizeof(int32_t);
    const std::size_t out_bytes       = out_elems * sizeof(int32_t);

    std::array<int32_t*, NUM_INPUTS> inbuf{};
    for (int i = 0; i < NUM_INPUTS; ++i) {
        inbuf[i] = reinterpret_cast<int32_t*>(GMIO::malloc(bytes_per_input));
        if (!inbuf[i]) {
            std::fprintf(stderr, "[error] GMIO::malloc failed for input %d\n", i);
            return -1;
        }
    }

    int32_t* outbuf = reinterpret_cast<int32_t*>(GMIO::malloc(out_bytes));
    if (!outbuf) {
        std::fprintf(stderr, "[error] GMIO::malloc failed for output\n");
        return -1;
    }

    bool ok = true;
    for (int i = 0; i < NUM_INPUTS; ++i) {
        const std::string path = in_prefix + "_in" + std::to_string(i) + "_stream.txt";
        if (!load_stream_file(path, inbuf[i], elems_per_input)) {
            ok = false;
        }
    }
    if (!ok) {
        std::fprintf(stderr, "[warn] fallback to ramp inputs for simulation\n");
        fill_ramp_inputs(inbuf, elems_per_input);
    }

    for (int i = 0; i < out_elems; ++i) outbuf[i] = 0;

    print_preview("[sim] input0 preview:", inbuf[0], PREVIEW);

    topStencil.init();

    topStencil.run(iter_cnt);

    for (int i = 0; i < NUM_INPUTS; ++i) {
        topStencil.in_gmio[i].gm2aie_nb(inbuf[i], bytes_per_input);
    }
    topStencil.out_gmio.aie2gm_nb(outbuf, out_bytes);

    topStencil.out_gmio.wait();
    topStencil.wait();
    topStencil.end();

    print_preview("[sim] output preview:", outbuf, PREVIEW);
    dump_output_matrix(out_path, outbuf, iter_cnt);

    for (int i = 0; i < NUM_INPUTS; ++i) {
        GMIO::free(inbuf[i]);
    }
    GMIO::free(outbuf);

    std::printf("[sim] done.\n");
    return 0;
}
#endif