#include "constants.h"

void loader_A_tiles(const fm_t A_BUF[I][K], hls::stream<fm_t> A_streams[NUM_OF_TILES], const int tileA){
    #pragma HLS INLINE off
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k++) {
            #pragma HLS PIPELINE II=1
            const fm_t temp = A_BUF[i + tileA*(I/NUM_OF_TILES)][k];
            for(int t=0; t<NUM_OF_TILES; t++){
                #pragma HLS UNROLL
                A_streams[t].write(temp);
            }
		}
	}
}
 
void loader_B_tile(const fm_t B_TILE[K][J/NUM_OF_TILES], hls::stream<fm_t> B_streams[J/NUM_OF_TILES]){
    #pragma HLS INLINE off
	for(int i = 0; i < I/NUM_OF_TILES; i++) {
		for(int k = 0; k < K; k++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS LOOP_FLATTEN
			for(int j = 0; j < J/NUM_OF_TILES; j++) {
                #pragma HLS UNROLL
                B_streams[j].write(B_TILE[k][j]);
			}
		}
	}
}
 
void loader_C_tile(hls::stream<fm_t> C_streams[J/NUM_OF_TILES], fm_t C[I/NUM_OF_TILES][J/NUM_OF_TILES]){
    #pragma HLS INLINE off
    for(int i = 0; i < I/NUM_OF_TILES; i++) {
        # pragma HLS PIPELINE II=1
		for(int j = 0; j < J/NUM_OF_TILES; j++) {
            #pragma HLS UNROLL
            C[i][j] = C_streams[j].read();
		}
	}
}
 
void loader_B_tiles(const fm_t B_BUF[NUM_OF_TILES][K][J/NUM_OF_TILES], hls::stream<fm_t> B_streams[NUM_OF_TILES][J/NUM_OF_TILES]){
    #pragma HLS INLINE off
    #pragma HLS DATAFLOW
    for(int t=0; t<NUM_OF_TILES; t++){
        #pragma HLS UNROLL
        loader_B_tile(B_BUF[t], B_streams[t]);
    }
}
 
void loader_C_tiles(hls::stream<fm_t> C_streams[NUM_OF_TILES][J/NUM_OF_TILES], fm_t C_TILES[NUM_OF_TILES][I/NUM_OF_TILES][J/NUM_OF_TILES]){
    #pragma HLS INLINE off
    #pragma HLS DATAFLOW
    for(int t=0; t<NUM_OF_TILES; t++){
        #pragma HLS UNROLL
        loader_C_tile(C_streams[t], C_TILES[t]);
    }
}