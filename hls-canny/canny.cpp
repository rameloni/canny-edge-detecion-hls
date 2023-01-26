#include "canny.hpp"
#include <hls_math.h>
#include <hls_dsp.h>
void rgb2gray(pixel_stream &src, pixel_stream &dst)
{
// #pragma HLS INTERFACE ap_ctrl_none port = return
// #pragma HLS INTERFACE axis port = &src
// #pragma HLS INTERFACE axis port = &dst
// #pragma HLS INTERFACE s_axilite port = mask
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
	// First approach
//	 uint8_t gray = (r + g + b) / 3;

	// Second approach
//	uint8_t gray = (r * 0.3 + g * 0.59 + b * 0.11);

	// Third approach
	// uint8_t gray = (r / 3 + g / 3 + b / 3);

	//Fourth approach
//	 uint16_t tmp = r+g+b;
//	 uint8_t gray = (tmp - (tmp >> 2))>> 2;

	//Fifth approach
	uint8_t gray = (r + (g<<1) + b) >> 2;

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
// #pragma HLS INTERFACE ap_ctrl_none port = return
// #pragma HLS INTERFACE axis port = &src
// #pragma HLS INTERFACE axis port = &dst
//  #pragma HLS INTERFACE s_axilite port = mask
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
	}

	// Store the pixel value in the

	// Write pixel to destination
	dst << p_out;

	// Need to change this
	p_out = p_in;
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

