#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define WIDTH 1280
#define HEIGHT 720

#define GR(v) ((v)&0xFF)
#define GG(v) (((v)&0xFF00)>>8)
#define GB(v) (((v)&0xFF0000)>>16)
#define SR(v) ((v)&0xFF)
#define SG(v) (((v)&0xFF)<<8)
#define SB(v) (((v)&0xFF)<<16)

typedef ap_axiu<32,1,1,1> pixel_data;
typedef hls::stream<pixel_data> pixel_stream;

void hfilt (pixel_stream &src, pixel_stream &dst, uint8_t l, uint8_t c, uint8_t r)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=&src
#pragma HLS INTERFACE axis port=&dst
#pragma HLS INTERFACE s_axilite port=l
#pragma HLS INTERFACE s_axilite port=c
#pragma HLS INTERFACE s_axilite port=r
#pragma HLS PIPELINE II=1

	// Data to be stored across 'function calls'
	static uint16_t x = 0;
	static uint16_t y = 0;

	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	////////////////////////////////

	// Pixel data to be stored across 'function calls'
	static pixel_data p_out;
	static uint32_t dl = 0;
	static uint32_t dc = 0;

	// Current (incoming) pixel data
	uint32_t dr = p_in.data;

	// Compute outgoing pixel data
	uint32_t dn =
			SR((GR(dl)*l+GR(dc)*c+GR(dr)*r)>>8) +
			SG((GG(dl)*l+GG(dc)*c+GG(dr)*r)>>8) +
			SB((GB(dl)*l+GB(dc)*c+GB(dr)*r)>>8);

	// Move one pixel to the right
	dl = dc;
	dc = dr;

	p_out.data = dn;

	////////////////////////////////

	// Write pixel to destination
	dst << p_out;

	// Copy previous pixel data to next output pixel
	p_out = p_in;

	// Increment X and Y counters
	if (p_in.last)
	{
		x = 0;
		y++;
	}
	else
		x++;
}
//
//void stream (pixel_stream &src, pixel_stream &dst, int frame)
//{
//	hfilt(src, dst, 50, 150, 50);
//}
