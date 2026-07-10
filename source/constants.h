#include <ap_fixed.h>
#include "host_visible.h"
#include <hls_stream.h>
#include <hls_vector.h>
#include "hls_burst_maxi.h"

#define NUM_OF_TILES 2 // Number of tiles to break A and B into
#define VEC_SIZE 16 // 512/32=16, where 512 is AXI Bus width and 32 is size of float

void gemm1(float A_DRAM[I][K], float B_DRAM[K][J], float C_DRAM[I][J]);
void gemm2(const hls::burst_maxi<hls::vector<float, VEC_SIZE>> A_DRAM, 
    const hls::burst_maxi<hls::vector<float, VEC_SIZE>> B_DRAM, 
    hls::burst_maxi<hls::vector<float, VEC_SIZE>> C_DRAM);
