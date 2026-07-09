#include "constants.h"

void storeOutputToDRAM(const fm_t C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES], hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> C_DRAM) {
	#pragma HLS INLINE off
	C_DRAM.write_request(0, I*J/VEC_SIZE);
	store_C_ti:
	for(int ti=0; ti<NUM_OF_TILES; ti++){
		store_C_ik:
		for(int ik=0; ik<I/NUM_OF_TILES; ik++){
			store_C_tj:
			for(int tj=0; tj<NUM_OF_TILES; tj++){
				store_C_jv:
				for(int jv = 0; jv < J/(NUM_OF_TILES*VEC_SIZE); jv++) {
					#pragma HLS PIPELINE II=1
					#pragma HLS LOOP_FLATTEN
					hls::vector<fm_t, VEC_SIZE> c_vec;
					for(int v = 0; v < VEC_SIZE; v++) {
						#pragma HLS UNROLL
						c_vec[v] = C_BUF[ti][tj][ik][jv*VEC_SIZE + v];
					}
					C_DRAM.write(c_vec);
				}
			}
		}
	}
	C_DRAM.write_response();
}

void loadAFromDRAM(hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> A_DRAM, fm_t A_BUF[I][K]) {
	#pragma HLS INLINE off
	A_DRAM.read_request(0, I*K/VEC_SIZE);
	load_A:
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K/VEC_SIZE; k++) {
			#pragma HLS PIPELINE II=1
			#pragma HLS LOOP_FLATTEN
			const hls::vector<fm_t, VEC_SIZE> a_vec = A_DRAM.read();
			for(int v = 0; v < VEC_SIZE; v++) {
				#pragma HLS UNROLL
				A_BUF[i][k*VEC_SIZE + v] = a_vec[v];
			}
		}
	}
}

void loadBFromDRAM(hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> B_DRAM, fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES]) {
	#pragma HLS INLINE off
	B_DRAM.read_request(0, K*J/VEC_SIZE);
	load_B:
	for(int k = 0; k < K/VEC_SIZE; k++) {
		for(int t=0; t<NUM_OF_TILES; t++){
			for(int j = 0; j < J/NUM_OF_TILES; j++) {
				#pragma HLS PIPELINE II=1
				#pragma HLS LOOP_FLATTEN
				const hls::vector<fm_t, VEC_SIZE> b_vec = B_DRAM.read();
				for(int v = 0; v < VEC_SIZE; v++) {
					#pragma HLS UNROLL
					B_BUF[t][k*VEC_SIZE+v][j] = b_vec[v];
				}
			}
		}
	}
}
