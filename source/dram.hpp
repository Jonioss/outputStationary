#include "constants.h"

// Function to store C's values from its local buffer to DRAM
void storeOutputToDRAM(const float C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES], hls::burst_maxi<hls::vector<float, VEC_SIZE>> C_DRAM) {
	#pragma HLS INLINE off // Pragma to disable inlining for this function
	C_DRAM.write_request(0, I*J/VEC_SIZE); // Request to write I*J/VEC_SIZE elements to DRAM
	store_C_ti:
	for(int ti=0; ti<NUM_OF_TILES; ti++){
		store_C_ik:
		for(int ik=0; ik<I/NUM_OF_TILES; ik++){
			store_C_tj:
			for(int tj=0; tj<NUM_OF_TILES; tj++){
				store_C_jv:
				for(int jv = 0; jv < J/(NUM_OF_TILES*VEC_SIZE); jv++) {
					#pragma HLS PIPELINE II=1 // Enable pipelining
					#pragma HLS LOOP_FLATTEN // Flatten the loop hierarchy for better performance
					hls::vector<float, VEC_SIZE> c_vec; // Helper vector with VEC_SIZE elements
					for(int v = 0; v < VEC_SIZE; v++) {
						#pragma HLS UNROLL // Unroll the loop to allow parallel execution of iterations
						c_vec[v] = C_BUF[ti][tj][ik][jv*VEC_SIZE + v]; // Put the buffer's values in a vector
					}
					C_DRAM.write(c_vec); // Write to DRAM after receiving permission from the request
				}
			}
		}
	}
	C_DRAM.write_response(); // Wait until all data has been written to DRAM
}

// Function to read A's values from DRAM to its local buffer
void loadAFromDRAM(hls::burst_maxi<hls::vector<float, VEC_SIZE>> A_DRAM, float A_BUF[I][K]) {
	#pragma HLS INLINE off // Pragma to disable inlining for this function
	A_DRAM.read_request(0, I*K/VEC_SIZE); // Request to DRAM to read A's values
	load_A:
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K/VEC_SIZE; k++) {
			#pragma HLS PIPELINE II=1 // Enable pipelining
			#pragma HLS LOOP_FLATTEN // Flatten the loop hierarchy for better performance
			const hls::vector<float, VEC_SIZE> a_vec = A_DRAM.read(); // Helper vector to read A's values from DRAM, 16 at a single burst
			for(int v = 0; v < VEC_SIZE; v++) {
				#pragma HLS UNROLL // Unroll the loop to allow parallel execution of iterations
				A_BUF[i][k*VEC_SIZE + v] = a_vec[v]; // Move A's values to the Buffer
			}
		}
	}
}

// Function to read B's values from DRAM to its local buffer
void loadBFromDRAM(hls::burst_maxi<hls::vector<float, VEC_SIZE>> B_DRAM, float B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES]) {
	#pragma HLS INLINE off // Pragma to disable inlining for this function
	B_DRAM.read_request(0, K*J/VEC_SIZE); // Request to DRAM to read B's values
	load_B:
	for(int k = 0; k < K/VEC_SIZE; k++) {
		for(int t=0; t<NUM_OF_TILES; t++){
			for(int j = 0; j < J/NUM_OF_TILES; j++) {
				#pragma HLS PIPELINE II=1 // Enable pipelining
				#pragma HLS LOOP_FLATTEN // Flatten the loop hierarchy for better performance
				const hls::vector<float, VEC_SIZE> b_vec = B_DRAM.read(); // Helper vector to read B's values from DRAM, 16 at a single burst
				for(int v = 0; v < VEC_SIZE; v++) {
					#pragma HLS UNROLL // Unroll the loop to allow parallel execution of iterations
					B_BUF[t][k*VEC_SIZE+v][j] = b_vec[v]; // Move B's values to the Buffer
				}
			}
		}
	}
}
