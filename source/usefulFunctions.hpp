#include "constants.h"

// ---------------------------------------
// -------------FOR TILING----------------
// ---------------------------------------

// Function to calculate partial sum
void calculatepsum(hls::stream<float> &A_stream, hls::stream<float> B_streams[J/NUM_OF_TILES], float psum[J/NUM_OF_TILES]) {
	#pragma HLS INLINE off
	#pragma HLS DATAFLOW
	for(int k = 0; k < K; k++) {
		#pragma HLS PIPELINE II=1 rewind // Allows the next iteration to start before the previous one ends
		const float a_val = A_stream.read();
		mat_mul_j:
		for(int j = 0; j < J/NUM_OF_TILES; j++) { // Performs 32 MACs in parallel to calculate the partial sum for each of B's lines
			#pragma HLS UNROLL
			psum[j] += a_val * B_streams[j].read();
		}
	}
}

void psumToStream(float psum[J/NUM_OF_TILES], hls::stream<float> C_streams[J/NUM_OF_TILES]) {
	#pragma HLS INLINE off
	for(int j = 0; j < J/NUM_OF_TILES; j++) {
		#pragma HLS UNROLL
		C_streams[j].write(psum[j]); // Pass the partial sums to a stream to be written to the output buffer C
	}
}

void matMulTile(hls::stream<float> &A_stream, hls::stream<float> B_streams[J/NUM_OF_TILES], hls::stream<float> C_streams[J/NUM_OF_TILES]){
	#pragma HLS INLINE off
	mat_mul_i:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		#pragma HLS DATAFLOW // Concurrent execution of functions
		float psum[J/NUM_OF_TILES] = {0}; // Initialize the partial sum array to zero for each tile
		#pragma HLS ARRAY_PARTITION variable=psum type=complete dim=1 // Partition for parallel access to elements
		calculatepsum(A_stream, B_streams, psum); // Calculate psum
		psumToStream(psum, C_streams); // Store psum
	}
}

void matMulTiles(hls::stream<float> A_streams[NUM_OF_TILES], hls::stream<float> B_streams[NUM_OF_TILES][J/NUM_OF_TILES], hls::stream<float> C_streams[NUM_OF_TILES][J/NUM_OF_TILES]){
	#pragma HLS INLINE off
	#pragma HLS DATAFLOW
	for(int t=0; t<NUM_OF_TILES; t++){
		#pragma HLS UNROLL
		matMulTile(A_streams[t], B_streams[t], C_streams[t]); // Perform MatMul on 1 tile of A and 2 tiles of B, in parallel
	}
}

