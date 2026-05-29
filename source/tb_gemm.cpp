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

int main() {
	long double MSE = 0.0;

	srand(0);

	generateMats();
	gemm2(A, B, C);

	for(int i = 0; i < I; i++) {
		for(int j = 0; j < J; j++) {
			MSE += pow((C_float[i][j] - (float)C[i][j]), 2);
		}
	}
	MSE = MSE / (I*J);

	cout << "MSE: " << MSE << endl;

	return 0;
}