#include <iostream>
#include <fstream>
#include <cmath>

#include "constants.h"

using namespace std;

float A_float[I][K];
float B_float[K][J];
float C_float[I][J];

fm_t A[I][K];
fm_t B[K][J];
fm_t C[I][J] = {0};

void readBin() {

	//Read A and cast to fm_t
	ifstream ifsA("A.bin", ios::in | ios::binary);
	ifsA.read((char*)(&A_float[0][0]), I*K*sizeof(float));
	ifsA.close();
	for(int i = 0; i < I; i++) {
		for(int k = 0; k < K; k++) {
			A[i][k] = (fm_t) A_float[i][k];
		}
	}

	//Read B and cast to fm_t
	ifstream ifsB("B.bin", ios::in | ios::binary);
	ifsB.read((char*)(&B_float[0][0]), K*J*sizeof(float));
	ifsB.close();
	for(int k = 0; k < K; k++) {
		for(int j = 0; j < J; j++) {
			B[k][j] = (fm_t) B_float[k][j];
		}
	}

	//Read C and cast to fm_t
	ifstream ifsC("C.bin", ios::in | ios::binary);
	ifsC.read((char*)(&C_float[0][0]), I*J*sizeof(float));
	ifsC.close();
}

int main() {
	long double MSE = 0.0;

	readBin();
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
