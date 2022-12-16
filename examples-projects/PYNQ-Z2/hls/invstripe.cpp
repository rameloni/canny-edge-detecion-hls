#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<32,1,1,1> pixel_data;
typedef hls::stream<pixel_data> pixel_stream;

#define WIDTH 1280
#define HEIGHT 720

void invstripe (pixel_stream &src, pixel_stream &dst, uint32_t mask)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=&src
#pragma HLS INTERFACE axis port=&dst
#pragma HLS INTERFACE s_axilite port=mask
#pragma HLS PIPELINE II=1

	// Data to be stored across 'function calls'
	static uint16_t x = 0;
	static uint16_t y = 0;
	static uint32_t d;

	pixel_data p;

	// Load pixel data from source
	src >> p;

	// Reset X and Y counters on user signal
	if (p.user)
		x = y = 0;

	////////////////////////////////

	// Decompose mask input
	bool stripe = (mask&2)>0;
	bool invert = (mask&1)>0;

	// Save every 50th column
	if (x % 50 == 0)
		d = p.data;

	// Apply effects
	if (x < WIDTH/2-50 && stripe)
		p.data = d;
	else if (x > WIDTH/2+50 && invert)
		p.data = ~(p.data & 0x00FFFFFF) | (p.data & 0xFF000000); // invert only the RGB channels

	////////////////////////////////

	// Write pixel to destination
	dst << p;

	// Increment X and Y counters
	if (p.last)
	{
		x = 0;
		y++;
	}
	else
		x++;
}

void stream (pixel_stream &src, pixel_stream &dst, int frame)
{
	invstripe(src, dst, 3);
}
