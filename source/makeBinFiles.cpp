#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "host_visible.h"


float A[I][K];
float B[K][J];
float C[I][J];

void initializeC() {
    for (int i = 0; i < I; i++) {
        for (int j = 0; j < J; j++) {
            C[i][j] = 0.0f;
        }
    }
}

void generateMats() {
    for (int i = 0; i < I; i++) {
        for (int k = 0; k < K; k++) {
            A[i][k] = static_cast<float>(rand()) / RAND_MAX;
        }
    }
    for (int k = 0; k < K; k++) {
        for (int j = 0; j < J; j++) {
            B[k][j] = static_cast<float>(rand()) / RAND_MAX;
        }
    }
    initializeC();
    for (int i = 0; i < I; i++) {
        for (int j = 0; j < J; j++) {
            for(int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void saveMatrixBin(const char* filename, const float* data, int rows, int cols) {
    std::ofstream ofs(filename, std::ios::binary | std::ios::out);
    if (!ofs) { std::cerr << "Cannot open: " << filename << "\n"; return; }

    //ofs.write(reinterpret_cast<const char*>(&rows), sizeof(int));
    //ofs.write(reinterpret_cast<const char*>(&cols), sizeof(int));
    ofs.write(reinterpret_cast<const char*>(data), rows * cols * sizeof(float));

    ofs.close();
    std::cout << "Saved " << rows << "x" << cols << " -> " << filename << "\n";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    generateMats();

    saveMatrixBin("A.bin", &A[0][0], I, K);
    saveMatrixBin("B.bin", &B[0][0], K, J);
    saveMatrixBin("C.bin", &C[0][0], I, J);

    return 0;
}