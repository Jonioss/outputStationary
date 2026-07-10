#include "constants.h"
 
void loadTileAFromBUF(const float A_BUF[I][K], float A_TILE[I/NUM_OF_TILES][K], const int tileA) {
    #pragma HLS INLINE off
	load_tile_A_buf:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k++) {
			#pragma HLS PIPELINE II=1
			A_TILE[i][k] = A_BUF[i + tileA*(I/NUM_OF_TILES)][k];
		}
	}
}
 
void storeTileToDRAM(const float C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES], float C_DRAM[I/NUM_OF_TILES][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	#pragma HLS DATAFLOW
	store_C_tile:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		#pragma HLS PIPELINE II=1
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			#pragma HLS UNROLL
			C_DRAM[i][j] = C_TILE[i][j];
		}
	}
}
 
void storeTilesToDRAM(const float C_TILES[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES], float C_DRAM[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES]) {
    #pragma HLS INLINE off
	#pragma HLS DATAFLOW
	for(int t=0; t<NUM_OF_TILES; t++){
		#pragma HLS UNROLL
		storeTileToDRAM(C_TILES[t], C_DRAM[t]);
	}
}