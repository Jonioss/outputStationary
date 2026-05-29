#include "constants.h"

// ---------------------------------------
// -------------FOR TILING----------------
// ---------------------------------------
void matMulTile(fm_t A[I/NUM_OF_TILES][K], fm_t B[K][J/NUM_OF_TILES], fm_t C[I/NUM_OF_TILES][J/NUM_OF_TILES]) {
	mat_mul_i:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		mat_mul_j:
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			fm_t psum = 0;
			mat_mul_k:
			for(int k = 0; k < K; k++) {
				psum += A[i][k] * B[k][j];
			}
			C[i][j] = psum;
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










