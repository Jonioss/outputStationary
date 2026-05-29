#include "constants.h"

void loadTileAFromDRAM(const fm_t A_DRAM[I][K], fm_t A_TILE[NUM_OF_TILES][I/NUM_OF_TILES][K], const int tileA) {
    #pragma HLS INLINE off
	load_tile_A:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k++) {
#pragma HLS UNROLL factor=NUM_OF_TILES
#pragma HLS PIPELINE II=1
			const int temp = A_DRAM[i+tileA*I/NUM_OF_TILES][k];
			for(int t=0; t<NUM_OF_TILES; t++){
				#pragma HLS UNROLL
				A_TILE[t][i][k] = temp;
			}
		}
	}
}

void loadTileBFromDRAM(const fm_t B_DRAM[K][J/NUM_OF_TILES], fm_t B_TILE[K][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	load_tile_B:
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			B_TILE[k][j] = B_DRAM[k][j];
		}
	}
}

void loadTilesBFromDRAM(const fm_t B_DRAM[NUM_OF_TILES][K][J/NUM_OF_TILES], fm_t B_TILES[NUM_OF_TILES][K][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	#pragma HLS DATAFLOW
	for(int t=0; t<NUM_OF_TILES; t++){
		#pragma HLS UNROLL
		loadTileBFromDRAM(B_DRAM[t], B_TILES[t]);
	}
}

void storeTileToDRAM(const fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[I/NUM_OF_TILES][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	store_C_tile:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			C_DRAM[i][j] = C_TILE[i][j];
		}
	}
}

void storeTilesToDRAM(const fm_t C_TILES[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	#pragma DATAFLOW
	for(int t=0; t<NUM_OF_TILES; t++){
		#pragma HLS UNROLL
		storeTileToDRAM(C_TILES[t], C_DRAM[t]);
	}
}
