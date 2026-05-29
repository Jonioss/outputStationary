#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

#include "constants.h"

void matMul1(fm_t A[I][K], fm_t B[K][J], fm_t C[I][J]);
void loadInputsFromDRAM(fm_t A_DRAM[I][K], fm_t B_DRAM[K][J], fm_t A_BUF[I][K], fm_t B_BUF[K][J]);
void storeOutputToDRAM(fm_t C_BUF[I][J], fm_t C_DRAM[I][J]);

void matMulTile(fm_t A[I/NUM_OF_TILES][K], fm_t B[K][J/NUM_OF_TILES], fm_t C[I/NUM_OF_TILES][J/NUM_OF_TILES]);
void loadTileAFromDRAM(fm_t A_DRAM[I][K], fm_t A_TILE[I/NUM_OF_TILES][K], int tileA);
void loadTileBFromDRAM(fm_t B_DRAM[K][J], fm_t B_TILE[K][J/NUM_OF_TILES], int tileB);
void storeTileToDRAM(fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_DRAM[I][J], int tileA, int tileB);
void zeroCTile(fm_t C_TILE[I/NUM_OF_TILES][J/NUM_OF_TILES]);

#endif
