#include "canny.hpp"

void rgb2gray(pixel_stream &src, pixel_stream &dst)
{
#pragma HLS INTERFACE ap_ctrl_none port = return
#pragma HLS INTERFACE axis port = &src
#pragma HLS INTERFACE axis port = &dst
#pragma HLS INTERFACE s_axilite port = mask
#pragma HLS PIPELINE II = 1

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

	// Convert RGB to grayscale
	uint8_t r = rgba2r(p_in.data);
	uint8_t g = rgba2g(p_in.data);
	uint8_t b = rgba2b(p_in.data);
	// Fiirst approach
	// uint8_t gray = (r + g + b) / 3;

	// Second approach
	uint8_t gray = (r * 0.3 + g * 0.59 + b * 0.11);

	// Third approach
	// uint8_t gray = (r / 3 + g / 3 + b / 3);

	// Set output pixel data
	p_out.data = r2rgba(gray) | g2rgba(gray) | b2rgba(gray);

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

// Gaussian blur
void gaussian(pixel_stream &src, pixel_stream &dst)
{
#pragma HLS INTERFACE ap_ctrl_none port = return
#pragma HLS INTERFACE axis port = &src
#pragma HLS INTERFACE axis port = &dst
// #pragma HLS INTERFACE s_axilite port = mask
#pragma HLS PIPELINE II = 1

	// Data to be stored across 'function calls'
	static uint16_t x = 0; // X coordinate --> cols
	static uint16_t y = 0; // Y coordinate

	// Buffer to store the pixel values (to be used in the convolution)
	static uint32_t buffer[GAUSSIAN_MASK_SIZE][WIDTH]; // Gaussian mask

	// Window is used to perform the convolution in parallel
	uint32_t window[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE]; // Window

	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	////////////////////////////////

	// Pixel data to be stored across 'function calls'
	static pixel_data p_out;

	uint8_t pixel = rgba2r(p_in.data);
	// Store pixel value in buffer
	if (x < WIDTH)
		// Store the pixel value in the buffer
		buffer[y % GAUSSIAN_MASK_SIZE][x] = pixel;

	// Check if we have enough data to perform the convolution
	if (y >= GAUSSIAN_MASK_SIZE - 1)
	{

		// Perform convolution
		// Final pixel value
		uint32_t _pixel = 0;

		// Perform the multiplication step
		for (int i = 0; i < GAUSSIAN_MASK_SIZE; i++)
#pragma HLS unroll
			for (int j = 0; j < GAUSSIAN_MASK_SIZE; j++)
#pragma HLS unroll
				window[i][j] = buffer[(y - (GAUSSIAN_MASK_SIZE - 1 - i)) % GAUSSIAN_MASK_SIZE][x - (GAUSSIAN_MASK_SIZE - 1 - j)] * GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE - 1 - i][GAUSSIAN_MASK_SIZE - 1 - j];
		// window[i][j] = buffer[(y - i)) % GAUSSIAN_MASK_SIZE][x - i] * GAUSSIAN_MASK[i][j];

		// Sum all the values in the window
		for (int i = 0; i < GAUSSIAN_MASK_SIZE; i++)
#pragma HLS unroll
			for (int j = 0; j < GAUSSIAN_MASK_SIZE; j++)
#pragma HLS unroll
				_pixel += window[i][j];

		// Sequential implementation
		// for (int i = 0; i < GAUSSIAN_MASK_SIZE; i++)
		// #pragma HLS unroll
		// 	for (int j = 0; j < GAUSSIAN_MASK_SIZE; j++)
		// #pragma HLS unroll
		// 		_pixel += buffer[(y - (GAUSSIAN_MASK_SIZE - 1 - i)) % GAUSSIAN_MASK_SIZE][x - (GAUSSIAN_MASK_SIZE - 1 - j)] * GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE - 1 - i][GAUSSIAN_MASK_SIZE - 1 - j];

		_pixel = GAUSS_NORMALIZE(_pixel);

		// Set output pixel data
		p_out.data = r2rgba(_pixel) | g2rgba(_pixel) | b2rgba(_pixel);

		// First output pixel
		if (x == 0 && y == GAUSSIAN_MASK_SIZE - 1)
			p_out.user = 1;
		else
			p_out.user = 0;

		// Last output pixel
		if (x == WIDTH - 1 && y == HEIGHT - 1)
			p_out.last = 1;
		else
			p_out.last = 0;
	}

	// Store the pixel value in the

	// Write pixel to destination
	dst << p_out;

	////////////////////////////////

	// Increment X and Y counters
	if (p_in.last)
	{
		// Stored a row of pixels
		x = 0;
		y++;
	}
	else
		x++;
}

// Stream function
void stream(pixel_stream &src, pixel_stream &dst, int frame)
{
	// 0. rgb2gray
	pixel_stream gray;
	rgb2gray(src, gray);

	// 1. Gaussian blur
	gaussian(gray, dst);
}
