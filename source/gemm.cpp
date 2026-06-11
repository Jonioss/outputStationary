#include "constants.h"
#include "dram.hpp"
#include "load_tiles.hpp"
#include "usefulFunctions.hpp"
#include "loader.hpp"

using namespace std;

void gemm2(const hls::vector<fm_t, 16> A_DRAM[I][K/16],const hls::vector<fm_t, 16> B_DRAM[K/16][J], hls::vector<fm_t, 16> C_DRAM[I][J/16]) {

	#pragma HLS INTERFACE m_axi offset=slave port=A_DRAM bundle=gmem0
	#pragma HLS INTERFACE m_axi offset=slave port=B_DRAM bundle=gmem1

	fm_t A_BUF[I][K];
	#pragma HLS bind_storage variable=A_BUF type=RAM_1WNR impl=BRAM
	fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=1 type=complete
	#pragma HLS bind_storage variable=B_BUF type=RAM_1WNR impl=BRAM
	fm_t C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=1 type=complete
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=2 type=complete
	#pragma HLS bind_storage variable=C_BUF type=RAM_1WNR impl=BRAM
	fm_t A_TILE[I/NUM_OF_TILES][K];
	#pragma HLS bind_storage variable=A_TILE type=RAM_1WNR impl=BRAM
	#pragma HLS ARRAY_PARTITION variable=A_TILE dim=2 type=cyclic factor=NUM_OF_TILES
	#pragma HLS stream variable=A_TILE type=pipo depth=2
	fm_t B_TILES[NUM_OF_TILES][K][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=B_TILES dim=1 type=complete
	#pragma HLS bind_storage variable=B_TILES type=RAM_1WNR impl=BRAM
	#pragma HLS stream variable=B_TILES type=pipo depth=2

//#pragma HLS INLINE
	loadInputsFromDRAM(A_DRAM, B_DRAM, A_BUF, B_BUF);
	loop_for_tile_a:
	for(int tileA = 0; tileA < NUM_OF_TILES; tileA++) {
		#pragma HLS DATAFLOW
		hls::stream<fm_t> A_streams[NUM_OF_TILES];
		//#pragma HLS ARRAY_PARTITION variable=A_streams dim=1 complete
		#pragma HLS STREAM variable=A_streams type=pipo depth=2
		hls::stream<fm_t> B_streams[NUM_OF_TILES][J/NUM_OF_TILES];
		//#pragma HLS ARRAY_PARTITION variable=B_streams dim=1 type=complete
		//#pragma HLS ARRAY_PARTITION variable=B_streams dim=2 complete
		#pragma HLS stream variable=B_streams type=pipo depth=2
		hls::stream<fm_t> C_streams[NUM_OF_TILES][J/NUM_OF_TILES];
		//#pragma HLS ARRAY_PARTITION variable=C_streams dim=2 type=complete
		//#pragma HLS ARRAY_PARTITION variable=C_streams dim=1 type=complete
		#pragma HLS STREAM variable=C_streams type=pipo depth=2
		fm_t C_TILES[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES];
		#pragma HLS ARRAY_PARTITION variable=C_TILES dim=1 type=complete
		#pragma HLS bind_storage variable=C_TILES type=RAM_1WNR impl=BRAM
		#pragma HLS stream variable=C_TILES type=pipo depth=2
		loadTileAFromDRAM(A_BUF, A_TILE, tileA);
		loader_A_tiles(A_TILE, A_streams);
		loadTilesBFromDRAM(B_BUF, B_TILES);
		loader_B_tiles(B_TILES, B_streams);
		matMulTiles(A_streams, B_streams, C_streams);
		loader_C_tiles(C_streams, C_TILES);
		storeTilesToDRAM(C_TILES, C_BUF[tileA]);
	}
	storeOutputToDRAM(C_BUF, C_DRAM);
}
