#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// Delay amount in pixels
#define DELAY 64

#define WIDTH 1280
#define HEIGHT 720


typedef ap_axiu<32,1,1,1> pixel_data;
typedef hls::stream<pixel_data> pixel_stream;

void delay (pixel_stream &src, pixel_stream &dst)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=&src
#pragma HLS INTERFACE axis port=&dst
#pragma HLS PIPELINE II=1

	// Data to be stored across 'function calls'
	static int x = 0;
	static int y = 0;

	pixel_data p_in;
	pixel_data p_out;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	////////////////////////////////

	// Pixel buffer and read/write pointers
	static pixel_data buffer[DELAY];
#pragma HLS dependence variable=buffer intra false
	static int iw = 0;
	static int ir = 0;

	// Uncomment the following lines (and comment the line after that) to delay only the *data*
	//p_out = p_in;
	//p_out.data = buffer[ir].data;
	p_out = buffer[ir];

	buffer[iw] = p_in;


	iw = (iw + 1) % DELAY;
	ir = (ir + 1) % DELAY;

	////////////////////////////////

	// Write pixel to destination
	dst << p_out;

	// Increment X and Y counters
	if (p_in.last)
	{
		x = 0;
		y++;
	}
	else
		x++;
}


void stream (pixel_stream &src, pixel_stream &dst, int frame)
{
	delay(src, dst);
}
