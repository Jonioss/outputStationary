#include "constants.h"

void loadTileAFromDRAM(fm_t A_DRAM[I][K], fm_t A_TILE[I/NUM_OF_TILES][K], int tileA) {
    #pragma HLS INLINE off
	load_tile_A:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k+=2) {
#pragma HLS PIPELINE II=1
			A_TILE[i][k] = A_DRAM[i+tileA*I/NUM_OF_TILES][k];
			A_TILE[i][k+1] = A_DRAM[i+tileA*I/NUM_OF_TILES][k+1];
		}
	}
}

void loadTileBFromDRAM(fm_t B_DRAM[K][J/NUM_OF_TILES], fm_t B_TILE[K][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	load_tile_B:
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			B_TILE[k][j] = B_DRAM[k][j];
		}
	}
}

void loadTilesBFromDRAM(fm_t B_DRAM[NUM_OF_TILES][K][J/NUM_OF_TILES], fm_t B_TILES[NUM_OF_TILES][K][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	#pragma HLS DATAFLOW
	loadTileBFromDRAM(B_DRAM[0], B_TILES[0]);
	loadTileBFromDRAM(B_DRAM[1], B_TILES[1]);
}

void storeTileToDRAM(fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[I][J], int tileA, int tileB) {
    #pragma HLS INLINE off
	store_C_tile:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			C_DRAM[i+tileA*I/NUM_OF_TILES][j+tileB*J/NUM_OF_TILES] = C_TILE[i][j];
		}
	}
}
