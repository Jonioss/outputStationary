#include "constants.h"

void storeOutputToDRAM(const fm_t C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES], hls::vector<fm_t, 16> C_DRAM[I][J/16]) {
	#pragma HLS INLINE off
	store_C_ti:
	for(int ti=0; ti<NUM_OF_TILES; ti++){
		store_C_ik:
		for(int ik=0; ik<I/NUM_OF_TILES; ik++){
			store_C_tj:
			for(int tj=0; tj<NUM_OF_TILES; tj++){
				store_C_jv:
				for(int jv = 0; jv < J/(NUM_OF_TILES*16); jv++) {
					#pragma HLS PIPELINE II=1
					#pragma HLS LOOP_FLATTEN
					hls::vector<fm_t, 16> c_vec;
					for(int v = 0; v < 16; v++) {
						#pragma HLS UNROLL
						c_vec[v] = C_BUF[ti][tj][ik][jv*16 + v];
					}
					C_DRAM[ti*I/NUM_OF_TILES + ik][tj*J/(NUM_OF_TILES*16) + jv] = c_vec;
				}
			}
		}
	}
}

void loadAFromDRAM(const hls::vector<fm_t, 16> A_DRAM[I][K/16], fm_t A_BUF[I][K]) {
	#pragma HLS INLINE off
	load_A:
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K/16; k++) {
			#pragma HLS PIPELINE II=1
			#pragma HLS LOOP_FLATTEN
			const hls::vector<fm_t, 16> a_vec = A_DRAM[i][k];
			for(int v = 0; v < 16; v++) {
				#pragma HLS UNROLL
				A_BUF[i][k*16 + v] = a_vec[v];
			}
		}
	}
}

void loadBFromDRAM(const hls::vector<fm_t, 16> B_DRAM[K/16][J], fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES]) {
	#pragma HLS INLINE off
	load_B:
	for(int k = 0; k < K/16; k++) {
		for(int t=0; t<NUM_OF_TILES; t++){
			for(int j = 0; j < J/NUM_OF_TILES; j++) {
				#pragma HLS PIPELINE II=1
				#pragma HLS LOOP_FLATTEN
				const hls::vector<fm_t, 16> b_vec = B_DRAM[k][j + t*J/NUM_OF_TILES];
				for(int v = 0; v < 16; v++) {
					#pragma HLS UNROLL
					B_BUF[t][k*16+v][j] = b_vec[v];
				}
			}
		}
	}
}
