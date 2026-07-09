#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

#include "constants.h"

using namespace std;

float A_float[I][K];
float B_float[K][J];
float C_float[I][J];

fm_t A[I][K];
fm_t B[K][J];
fm_t C[I][J] = {0};

hls::vector<fm_t, 16> A_vec[I][K/16];
hls::vector<fm_t, 16> B_vec[K/16][J];
hls::vector<fm_t, 16> C_vec[I][J/16];

void generateMats() {

	//Generate A and cast to fm_t
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K; k++) {
			A_float[i][k] = static_cast<float>(rand()) / RAND_MAX;
			A[i][k] = (fm_t) A_float[i][k];
		}
	}

	//Generate B and cast to fm_t
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J; j++) {
			B_float[k][j] = static_cast<float>(rand()) / RAND_MAX;
			B[k][j] = (fm_t) B_float[k][j];
		}
	}

	//Generate golden C using float
	for(int i = 0; i < I; i++) {
		for(int j = 0; j < J; j++) {
			C_float[i][j] = 0.0f;
			C[i][j] = 0;
			for(int k = 0; k < K; k++) {
				C_float[i][j] += A_float[i][k] * B_float[k][j];
			}
		}
	}
}

void initVectors() {
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K/16; k++) {
			for(int v = 0; v < 16; v++) {
				A_vec[i][k][v] = A[i][k*16 + v];
			}
		}
	}
	for(int k = 0; k < K/16; k++) {
		for(int j = 0; j < J; j++) {
			for(int v = 0; v < 16; v++) {
				B_vec[k][j][v] = B[k*16+v][j];
			}
		}
	}
	for(int i = 0; i < I; i++) {
		for(int j = 0; j < J/16; j++) {
			for(int v = 0; v < 16; v++) {
				C_vec[i][j][v] = (fm_t) 0.0;
			}
		}
	}
}

int main() {
	long double MSE = 0.0;

	srand(0);

	generateMats();
	initVectors();

	// Calculate GeMM
	//gemm2(A_vec, B_vec, C_vec);
	gemm2(&A_vec[0][0], &B_vec[0][0], &C_vec[0][0]);

	// Unpack results
	for(int i = 0; i < I; i++) {
		for(int j = 0; j < J/16; j++) {
			for(int v = 0; v < 16; v++) {
				C[i][j*16 + v] = C_vec[i][j][v];
			}
		}
	}

	for(int i = 0; i < I; i++) {
		for(int j = 0; j < J; j++) {
			MSE += pow((C_float[i][j] - (float)C[i][j]), 2);
		}
	}
	MSE = MSE / (I*J);

	cout << "MSE: " << MSE << endl;

	return 0;
}