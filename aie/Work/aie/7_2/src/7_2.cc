// Automatically generated Processor driver using MathEngine Tool chain
// Processor File : /media/4T/home/xianyijiang/my_sparta/aie/./Work/aie/7_2/src/7_2.cc
// Runs 1 ME kernel#undef DEBUG 
#undef DEBUGLOCKS 

#include <adf.h>
// Initialize sync_buffer[1] with compiler option static test-iterations value or -1
volatile static int sync_buffer[8] = {0, -1};
#include <adf/sync/mesync.h>

#include "ProcessUnit/hdiff.h"
#include "include.h"
// Define Locks
#define LOCK_6_2_0_0 16
#define LOCK_6_2_1_0 17
#define LOCK_7_3_0_4096 32
#define LOCK_7_3_1_7168 33
#define LOCK_6_2_2_0 18
#define LOCK_6_2_3_0 19
#define LOCK_7_2_4_6144 52
#define LOCK_7_2_5_6688 53
#define LOCK_7_1_0_0 0
#define LOCK_7_1_1_0 1
#define LOCK_7_2_6_0 54
#define LOCK_7_2_7_2048 55
#define LOCK_7_2_8_7168 56
#define LOCK_7_1_2_0 2
#define LOCK_7_3_2_5120 34
#define LOCK_7_3_3_0 35
#define LOCK_7_3_4_0 36
#define LOCK_7_3_5_2048 37
#define LOCK_7_3_6_2048 38
#define LOCK_7_3_7_6144 39
#define LOCK_7_3_8_2048 40
#define LOCK_7_3_9_4096 41
#define LOCK_7_3_10_6144 42
#define LOCK_7_3_11_2048 43

// Declare shared memory buffers
v4int32 buf10[64];
v4int32 buf10d[64];
v4int32 buf11[64];
v4int32 buf11d[64];
v4int32 buf12[128];
v4int32 buf12d[128];
v4int32 buf13[128];
v4int32 buf13d[128];
v4int32 buf14[128];
v4int32 buf14d[128];
v4int32 buf15[128];
v4int32 buf15d[128];
v4int32 buf16[128];
v4int32 buf16d[128];
v4int32 buf2[64];
v4int32 buf2d[64];
v4int32 buf4[64];
v4int32 buf4d[64];
v4int32 buf6[64];
v4int32 buf6d[64];
v4int32 buf8[64];
v4int32 buf8d[64];
v4int32 buf9[64];
v4int32 buf9d[64];

// Declare Kernel functions and initializers
void hdiff_flux1(input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,output_window<int> *,output_window<int> *,output_window<int> *,output_window<int> *,output_window<int> *);

// Declare Kernel objects and external arrays

