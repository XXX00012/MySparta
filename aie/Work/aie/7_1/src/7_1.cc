// Automatically generated Processor driver using MathEngine Tool chain
// Processor File : /media/4T/home/xianyijiang/my_sparta/aie/./Work/aie/7_1/src/7_1.cc
// Runs 1 ME kernel#undef DEBUG 
#undef DEBUGLOCKS 

#include <adf.h>
// Initialize sync_buffer[1] with compiler option static test-iterations value or -1
volatile static int sync_buffer[8] = {0, -1};
#include <adf/sync/mesync.h>

#include "ProcessUnit/hdiff.h"
#include "include.h"
// Define Locks
#define LOCK_7_0_0_0 0
#define LOCK_7_0_1_0 1
#define LOCK_7_0_2_0 2
#define LOCK_7_0_3_0 3
#define LOCK_8_1_0_0 48
#define LOCK_8_1_1_1024 49
#define LOCK_7_2_0_0 32
#define LOCK_7_2_1_0 33
#define LOCK_7_2_2_1024 34
#define LOCK_7_2_3_7168 35
#define LOCK_7_2_4_6144 36
#define LOCK_7_2_5_6688 37
#define LOCK_7_1_0_0 16
#define LOCK_7_1_1_0 17
#define LOCK_7_2_6_0 38
#define LOCK_7_2_7_2048 39
#define LOCK_7_2_8_7168 40
#define LOCK_7_1_2_0 18

// Declare shared memory buffers
v4int32 buf0[64];
v4int32 buf0d[64];
v4int32 buf1[64];
v4int32 buf10[64];
v4int32 buf10d[64];
v4int32 buf11[64];
v4int32 buf11d[64];
v4int32 buf1d[64];
v4int32 buf3[64];
v4int32 buf3d[64];
v4int32 buf5[64];
v4int32 buf5d[64];
v4int32 buf7[64];
v4int32 buf7d[64];
v4int32 buf8[64];
v4int32 buf8d[64];
v4int32 buf9[64];
v4int32 buf9d[64];

// Declare Kernel functions and initializers
void hdiff_lap(input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,input_window<int> *,output_window<int> *,output_window<int> *,output_window<int> *,output_window<int> *);

// Declare Kernel objects and external arrays

