#include "constants.h"

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