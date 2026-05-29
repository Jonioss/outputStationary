#include "constants.h"

void loadInputsFromDRAM(const fm_t A_DRAM[I][K],const fm_t B_DRAM[K][J], fm_t A_BUF[I][K], fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES]){
	#pragma HLS INLINE off
	load_A:
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K; k++) {
			A_BUF[i][k] = A_DRAM[i][k];
		}
	}
	load_B:
	for(int k = 0; k < K; k++) {
		for(int t=0; t<NUM_OF_TILES; t++){
			for(int j = 0; j < J/NUM_OF_TILES; j++) {
				B_BUF[t][k][j] = B_DRAM[k][t*(J/NUM_OF_TILES) + j];
			}
		}
	}
}
void storeOutputToDRAM(const fm_t C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[I][J]) {
	#pragma HLS INLINE off
	store_C:
	for(int ti=0; ti<NUM_OF_TILES; ti++){
		for(int ik=0; ik<I/NUM_OF_TILES; ik++){
			for(int tj=0; tj<NUM_OF_TILES; tj++){
				for(int jk=0; jk<J/NUM_OF_TILES; jk++){
					C_DRAM[ti*(I/NUM_OF_TILES) + ik][tj*(J/NUM_OF_TILES) + jk] = C_BUF[ti][tj][ik][jk];
				}
			}
		}
	}
}