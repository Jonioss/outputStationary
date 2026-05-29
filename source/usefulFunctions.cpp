#include "constants.h"
#include "usefulFunctions.h"

void matMul1(fm_t A[I][K], fm_t B[K][J], fm_t C[I][J]) {
	mat_mul_i:
	for(int i = 0; i < I; i++) {
		mat_mul_j:
		for(int j = 0; j < J; j++) {
			mat_mul_k:
			for(int k = 0; k < K; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void loadInputsFromDRAM(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t A_BUF[I][K], fm_t B_BUF[K][J]) {
	load_A:
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K; k++) {
			A_BUF[i][k] = A_DRAM[i][k];
		}
	}
	load_B:
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J; j++) {
			B_BUF[k][j] = B_DRAM[k][j];
		}
	}
}

void storeOutputToDRAM(fm_t C_BUF[I][J], fm_t C_DRAM[I][J]) {
	store_C:
	for(int i = 0; i < I; i++) {
		for(int j = 0; j < J; j++) {
			C_DRAM[i][j] = C_BUF[i][j];
		}
	}
}

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

void loadTileAFromDRAM(fm_t A_DRAM[I][K], fm_t A_TILE[I/NUM_OF_TILES][K], int tileA) {
#pragma HLS INLINE
	load_tile_A:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k+=2) {
#pragma HLS PIPELINE II=1
			A_TILE[i][k] = A_DRAM[i+tileA*I/NUM_OF_TILES][k];
			A_TILE[i][k+1] = A_DRAM[i+tileA*I/NUM_OF_TILES][k+1];
		}
	}
}

void loadTileBFromDRAM(fm_t B_DRAM[K][J], fm_t B_TILE[K][J/NUM_OF_TILES], int tileB) {
	load_tile_B:
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			B_TILE[k][j] = B_DRAM[k][j+tileB*J/NUM_OF_TILES];
		}
	}
}

void storeTileToDRAM(fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[I][J], int tileA, int tileB) {
	store_C_tile:
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
			C_DRAM[i+tileA*I/NUM_OF_TILES][j+tileB*J/NUM_OF_TILES] = C_TILE[i][j];
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










