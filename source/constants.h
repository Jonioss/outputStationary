#include <ap_fixed.h>
#include "host_visible.h"
#include <hls_stream.h>
#include <hls_vector.h>

#define NUM_OF_TILES 2

typedef ap_fixed<32, 8> fm_t;

void gemm1(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t C_DRAM[I][J]);
void gemm2(const hls::vector<fm_t, 16> A_DRAM[I][K/16], const hls::vector<fm_t, 16> B_DRAM[K/16][J], hls::vector<fm_t, 16> C_DRAM[I][J/16]);
