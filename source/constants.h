#include <ap_fixed.h>
#include "host_visible.h"
#include <hls_stream.h>
#include <hls_vector.h>
#include "hls_burst_maxi.h"

#define NUM_OF_TILES 2
#define VEC_SIZE 16 // 512/32=16

//typedef ap_fixed<32, 8> fm_t;
typedef float fm_t;

void gemm1(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t C_DRAM[I][J]);
void gemm2(const hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> A_DRAM, 
    const hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> B_DRAM, 
    hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> C_DRAM);
