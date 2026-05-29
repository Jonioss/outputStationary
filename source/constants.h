#include <ap_fixed.h>
#include "host_visible.h"

#define NUM_OF_TILES 2

typedef ap_fixed<32, 8> fm_t;

void gemm1(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t C_DRAM[I][J]);
void gemm2(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t C_DRAM[I][J]);
