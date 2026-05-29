#include "constants.h"

void loadTileAFromDRAM(fm_t A_DRAM[I][K], fm_t A_TILE[I/NUM_OF_TILES][K], int tileA) {
#pragma HLS INLINE
	load_tile_A:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k+=2) {
#pragma HLS PIPELINE II=1
			A_TILE[i][k] = A_DRAM[i+tileA*I/NUM_OF_TILES][k];
			A_TILE[i][k+1] = A_DRAM[i+tileA*I/NUM_OF_TILES][k+1];
		}
	}
}

void loadTileBFromDRAM(fm_t B_DRAM[K][J], fm_t B_TILE[K][J/NUM_OF_TILES], int tileB) {
	load_tile_B:
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			B_TILE[k][j] = B_DRAM[k][j+tileB*J/NUM_OF_TILES];
		}
	}
}

void storeTileToDRAM(fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[I][J], int tileA, int tileB) {
	store_C_tile:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			C_DRAM[i+tileA*I/NUM_OF_TILES][j+tileB*J/NUM_OF_TILES] = C_TILE[i][j];
		}
	}
}