int main(void) {
  sync_buffer[0] = 0; //reset end signal
  window_internal window_buf0_buf0d[1];
  window_datatype * buf0_ptr = (window_datatype * )buf0;
  window_datatype * buf0d_ptr = (window_datatype * )buf0d;
  window_init(window_buf0_buf0d, 1, buf0, LOCK_7_0_0_0, buf0d, LOCK_7_0_1_0, 64, 64);
  window_internal window_buf1_buf1d[1];
  window_datatype * buf1_ptr = (window_datatype * )buf1;
  window_datatype * buf1d_ptr = (window_datatype * )buf1d;
  window_init(window_buf1_buf1d, 1, buf1, LOCK_7_0_2_0, buf1d, LOCK_7_0_3_0, 64, 64);
  window_internal window_buf3_buf3d[1];
  window_datatype * buf3_ptr = (window_datatype * )buf3;
  window_datatype * buf3d_ptr = (window_datatype * )buf3d;
  window_init(window_buf3_buf3d, 1, buf3, LOCK_8_1_0_0, buf3d, LOCK_8_1_1_1024, 64, 64);
  window_internal window_buf5_buf5d[1];
  window_datatype * buf5_ptr = (window_datatype * )buf5;
  window_datatype * buf5d_ptr = (window_datatype * )buf5d;
  window_init(window_buf5_buf5d, 1, buf5, LOCK_7_2_0_0, buf5d, LOCK_7_2_1_0, 64, 64);
  window_internal window_buf7_buf7d[1];
  window_datatype * buf7_ptr = (window_datatype * )buf7;
  window_datatype * buf7d_ptr = (window_datatype * )buf7d;
  window_init(window_buf7_buf7d, 1, buf7, LOCK_7_2_2_1024, buf7d, LOCK_7_2_3_7168, 64, 64);
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
  int lockid_i0_pi0 = LOCK_7_0_0_0;
  int lockid_i0_pi1 = LOCK_7_0_2_0;
  int lockid_i0_pi2 = LOCK_8_1_0_0;
  int lockid_i0_pi3 = LOCK_7_2_0_0;
  int lockid_i0_pi4 = LOCK_7_2_2_1024;
  int lockid_i0_po0 = LOCK_7_2_4_6144;
  int lockid_i0_po1 = LOCK_7_1_0_0;
  int lockid_i0_po2 = LOCK_7_2_6_0;
  int lockid_i0_po3 = LOCK_7_2_8_7168;
  input_window_int32 *input_window_i0_pi0 = (get_input_window_int32(window_buf0_buf0d));
  input_window_int32 *input_window_i0_pi1 = (get_input_window_int32(window_buf1_buf1d));
  input_window_int32 *input_window_i0_pi2 = (get_input_window_int32(window_buf3_buf3d));
  input_window_int32 *input_window_i0_pi3 = (get_input_window_int32(window_buf5_buf5d));
  input_window_int32 *input_window_i0_pi4 = (get_input_window_int32(window_buf7_buf7d));
  output_window_int32 *output_window_i0_po0 = (get_output_window_int32(window_buf8_buf8d));
  output_window_int32 *output_window_i0_po1 = (get_output_window_int32(window_buf9_buf9d));
  output_window_int32 *output_window_i0_po2 = (get_output_window_int32(window_buf10_buf10d));
  output_window_int32 *output_window_i0_po3 = (get_output_window_int32(window_buf11_buf11d));
  int32 index = 1;
  while(true)
  {

  int32 proc_7_1_bounds = sync_buffer[1];

  while (proc_7_1_bounds)
  {

    // Kernel call : i0:hdiff_lap
    acquire(lockid_i0_pi0, ACQ_READ);
    acquire(lockid_i0_pi1, ACQ_READ);
    acquire(lockid_i0_pi2, ACQ_READ);
    acquire(lockid_i0_pi3, ACQ_READ);
    acquire(lockid_i0_pi4, ACQ_READ);
    acquire(lockid_i0_po0, ACQ_WRITE);
    acquire(lockid_i0_po1, ACQ_WRITE);
    acquire(lockid_i0_po2, ACQ_WRITE);
    acquire(lockid_i0_po3, ACQ_WRITE);
    hdiff_lap((input_window_i0_pi0),(input_window_i0_pi1),(input_window_i0_pi2),(input_window_i0_pi3),(input_window_i0_pi4),(output_window_i0_po0),(output_window_i0_po1),(output_window_i0_po2),(output_window_i0_po3));
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The output port margin is zero, therefore write pointer is in correct position for reading
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i0_pi0,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i0_pi1,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i0_pi2,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i0_pi3,REL_WRITE);
    // The input port margin is zero, therefore there is no copy to be done
    // And now the lock has ended
    release(lockid_i0_pi4,REL_WRITE);
    release(lockid_i0_po0,REL_READ);
    release(lockid_i0_po1,REL_READ);
    release(lockid_i0_po2,REL_READ);
    release(lockid_i0_po3,REL_READ);
    lockid_i0_pi0 = index ? LOCK_7_0_1_0: LOCK_7_0_0_0;
    adjustPingPongBuffer(window_buf0_buf0d,buf0_ptr,buf0d_ptr,0,index);
    input_window_i0_pi0->ptr = window_buf0_buf0d->head;
    lockid_i0_pi1 = index ? LOCK_7_0_3_0: LOCK_7_0_2_0;
    adjustPingPongBuffer(window_buf1_buf1d,buf1_ptr,buf1d_ptr,0,index);
    input_window_i0_pi1->ptr = window_buf1_buf1d->head;
    lockid_i0_pi2 = index ? LOCK_8_1_1_1024: LOCK_8_1_0_0;
    adjustPingPongBuffer(window_buf3_buf3d,buf3_ptr,buf3d_ptr,0,index);
    input_window_i0_pi2->ptr = window_buf3_buf3d->head;
    lockid_i0_pi3 = index ? LOCK_7_2_1_0: LOCK_7_2_0_0;
    adjustPingPongBuffer(window_buf5_buf5d,buf5_ptr,buf5d_ptr,0,index);
    input_window_i0_pi3->ptr = window_buf5_buf5d->head;
    lockid_i0_pi4 = index ? LOCK_7_2_3_7168: LOCK_7_2_2_1024;
    adjustPingPongBuffer(window_buf7_buf7d,buf7_ptr,buf7d_ptr,0,index);
    input_window_i0_pi4->ptr = window_buf7_buf7d->head;
    lockid_i0_po0 = index ? LOCK_7_2_5_6688: LOCK_7_2_4_6144;
    adjustPingPongBuffer(window_buf8_buf8d,buf8_ptr,buf8d_ptr,0,index);
    output_window_i0_po0->ptr = window_buf8_buf8d->head;
    lockid_i0_po1 = index ? LOCK_7_1_1_0: LOCK_7_1_0_0;
    adjustPingPongBuffer(window_buf9_buf9d,buf9_ptr,buf9d_ptr,0,index);
    output_window_i0_po1->ptr = window_buf9_buf9d->head;
    lockid_i0_po2 = index ? LOCK_7_2_7_2048: LOCK_7_2_6_0;
    adjustPingPongBuffer(window_buf10_buf10d,buf10_ptr,buf10d_ptr,0,index);
    output_window_i0_po2->ptr = window_buf10_buf10d->head;
    lockid_i0_po3 = index ? LOCK_7_1_2_0: LOCK_7_2_8_7168;
    adjustPingPongBuffer(window_buf11_buf11d,buf11_ptr,buf11d_ptr,0,index);
    output_window_i0_po3->ptr = window_buf11_buf11d->head;
    index = 1 - index;

    chess_memory_fence();

    if(proc_7_1_bounds > 0)
      --proc_7_1_bounds;
  }
  done();
  if (sync_buffer[0] > 0) break;
  }
  return 0;
}