#include "constants.h"
#include "dram.hpp"
#include "load_tiles.hpp"
#include "usefulFunctions.hpp"
#include "loader.hpp"

using namespace std;

void gemm2(const hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> A_DRAM, 
    const hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> B_DRAM, 
    hls::burst_maxi<hls::vector<fm_t, VEC_SIZE>> C_DRAM) {

	#pragma HLS INTERFACE m_axi offset=slave port=A_DRAM bundle=gmem0 depth=I*K/VEC_SIZE max_read_burst_length=VEC_SIZE
	#pragma HLS INTERFACE m_axi offset=slave port=B_DRAM bundle=gmem1 depth=K*J/VEC_SIZE max_read_burst_length=VEC_SIZE
	#pragma HLS INTERFACE m_axi offset=slave port=C_DRAM bundle=gmem2 depth=I*J/VEC_SIZE max_write_burst_length=VEC_SIZE

	fm_t A_BUF[I][K];
	#pragma HLS bind_storage variable=A_BUF type=RAM_1WNR impl=BRAM
	#pragma HLS ARRAY_PARTITION variable=A_BUF dim=1 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=A_BUF dim=2 type=complete
	fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=1 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=2 type=complete
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=3 type=complete
	fm_t C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=1 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=2 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=4 type=complete
	#pragma HLS bind_storage variable=C_BUF type=RAM_1WNR impl=BRAM
	
	loadBFromDRAM(B_DRAM, B_BUF);
	loadAFromDRAM(A_DRAM, A_BUF);
	loop_for_tile_a:
	for(int tileA = 0; tileA < NUM_OF_TILES; tileA++) {
		#pragma HLS DATAFLOW
		#pragma HLS STABLE variable=A_BUF
    	#pragma HLS STABLE variable=B_BUF
		#pragma HLS STABLE variable=C_BUF
		hls::stream<fm_t> A_streams[NUM_OF_TILES];
		#pragma HLS STREAM variable=A_streams type=pipo depth=4
		hls::stream<fm_t> C_streams[NUM_OF_TILES][J/NUM_OF_TILES];
		#pragma HLS STREAM variable=C_streams type=pipo depth=2
		hls::stream<fm_t> B_streams[NUM_OF_TILES][J/NUM_OF_TILES];
		#pragma HLS stream variable=B_streams type=pipo depth=4
		loader_A_tiles(A_BUF, A_streams, tileA);
		loader_B_tiles(B_BUF, B_streams);
		matMulTiles(A_streams, B_streams, C_streams);
		loader_C_tiles(C_streams, C_BUF[tileA]);
	}
	storeOutputToDRAM(C_BUF, C_DRAM);
}