int main(void) {
  sync_buffer[0] = 0; //reset end signal
  window_internal window_buf2_buf2d[1];
  window_datatype * buf2_ptr = (window_datatype * )buf2;
  window_datatype * buf2d_ptr = (window_datatype * )buf2d;
  window_init(window_buf2_buf2d, 1, buf2, LOCK_6_2_0_0, buf2d, LOCK_6_2_1_0, 64, 64);
  window_internal window_buf4_buf4d[1];
  window_datatype * buf4_ptr = (window_datatype * )buf4;
  window_datatype * buf4d_ptr = (window_datatype * )buf4d;
  window_init(window_buf4_buf4d, 1, buf4, LOCK_7_3_0_4096, buf4d, LOCK_7_3_1_7168, 64, 64);
  window_internal window_buf6_buf6d[1];
  window_datatype * buf6_ptr = (window_datatype * )buf6;
  window_datatype * buf6d_ptr = (window_datatype * )buf6d;
  window_init(window_buf6_buf6d, 1, buf6, LOCK_6_2_2_0, buf6d, LOCK_6_2_3_0, 64, 64);
  window_internal window_buf8_buf8d[1];
  window_datatype * buf8_ptr = (window_datatype * )buf8;
  window_datatype * buf8d_ptr = (window_datatype * )buf8d;
  window_init(window_buf8_buf8d, 1, buf8, LOCK_7_2_4_6144, buf8d, LOCK_7_2_5_6688, 64, 64);
  window_internal window_buf9_buf9d[1];
  window_datatype * buf9_ptr = (window_datatype * )buf9;
  window_datatype * buf9d_ptr = (window_datatype * )buf9d;
  window_init(window_buf9_buf9d, 1, buf9, LOCK_7_1_0_0, buf9d, LOCK_7_1_1_0, 64, 64);
  window_internal window_buf10_buf10d[1];
  window_datatype * buf10_ptr = (window_datatype * )buf10;
  window_datatype * buf10d_ptr = (window_datatype * )buf10d;
  window_init(window_buf10_buf10d, 1, buf10, LOCK_7_2_6_0, buf10d, LOCK_7_2_7_2048, 64, 64);
  window_internal window_buf11_buf11d[1];
  window_datatype * buf11_ptr = (window_datatype * )buf11;
  window_datatype * buf11d_ptr = (window_datatype * )buf11d;
  window_init(window_buf11_buf11d, 1, buf11, LOCK_7_2_8_7168, buf11d, LOCK_7_1_2_0, 64, 64);
  window_internal window_buf12_buf12d[1];
  window_datatype * buf12_ptr = (window_datatype * )buf12;
  window_datatype * buf12d_ptr = (window_datatype * )buf12d;
  window_init(window_buf12_buf12d, 1, buf12, LOCK_7_3_2_5120, buf12d, LOCK_7_3_3_0, 128, 128);
  window_internal window_buf13_buf13d[1];
  window_datatype * buf13_ptr = (window_datatype * )buf13;
  window_datatype * buf13d_ptr = (window_datatype * )buf13d;
  window_init(window_buf13_buf13d, 1, buf13, LOCK_7_3_4_0, buf13d, LOCK_7_3_5_2048, 128, 128);
  window_internal window_buf14_buf14d[1];
  window_datatype * buf14_ptr = (window_datatype * )buf14;
  window_datatype * buf14d_ptr = (window_datatype * )buf14d;
  window_init(window_buf14_buf14d, 1, buf14, LOCK_7_3_6_2048, buf14d, LOCK_7_3_7_6144, 128, 128);
  window_internal window_buf15_buf15d[1];
  window_datatype * buf15_ptr = (window_datatype * )buf15;
  window_datatype * buf15d_ptr = (window_datatype * )buf15d;
  window_init(window_buf15_buf15d, 1, buf15, LOCK_7_3_8_2048, buf15d, LOCK_7_3_9_4096, 128, 128);
  window_internal window_buf16_buf16d[1];
  window_datatype * buf16_ptr = (window_datatype * )buf16;
  window_datatype * buf16d_ptr = (window_datatype * )buf16d;
  window_init(window_buf16_buf16d, 1, buf16, LOCK_7_3_10_6144, buf16d, LOCK_7_3_11_2048, 128, 128);
  int lockid_i1_pi0 = LOCK_6_2_0_0;
  int lockid_i1_pi1 = LOCK_7_3_0_4096;
  int lockid_i1_pi2 = LOCK_6_2_2_0;
  int lockid_i1_pi3 = LOCK_7_2_4_6144;
  int lockid_i1_pi4 = LOCK_7_1_0_0;
  int lockid_i1_pi5 = LOCK_7_2_6_0;
  int lockid_i1_pi6 = LOCK_7_2_8_7168;
  int lockid_i1_po0 = LOCK_7_3_2_5120;
  int lockid_i1_po1 = LOCK_7_3_4_0;
  int lockid_i1_po2 = LOCK_7_3_6_2048;
  int lockid_i1_po3 = LOCK_7_3_8_2048;
  int lockid_i1_po4 = LOCK_7_3_10_6144;
  input_window_int32 *input_window_i1_pi0 = (get_input_window_int32(window_buf2_buf2d));
  input_window_int32 *input_window_i1_pi1 = (get_input_window_int32(window_buf4_buf4d));
  input_window_int32 *input_window_i1_pi2 = (get_input_window_int32(window_buf6_buf6d));
  input_window_int32 *input_window_i1_pi3 = (get_input_window_int32(window_buf8_buf8d));
  input_window_int32 *input_window_i1_pi4 = (get_input_window_int32(window_buf9_buf9d));
  input_window_int32 *input_window_i1_pi5 = (get_input_window_int32(window_buf10_buf10d));
  input_window_int32 *input_window_i1_pi6 = (get_input_window_int32(window_buf11_buf11d));
  output_window_int32 *output_window_i1_po0 = (get_output_window_int32(window_buf12_buf12d));
  output_window_int32 *output_window_i1_po1 = (get_output_window_int32(window_buf13_buf13d));
  output_window_int32 *output_window_i1_po2 = (get_output_window_int32(window_buf14_buf14d));
  output_window_int32 *output_window_i1_po3 = (get_output_window_int32(window_buf15_buf15d));
  output_window_int32 *output_window_i1_po4 = (get_output_window_int32(window_buf16_buf16d));
  int32 index = 1;
  while(true)
  {

  int32 proc_7_2_bounds = sync_buffer[1];

  while (proc_7_2_bounds)
  {

    // Kernel call : i1:hdiff_flux1
    acquire(lockid_i1_pi0, ACQ_READ);
    acquire(lockid_i1_pi1, ACQ_READ);
    acquire(lockid_i1_pi2, ACQ_READ);
    acquire(lockid_i1_pi3, ACQ_READ);
    acquire(lockid_i1_pi4, ACQ_READ);
    acquire(lockid_i1_pi5, ACQ_READ);
    acquire(lockid_i1_pi6, ACQ_READ);
    acquire(lockid_i1_po0, ACQ_WRITE);
    acquire(lockid_i1_po1, ACQ_WRITE);
    acquire(lockid_i1_po2, ACQ_WRITE);
    acquire(lockid_i1_po3, ACQ_WRITE);
    acquire(lockid_i1_po4, ACQ_WRITE);
    hdiff_flux1((input_window_i1_pi0),(input_window_i1_pi1),(input_window_i1_pi2),(input_window_i1_pi3),(input_window_i1_pi4),(input_window_i1_pi5),(input_window_i1_pi6),(output_window_i1_po0),(output_window_i1_po1),(output_window_i1_po2),(output_window_i1_po3),(output_window_i1_po4));
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi0,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi1,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi2,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi3,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi4,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi5,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i1_pi6,REL_WRITE);
    release(lockid_i1_po0,REL_READ);
    release(lockid_i1_po1,REL_READ);
    release(lockid_i1_po2,REL_READ);
    release(lockid_i1_po3,REL_READ);
    release(lockid_i1_po4,REL_READ);
    lockid_i1_pi0 = index ? LOCK_6_2_1_0: LOCK_6_2_0_0;
    adjustPingPongBuffer(window_buf2_buf2d,buf2_ptr,buf2d_ptr,0,index);
    input_window_i1_pi0->ptr = window_buf2_buf2d->head;
    lockid_i1_pi1 = index ? LOCK_7_3_1_7168: LOCK_7_3_0_4096;
    adjustPingPongBuffer(window_buf4_buf4d,buf4_ptr,buf4d_ptr,0,index);
    input_window_i1_pi1->ptr = window_buf4_buf4d->head;
    lockid_i1_pi2 = index ? LOCK_6_2_3_0: LOCK_6_2_2_0;
    adjustPingPongBuffer(window_buf6_buf6d,buf6_ptr,buf6d_ptr,0,index);
    input_window_i1_pi2->ptr = window_buf6_buf6d->head;
    lockid_i1_pi3 = index ? LOCK_7_2_5_6688: LOCK_7_2_4_6144;
    adjustPingPongBuffer(window_buf8_buf8d,buf8_ptr,buf8d_ptr,0,index);
    input_window_i1_pi3->ptr = window_buf8_buf8d->head;
    lockid_i1_pi4 = index ? LOCK_7_1_1_0: LOCK_7_1_0_0;
    adjustPingPongBuffer(window_buf9_buf9d,buf9_ptr,buf9d_ptr,0,index);
    input_window_i1_pi4->ptr = window_buf9_buf9d->head;
    lockid_i1_pi5 = index ? LOCK_7_2_7_2048: LOCK_7_2_6_0;
    adjustPingPongBuffer(window_buf10_buf10d,buf10_ptr,buf10d_ptr,0,index);
    input_window_i1_pi5->ptr = window_buf10_buf10d->head;
    lockid_i1_pi6 = index ? LOCK_7_1_2_0: LOCK_7_2_8_7168;
    adjustPingPongBuffer(window_buf11_buf11d,buf11_ptr,buf11d_ptr,0,index);
    input_window_i1_pi6->ptr = window_buf11_buf11d->head;
    lockid_i1_po0 = index ? LOCK_7_3_3_0: LOCK_7_3_2_5120;
    adjustPingPongBuffer(window_buf12_buf12d,buf12_ptr,buf12d_ptr,0,index);
    output_window_i1_po0->ptr = window_buf12_buf12d->head;
    lockid_i1_po1 = index ? LOCK_7_3_5_2048: LOCK_7_3_4_0;
    adjustPingPongBuffer(window_buf13_buf13d,buf13_ptr,buf13d_ptr,0,index);
    output_window_i1_po1->ptr = window_buf13_buf13d->head;
    lockid_i1_po2 = index ? LOCK_7_3_7_6144: LOCK_7_3_6_2048;
    adjustPingPongBuffer(window_buf14_buf14d,buf14_ptr,buf14d_ptr,0,index);
    output_window_i1_po2->ptr = window_buf14_buf14d->head;
    lockid_i1_po3 = index ? LOCK_7_3_9_4096: LOCK_7_3_8_2048;
    adjustPingPongBuffer(window_buf15_buf15d,buf15_ptr,buf15d_ptr,0,index);
    output_window_i1_po3->ptr = window_buf15_buf15d->head;
    lockid_i1_po4 = index ? LOCK_7_3_11_2048: LOCK_7_3_10_6144;
    adjustPingPongBuffer(window_buf16_buf16d,buf16_ptr,buf16d_ptr,0,index);
    output_window_i1_po4->ptr = window_buf16_buf16d->head;
    index = 1 - index;

    chess_memory_fence();

    if(proc_7_2_bounds > 0)
      --proc_7_2_bounds;
  }
  done();
  if (sync_buffer[0] > 0) break;
  }
  return 0;
}