void Sobel(pixel_stream &src, pixel_stream &dst, ap_uint<2> &grad_dir)
{
// #pragma HLS INTERFACE ap_ctrl_none port = return
// #pragma HLS INTERFACE axis port = &src
// #pragma HLS INTERFACE axis port = &dst
//  #pragma HLS INTERFACE s_axilite port = mask
#pragma HLS PIPELINE II = 1
	//
	// Data to be stored across 'function calls'
	static uint16_t x = 0; // X coordinate --> cols
	static uint16_t y = 0; // Y coordinate
						   //
	// Buffer to store the pixel values (to be used in the convolution)
	static uint32_t buffer[SOBEL_KERNEL_SIZE][WIDTH]; // Gaussian mask

	// Window is used to perform the convolution in parallel
	uint32_t h_window[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE]; // Window
	uint32_t v_window[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE];
	//
	pixel_data p_in;
	//
	//		// Load input data from source
	src >> p_in;
	//
	//		// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	////////////////////////////////

	// Pixel data to be stored across 'function calls'
	static pixel_data p_out;

	grad_dir = 0;

	uint8_t pixel = rgba2r(p_in.data);
	//		// Store pixel value in buffer
	if (x < WIDTH)
		// Store the pixel value in the buffer
		buffer[y % SOBEL_KERNEL_SIZE][x] = pixel;

	// Check if we have enough data to perform the convolution
	if (y >= SOBEL_KERNEL_SIZE - 1)
	{

		// Perform convolution
		// Final pixel value
		int32_t h_pixel = 0;
		int32_t v_pixel = 0;
		uint32_t _pixel = 0;
		// Perform the multiplication step
		for (int i = 0; i < SOBEL_KERNEL_SIZE; i++)
		{
#pragma HLS unroll
			for (int j = 0; j < SOBEL_KERNEL_SIZE; j++)
			{
#pragma HLS unroll
				h_window[i][j] = buffer[(y - (SOBEL_KERNEL_SIZE - 1 - i)) % SOBEL_KERNEL_SIZE][x - (SOBEL_KERNEL_SIZE - 1 - j)] * H_SOBEL_KERNEL[SOBEL_KERNEL_SIZE - 1 - i][SOBEL_KERNEL_SIZE - 1 - j];
				v_window[i][j] = buffer[(y - (SOBEL_KERNEL_SIZE - 1 - i)) % SOBEL_KERNEL_SIZE][x - (SOBEL_KERNEL_SIZE - 1 - j)] * V_SOBEL_KERNEL[SOBEL_KERNEL_SIZE - 1 - i][SOBEL_KERNEL_SIZE - 1 - j];
			}
		}

		// Sum all the values in the window
		for (int i = 0; i < SOBEL_KERNEL_SIZE; i++)
		{
#pragma HLS unroll
			for (int j = 0; j < SOBEL_KERNEL_SIZE; j++)
			{
#pragma HLS unroll
				h_pixel += h_window[i][j];
				v_pixel += v_window[i][j];
			}
		}

		// Correct
		// _pixel = hls::sqrt(h_pixel * h_pixel + v_pixel * v_pixel);
		// Optimized: uses approximations
		_pixel = hls::abs(h_pixel) + hls::abs(v_pixel);

		if (_pixel > 255)
		{
			_pixel = 255;
		}

		// Set output pixel data
		p_out.data = r2rgba(_pixel) | g2rgba(_pixel) | b2rgba(_pixel);

		// Compute the gradient direction
//		float dir_rad = hls::atan2(v_pixel, h_pixel);
		// Tangent approximation
		        if (h_pixel == 0)
		            grad_dir = 2;
		        else
		        {
		        int32_t dir_angle = (v_pixel << 8) / h_pixel;

		        // Direction: 0: (0° || 180°), 1: (45° || -45°), 2: (90° || -90°), 3: (135 || -135°)

		        if (dir_angle > -618 && dir_angle <= -106)
		            // for angles between 112.5 and 157.5 (tan(112.5) = -2.4142, tan(157.5) = -0.4142)
		            grad_dir = 3;
		        else if (dir_angle > -106 && dir_angle <= 106)
		            // Four possible directions: 0, 45, 90, 135 for anglse between 0 and 360
		            // for angles between -22.5 and 22.5 (tan(-22.5) = -0.4142, tan(22.5) = 0.4142) and between 157.5 and 180 (tan(157.5) = 2.4142, tan(180) = 0)
		            grad_dir = 0;
		        else if (dir_angle > 106 && dir_angle <= 618)
		            // for angles between 22.5 and 67.5 (tan(22.5) = 0.4142, tan(67.5) = 2.4142)
		            // dir = 45
		            grad_dir = 1;
		        else
		            // for angles between 67.5 and 112.5 (tan(67.5) = 2.4142, tan(112.5) = -2.4142)
		            // dir = 90
		            grad_dir = 2;
		        }

	}
	// Store the pixel value in the

	// Write pixel to destination
	dst << p_out;

	// Need to change this
	p_out = p_in;
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

void non_max_sup(pixel_stream &src, pixel_stream &dst, ap_uint<2> &grad_dir){
#pragma HLS PIPELINE II = 1
	//
	// Data to be stored across 'function calls'
	static uint16_t x = 0; // X coordinate --> cols
	static uint16_t y = 0; // Y coordinate


	pixel_data p_in;

	// Load input data from source
	src >> p_in;
	static pixel_data p_out;

	// Buffer to store the pixel values (to be used in the convolution)
	static uint32_t buffer[SOBEL_KERNEL_SIZE][WIDTH][2];
	uint32_t window[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE][2];

	if (p_in.user){
		x = y = 0;
	}
	uint8_t pixel = rgba2r(p_in.data);
	for(int i = 0; i < SOBEL_KERNEL_SIZE-1; i++){
//		Using a FIFO to store the line data
		buffer[i][x][0] = buffer[i+1][x][0];
		buffer[i][x][1] = buffer[i+1][x][1];
	}
	if (x < WIDTH){
//	write the new pixel to the line buffer
		buffer[SOBEL_KERNEL_SIZE-1][x][0] = pixel;
		buffer[SOBEL_KERNEL_SIZE-1][x][1] = grad_dir;
	}

	uint32_t _pixel = 0;

	if (y >= SOBEL_KERNEL_SIZE - 1){
		//	Using a FIFO to be the window buffer

		for(int i = 0; i< SOBEL_KERNEL_SIZE; i++){
			for (int j = 0; j < SOBEL_KERNEL_SIZE-1; j++){
			window[i][j][0] = window[i][j+1][0];
			window[i][j][1] = window[i][j+1][1];
			}
		}


	//	writing the new pixel to the window buffer from line buffer
		for(int i = 0; i < SOBEL_KERNEL_SIZE; i++){
			window[i][SOBEL_KERNEL_SIZE-1][0] = buffer[i][x][0];
			window[i][SOBEL_KERNEL_SIZE-1][1] = buffer[i][x][1];
		}

// starting non maximum supression
		uint32_t grad_current, value_current;
		grad_current = window[1][1][1];
		value_current = window[1][1][0];

		if (grad_current == 0){
			if (value_current < window[1][0][0] || value_current < window[1][2][0]){
				value_current = 0;
			}
		}
		else if(grad_current == 1){
			if (value_current < window[0][0][0] || value_current < window[2][2][0]){
				value_current = 0;
			}
		}
		else if(grad_current == 2){
			if (value_current < window[0][1][0] || value_current < window[2][1][0]){
				value_current = 0;
			}
		}
		else if(grad_current == 3){
			if (value_current < window[2][0][0] || value_current < window[0][2][0]){
				value_current = 0;
			}
		}

		_pixel = value_current;
	}

	// outputing

	_pixel = p_out.data;

	dst << p_out;

	// Need to change this
	p_out = p_in;
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
pixel_stream gray, sobel, gauss;
ap_uint<2> grad_dir = 0;
void stream(pixel_stream &src, pixel_stream &dst, int frame)
{
#pragma HLS STREAM variable = gray depth = 1 dim = 1
#pragma HLS STREAM variable = gauss depth = 1 dim = 1
#pragma HLS STREAM variable = sobel depth = 1 dim = 1
	// 0. rgb2gray

	rgb2gray(src, gray);

	// 1. Gaussian blur
	gaussian(gray, gauss);

	Sobel(gauss, sobel, grad_dir);
//
	non_max_sup(sobel, dst, grad_dir);
}
