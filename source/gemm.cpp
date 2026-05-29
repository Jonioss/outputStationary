#include "constants.h"
#include "dram.hpp"
#include "load_tiles.hpp"
#include "usefulFunctions.hpp"

using namespace std;

void gemm2(const fm_t A_DRAM[I][K],const fm_t B_DRAM[K][J], fm_t C_DRAM[I][J]) {

#pragma HLS INTERFACE m_axi port=A_DRAM bundle=gmem0
#pragma HLS INTERFACE m_axi port=B_DRAM bundle=gmem1

	fm_t A_BUF[I][K];
	fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=1 type=complete
	fm_t C_BUF[I][J];

	fm_t A_TILE[I/NUM_OF_TILES][K];
	fm_t B_TILES[NUM_OF_TILES][K][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=B_TILES dim=1 type=complete

//#pragma HLS INLINE
	loadInputsFromDRAM(A_DRAM, B_DRAM, A_BUF, B_BUF);
	loop_for_tile_a:
	for(int tileA = 0; tileA < NUM_OF_TILES; tileA++) {
//#pragma HLS UNROLL
		loadTileAFromDRAM(A_BUF, A_TILE, tileA);
		for(int tileB = 0; tileB < NUM_OF_TILES/2; tileB++) {
#pragma HLS DATAFLOW
			const int real_tileB = tileB*2;
			fm_t C_TILES[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES];
			#pragma HLS ARRAY_PARTITION variable=C_TILES dim=1 type=complete
			loadTilesBFromDRAM(B_BUF, B_TILES);
			matMulTile(A_TILE, B_TILES[0], C_TILES[0]);
			//storeTileToDRAM(C_TILE, C_DRAM, tileA, tileB);

			matMulTile(A_TILE, B_TILES[1], C_TILES[1]);
			//storeTileToDRAM(C_TILE2, C_DRAM, tileA, tileB);

			storeTileToDRAM(C_TILES[0], C_BUF, tileA, real_tileB);
			storeTileToDRAM(C_TILES[1], C_BUF, tileA, real_tileB+1);
		}
	}
	storeOutputToDRAM(C_BUF, C_DRAM);
}
