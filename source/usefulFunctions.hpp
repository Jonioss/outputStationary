#include "constants.h"

// ---------------------------------------
// -------------FOR TILING----------------
// ---------------------------------------

// make psum and write functions
void calculatepsum(hls::stream<fm_t> &A_stream, hls::stream<fm_t> B_streams[J/NUM_OF_TILES], fm_t psum[J/NUM_OF_TILES]) {
	#pragma HLS INLINE off
	for(int k = 0; k < K; k++) {
		#pragma HLS PIPELINE II=1
		const fm_t a_val = A_stream.read();
		mat_mul_j:
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			#pragma HLS UNROLL
			psum[j] += a_val * B_streams[j].read();
		}
	}
}

void psumToStream(fm_t psum[J/NUM_OF_TILES], hls::stream<fm_t> C_streams[J/NUM_OF_TILES]) {
	#pragma HLS INLINE off
	for(int j = 0; j < J/NUM_OF_TILES; j++) {
		#pragma HLS UNROLL
		C_streams[j].write(psum[j]);
	}
}

// void matMulTile(const fm_t A[I/NUM_OF_TILES][K],const fm_t B[K][J/NUM_OF_TILES], fm_t C[I/NUM_OF_TILES][J/NUM_OF_TILES]) {
void matMulTile(hls::stream<fm_t> &A_stream, hls::stream<fm_t> B_streams[J/NUM_OF_TILES], hls::stream<fm_t> C_streams[J/NUM_OF_TILES]){
	#pragma HLS INLINE off
	mat_mul_i:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		#pragma HLS DATAFLOW
		fm_t psum[J/NUM_OF_TILES] = {0};
		#pragma HLS STREAM variable=psum type=pipo depth=2
		#pragma HLS ARRAY_PARTITION variable=psum type=complete dim=1
		calculatepsum(A_stream, B_streams, psum);
		psumToStream(psum, C_streams);
	}
}

void zeroCTile(fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES]) {
	zeroC:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			C_TILE[i][j] = 0;
		}
	}
}

void matMulTiles(hls::stream<fm_t> A_streams[NUM_OF_TILES], hls::stream<fm_t> B_streams[NUM_OF_TILES][J/NUM_OF_TILES], hls::stream<fm_t> C_streams[NUM_OF_TILES][J/NUM_OF_TILES]){
	#pragma HLS INLINE off
	#pragma HLS DATAFLOW
	for(int t=0; t<NUM_OF_TILES; t++){
		#pragma HLS UNROLL
		matMulTile(A_streams[t], B_streams[t], C_streams[t]);
	}
}

