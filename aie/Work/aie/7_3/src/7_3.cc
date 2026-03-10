// Automatically generated Processor driver using MathEngine Tool chain
// Processor File : /media/4T/home/xianyijiang/my_sparta/aie/./Work/aie/7_3/src/7_3.cc
// Runs 1 ME kernel#undef DEBUG 
#undef DEBUGLOCKS 

#include <adf.h>
// Initialize sync_buffer[1] with compiler option static test-iterations value or -1
volatile static int sync_buffer[8] = {0, -1};
#include <adf/sync/mesync.h>

#include "ProcessUnit/hdiff.h"
#include "include.h"
// Define Locks
#define LOCK_7_3_2_5120 18
#define LOCK_7_3_3_0 19
#define LOCK_7_3_4_0 20
#define LOCK_7_3_5_2048 21
#define LOCK_7_3_6_2048 22
#define LOCK_7_3_7_6144 23
#define LOCK_7_3_8_2048 24
#define LOCK_7_3_9_4096 25
#define LOCK_7_3_10_6144 26
#define LOCK_7_3_11_2048 27
#define LOCK_7_4_0_0 32
#define LOCK_7_4_1_0 33

// Declare shared memory buffers
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
v4int32 buf17[64];
v4int32 buf17d[64];

// Declare Kernel functions and initializers
void hdiff_flux2(input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,output_window<int> *);

// Declare Kernel objects and external arrays

int main(void) {
  sync_buffer[0] = 0; //reset end signal
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
  window_internal window_buf17_buf17d[1];
  window_datatype * buf17_ptr = (window_datatype * )buf17;
  window_datatype * buf17d_ptr = (window_datatype * )buf17d;
  window_init(window_buf17_buf17d, 1, buf17, LOCK_7_4_0_0, buf17d, LOCK_7_4_1_0, 64, 64);
  int lockid_i2_pi0 = LOCK_7_3_2_5120;
  int lockid_i2_pi1 = LOCK_7_3_4_0;
  int lockid_i2_pi2 = LOCK_7_3_6_2048;
  int lockid_i2_pi3 = LOCK_7_3_8_2048;
  int lockid_i2_pi4 = LOCK_7_3_10_6144;
  int lockid_i2_po0 = LOCK_7_4_0_0;
  input_window_int32 *input_window_i2_pi0 = (get_input_window_int32(window_buf12_buf12d));
  input_window_int32 *input_window_i2_pi1 = (get_input_window_int32(window_buf13_buf13d));
  input_window_int32 *input_window_i2_pi2 = (get_input_window_int32(window_buf14_buf14d));
  input_window_int32 *input_window_i2_pi3 = (get_input_window_int32(window_buf15_buf15d));
  input_window_int32 *input_window_i2_pi4 = (get_input_window_int32(window_buf16_buf16d));
  output_window_int32 *output_window_i2_po0 = (get_output_window_int32(window_buf17_buf17d));
  int32 index = 1;
  while(true)
  {

  int32 proc_7_3_bounds = sync_buffer[1];

  while (proc_7_3_bounds)
  {

    // Kernel call : i2:hdiff_flux2
    acquire(lockid_i2_pi0, ACQ_READ);
    acquire(lockid_i2_pi1, ACQ_READ);
    acquire(lockid_i2_pi2, ACQ_READ);
    acquire(lockid_i2_pi3, ACQ_READ);
    acquire(lockid_i2_pi4, ACQ_READ);
    acquire(lockid_i2_po0, ACQ_WRITE);
    hdiff_flux2((input_window_i2_pi0),(input_window_i2_pi1),(input_window_i2_pi2),(input_window_i2_pi3),(input_window_i2_pi4),(output_window_i2_po0));
    // The output port does not connect to an ME compute node, so write pointer need not be advanced
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i2_pi0,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i2_pi1,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i2_pi2,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i2_pi3,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i2_pi4,REL_WRITE);
    release(lockid_i2_po0,REL_READ);
    lockid_i2_pi0 = index ? LOCK_7_3_3_0: LOCK_7_3_2_5120;
    adjustPingPongBuffer(window_buf12_buf12d,buf12_ptr,buf12d_ptr,0,index);
    input_window_i2_pi0->ptr = window_buf12_buf12d->head;
    lockid_i2_pi1 = index ? LOCK_7_3_5_2048: LOCK_7_3_4_0;
    adjustPingPongBuffer(window_buf13_buf13d,buf13_ptr,buf13d_ptr,0,index);
    input_window_i2_pi1->ptr = window_buf13_buf13d->head;
    lockid_i2_pi2 = index ? LOCK_7_3_7_6144: LOCK_7_3_6_2048;
    adjustPingPongBuffer(window_buf14_buf14d,buf14_ptr,buf14d_ptr,0,index);
    input_window_i2_pi2->ptr = window_buf14_buf14d->head;
    lockid_i2_pi3 = index ? LOCK_7_3_9_4096: LOCK_7_3_8_2048;
    adjustPingPongBuffer(window_buf15_buf15d,buf15_ptr,buf15d_ptr,0,index);
    input_window_i2_pi3->ptr = window_buf15_buf15d->head;
    lockid_i2_pi4 = index ? LOCK_7_3_11_2048: LOCK_7_3_10_6144;
    adjustPingPongBuffer(window_buf16_buf16d,buf16_ptr,buf16d_ptr,0,index);
    input_window_i2_pi4->ptr = window_buf16_buf16d->head;
    lockid_i2_po0 = index ? LOCK_7_4_1_0: LOCK_7_4_0_0;
    adjustPingPongBuffer(window_buf17_buf17d,buf17_ptr,buf17d_ptr,0,index);
    output_window_i2_po0->ptr = window_buf17_buf17d->head;
    index = 1 - index;

    chess_memory_fence();

    if(proc_7_3_bounds > 0)
      --proc_7_3_bounds;
  }
  done();
  if (sync_buffer[0] > 0) break;
  }
  return 0;
}