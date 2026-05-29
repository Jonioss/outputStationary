#include "constants.h"
#include "dram.hpp"
#include "load_tiles.hpp"
#include "usefulFunctions.hpp"

using namespace std;

void gemm2(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t C_DRAM[I][J]) {

#pragma HLS INTERFACE m_axi port=A_DRAM bundle=gmem0
#pragma HLS INTERFACE m_axi port=B_DRAM bundle=gmem1

	fm_t A_BUF[I][K];
#pragma HLS ARRAY_PARTITION variable=A_BUF dim=2 type=cyclic factor=2
	fm_t B_BUF[K][J];
#pragma HLS ARRAY_PARTITION variable=B_BUF dim=2 type=block factor=2
	fm_t C_BUF[I][J];
#pragma HLS ARRAY_PARTITION variable=C_BUF dim=1 block factor=2
#pragma HLS ARRAY_PARTITION variable=C_BUF dim=2 block factor=2

	fm_t A_TILE[I/NUM_OF_TILES][K];
#pragma HLS ARRAY_PARTITION variable=A_TILE dim=2 type=cyclic factor=2
	fm_t B_TILE[K][J/NUM_OF_TILES];
#pragma HLS ARRAY_PARTITION variable=B_TILE dim=1 type=cyclic factor=2
	fm_t B_TILE2[K][J/NUM_OF_TILES];
#pragma HLS ARRAY_PARTITION variable=B_TILE2 dim=1 cyclic factor=2

//#pragma HLS INLINE
	loadInputsFromDRAM(A_DRAM, B_DRAM, A_BUF, B_BUF);
	loop_for_tile_a:
	for(int tileA = 0; tileA < NUM_OF_TILES; tileA++) {
//#pragma HLS UNROLL
		loadTileAFromDRAM(A_BUF, A_TILE, tileA);
		for(int tileB = 0; tileB < NUM_OF_TILES/2; tileB++) {
#pragma HLS DATAFLOW
			const int real_tileB = tileB*2;
			fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES];
//#pragma HLS ARRAY_PARTITION variable=C_TILE type=cyclic
			loadTileBFromDRAM(B_BUF, B_TILE, real_tileB);
			loadTileBFromDRAM(B_BUF, B_TILE2, real_tileB+1);

			matMulTile(A_TILE, B_TILE, C_TILE);
			//storeTileToDRAM(C_TILE, C_DRAM, tileA, tileB);

			fm_t C_TILE2[I/NUM_OF_TILES][J/NUM_OF_TILES];
			matMulTile(A_TILE, B_TILE2, C_TILE2);
			//storeTileToDRAM(C_TILE2, C_DRAM, tileA, tileB);

			storeTileToDRAM(C_TILE, C_BUF, tileA, real_tileB);
			storeTileToDRAM(C_TILE2, C_BUF, tileA, real_tileB+1);
		}
	}
	storeOutputToDRAM(C_BUF, C_DRAM);
}
