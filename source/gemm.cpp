#include "constants.h"
#include "dram.hpp"
#include "usefulFunctions.hpp"
#include "loader.hpp"

using namespace std;

// The top function for GeMM
void gemm2(const hls::burst_maxi<hls::vector<float, VEC_SIZE>> A_DRAM, 
    const hls::burst_maxi<hls::vector<float, VEC_SIZE>> B_DRAM, 
    hls::burst_maxi<hls::vector<float, VEC_SIZE>> C_DRAM) {

	// HLS INTERFACE pragmas for AXI4 master interfaces
	#pragma HLS INTERFACE m_axi offset=slave port=A_DRAM bundle=gmem0 depth=I*K/VEC_SIZE max_read_burst_length=VEC_SIZE
	#pragma HLS INTERFACE m_axi offset=slave port=B_DRAM bundle=gmem1 depth=K*J/VEC_SIZE max_read_burst_length=VEC_SIZE
	#pragma HLS INTERFACE m_axi offset=slave port=C_DRAM bundle=gmem2 depth=I*J/VEC_SIZE max_write_burst_length=VEC_SIZE

	// Buffer Declerations with partitions and storage binding
	float A_BUF[I][K];
	#pragma HLS bind_storage variable=A_BUF type=RAM_1WNR impl=BRAM
	#pragma HLS ARRAY_PARTITION variable=A_BUF dim=1 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=A_BUF dim=2 type=complete
	float B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=1 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=2 type=complete
	#pragma HLS ARRAY_PARTITION variable=B_BUF dim=3 type=complete
	float C_BUF[NUM_OF_TILES][NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES];
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=1 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=2 type=block factor=NUM_OF_TILES
	#pragma HLS ARRAY_PARTITION variable=C_BUF dim=4 type=complete
	#pragma HLS bind_storage variable=C_BUF type=RAM_1WNR impl=BRAM
	
	// Loading A and B from DRAM to buffers
	loadBFromDRAM(B_DRAM, B_BUF);
	loadAFromDRAM(A_DRAM, A_BUF);

	// The loop that calculates GeMM
	loop_for_tile_a:
	for(int tileA = 0; tileA < NUM_OF_TILES; tileA++) {
		#pragma HLS DATAFLOW // Pragma to enable concurrent execution of functions, boosting performance

		// Pragmas to indicate that the buffers are stable and will not change during the execution of the loop
		#pragma HLS STABLE variable=A_BUF
    	#pragma HLS STABLE variable=B_BUF
		#pragma HLS STABLE variable=C_BUF
		
		// Definition of streams for A, B and C tiles
		hls::stream<float> A_streams[NUM_OF_TILES];
		#pragma HLS STREAM variable=A_streams type=pipo depth=4
		hls::stream<float> C_streams[NUM_OF_TILES][J/NUM_OF_TILES];
		#pragma HLS STREAM variable=C_streams type=pipo depth=2
		hls::stream<float> B_streams[NUM_OF_TILES][J/NUM_OF_TILES];
		#pragma HLS stream variable=B_streams type=pipo depth=4

		// Load A and B from buffers to streams
		loader_A_tiles(A_BUF, A_streams, tileA);
		loader_B_tiles(B_BUF, B_streams);

		// Perform MatMul
		matMulTiles(A_streams, B_streams, C_streams);

		// Load C from its stream to its Buffer
		loader_C_tiles(C_streams, C_BUF[tileA]);
	}

	// Store C's values from its buffer to DRAM
	storeOutputToDRAM(C_BUF, C_DRAM);
}
