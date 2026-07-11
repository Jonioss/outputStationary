#include "constants.h"

// Functions to stream Buffer's values from the buffers to the matmultile functions
void loader_A_tiles(const float A_BUF[I][K], hls::stream<float> A_streams[NUM_OF_TILES], const int tileA){
    #pragma HLS INLINE off // Pragma to turn off inlining for this function
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k++) {
            #pragma HLS PIPELINE II=1 // Enable pipelining
            #pragma HLS LOOP_FLATTEN // Flatten the loop hierarchy for better performance
            const float temp = A_BUF[i + tileA*(I/NUM_OF_TILES)][k];
            for(int t=0; t<NUM_OF_TILES; t++){
                #pragma HLS UNROLL // Unroll the loop to allow parallel execution of iterations
                A_streams[t].write(temp); // put A's values in a stream
            }
		}
	}
}
void loader_B_tile(const float B_TILE[K][J/NUM_OF_TILES], hls::stream<float> B_streams[J/NUM_OF_TILES]){
    #pragma HLS INLINE off // Pragma to turn off inlining for this function
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS LOOP_FLATTEN
			for(int j = 0; j < J/NUM_OF_TILES; j++) {
                #pragma HLS UNROLL
                B_streams[j].write(B_TILE[k][j]); // put B's values in a stream
			}
		}
	}
}
void loader_B_tiles(const float B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES], hls::stream<float> B_streams[NUM_OF_TILES][J/NUM_OF_TILES]){
    #pragma HLS INLINE off
    #pragma HLS DATAFLOW
    for(int t=0; t<NUM_OF_TILES; t++){
        #pragma HLS UNROLL
        loader_B_tile(B_BUF[t], B_streams[t]); // Pass multiple tiles to multiple streams at the same time
    }
}

// Functions to stream Buffer's values from the matmultile functions to the buffer
void loader_C_tile(hls::stream<float> C_streams[J/NUM_OF_TILES], float C[I/NUM_OF_TILES][J/NUM_OF_TILES]){
    #pragma HLS INLINE off
    for(int i = 0; i < I/NUM_OF_TILES; i++) {
        #pragma HLS PIPELINE II=1
        #pragma HLS LOOP_FLATTEN
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
            #pragma HLS UNROLL
            C[i][j] = C_streams[j].read(); // Put C's values in a stream
		}
	}
}
void loader_C_tiles(hls::stream<float> C_streams[NUM_OF_TILES][J/NUM_OF_TILES], float C_TILES[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES]){
    #pragma HLS INLINE off
    #pragma HLS DATAFLOW
    for(int t=0; t<NUM_OF_TILES; t++){
        #pragma HLS UNROLL
        loader_C_tile(C_streams[t], C_TILES[t]); // Pass multiple tiles to multiple streams at the same time
    }
}