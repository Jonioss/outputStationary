#include "constants.h"

// ---------------------------------------
// -------------FOR TILING----------------
// ---------------------------------------
// void matMulTile(const fm_t A[I/NUM_OF_TILES][K],const fm_t B[K][J/NUM_OF_TILES], fm_t C[I/NUM_OF_TILES][J/NUM_OF_TILES]) {
void matMulTile(hls::stream<fm_t> &A_stream, hls::stream<fm_t> &B_stream, hls::stream<fm_t> &C_stream){
	mat_mul_i:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		mat_mul_j:
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			fm_t psum = 0;
			mat_mul_k:
			for(int k = 0; k < K; k++) {
				// psum += A[i][k] * B[k][j];
				psum += A_stream.read() * B_stream.read();
			}
			//C[i][j] = psum;
			C_stream.write(psum);
		}
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

void matMulTiles(hls::stream<fm_t> A_streams[NUM_OF_TILES], hls::stream<fm_t> B_streams[NUM_OF_TILES], hls::stream<fm_t> C_streams[NUM_OF_TILES]) {
	#pragma HLS INLINE off
	#pragma HLS DATAFLOW
	for(int t=0; t<NUM_OF_TILES; t++){
		#pragma HLS UNROLL
		matMulTile(A_streams[t], B_streams[t], C_streams[t]);
	}
}








