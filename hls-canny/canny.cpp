#include "canny.hpp"
#include <hls_math.h>
#include <hls_dsp.h>

void rgb2gray(pixel_stream &src, pixel_stream &dst)
{

#pragma HLS PIPELINE II = 1

	// Position counters
	static uint16_t x = 0;
	static uint16_t y = 0;

	// Input pixel data
	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	// End input interface
	////////////////////////////////

	// Output pixel data
	static pixel_data p_out;

	// Convert RGB to grayscale
	// Get RGB values
	uint8_t r = rgba2r(p_in.data);
	uint8_t g = rgba2g(p_in.data);
	uint8_t b = rgba2b(p_in.data);

	// Convert to grayscale
	uint8_t gray = (r + (g << 1) + b) >> 2;

	// Set output pixel data
	p_out.data = r2rgba(gray) | g2rgba(gray) | b2rgba(gray);

	////////////////////////////////
	// Output interface

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
// Interval maximum 1
#pragma HLS PIPELINE II = 1

	// Position counters
	static uint16_t x = 0;
	static uint16_t y = 0;
	static uint8_t cnt = 0; // Counter to store the current line in the line buffer

	// Line buffer to store the input data rows
	static uint8_t buffer[GAUSSIAN_MASK_SIZE][WIDTH];
#pragma HLS ARRAY_RESHAPE variable = buffer factor = 5 dim = 1 cyclic
#pragma HLS dependence variable = buffer false

	// Sliding window to store the input data for the convolution
	static uint8_t window[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE];
#pragma HLS ARRAY_PARTITION variable = window dim = 0 complete

	// Input pixel data
	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	// End input interface
	////////////////////////////////

	// Output pixel data
	static pixel_data p_out;

	// Get pixel value
	uint8_t pixel = rgba2r(p_in.data);

	// Store pixel value in buffer: storing multiple lines
	if (x < WIDTH)
		buffer[cnt][x] = pixel;

	// Slide the window
	for (int i = 0; i < GAUSSIAN_MASK_SIZE; i++)
#pragma HLS unroll
		for (int j = 0; j < GAUSSIAN_MASK_SIZE - 1; j++)
#pragma HLS UNROLL
			window[i][j] = window[i][j + 1];

	// Fill the new column of the window
	for (int i = 0; i < GAUSSIAN_MASK_SIZE; ++i)
	{
#pragma HLS unroll
		uint8_t index = (cnt + i + 1) % GAUSSIAN_MASK_SIZE;
		// Check for removing the dependency
		if (index == cnt)
			window[i][GAUSSIAN_MASK_SIZE - 1] = pixel;
		else
			window[i][GAUSSIAN_MASK_SIZE - 1] = buffer[index][x];
	}

	// Check if there are enough data to perform the convolution
	if (y >= GAUSSIAN_MASK_SIZE - 1)
	{
		// Final pixel value
		uint32_t _pixel = 0;

		// Perform convolution
		for (int i = 0; i < GAUSSIAN_MASK_SIZE; i++)
#pragma HLS PIPELINE
			for (int j = 0; j < GAUSSIAN_MASK_SIZE; j++)
#pragma HLS PIPELINE
				_pixel += window[i][j] * GAUSSIAN_MASK[i][j];

		// Normalize the pixel value
		_pixel = GAUSS_NORMALIZE(_pixel);

		// Set output pixel data
		p_out.data = r2rgba(_pixel) | g2rgba(_pixel) | b2rgba(_pixel);
	}
	else
	{
		// Set output pixel data
		p_out.data = p_in.data;
	}

	////////////////////////////////
	// Output interface

	// Write pixel to destination
	dst << p_out;

	// Copy previous pixel data to next output pixel
	p_out = p_in;

	// Increment X and Y counters
	if (p_in.last)
	{
		x = 0;
		y++;

		// Increment the counter
		cnt = (cnt + 1) % GAUSSIAN_MASK_SIZE;
	}
	else
		x++;
}

// Sobel edge detection
void Sobel(pixel_stream &src, pixel_stream &dst)
{
// Interval maximum 1
#pragma HLS PIPELINE II = 1

	// Position counters
	static uint16_t x = 0;
	static uint16_t y = 0;
	static uint8_t cnt = 0; // Counter to store the current line in the line buffer

	// Line buffer to store the input data rows
	static uint8_t buffer[SOBEL_KERNEL_SIZE][WIDTH]; // Gaussian mask
#pragma HLS ARRAY_PARTITION variable = buffer complete dim = 1
#pragma HLS dependence variable = buffer false

	// Sliding window to store the input data for the convolution
	static uint8_t window[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE]; // Window
#pragma HLS ARRAY_PARTITION variable = window complete dim = 0

	// Input pixel data
	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	// End input interface
	////////////////////////////////

	// Output pixel data
	static pixel_data p_out;

	// Get pixel value
	uint8_t pixel = rgba2r(p_in.data);

	// Store pixel value in buffer: storing multiple lines
	if (x < WIDTH)
		buffer[cnt][x] = pixel;

	// Slide the window
	for (int i = 0; i < SOBEL_KERNEL_SIZE; i++)
#pragma HLS unroll
		for (int j = 0; j < SOBEL_KERNEL_SIZE - 1; j++)
#pragma HLS UNROLL
			window[i][j] = window[i][j + 1];

	// Fill the new column of the window
	for (int i = 0; i < SOBEL_KERNEL_SIZE; ++i)
	{
#pragma HLS unroll
		uint8_t index = (cnt + i + 1) % SOBEL_KERNEL_SIZE;
		// Check for removing the dependency
		if (index == cnt)
			window[i][SOBEL_KERNEL_SIZE - 1] = pixel;
		else
			window[i][SOBEL_KERNEL_SIZE - 1] = buffer[index][x];
	}

	// Check if there are enough data to perform the convolution
	if (y >= SOBEL_KERNEL_SIZE - 1)
	{
		// Final pixel value
		int32_t h_pixel = 0;
		int32_t v_pixel = 0;

		uint32_t _pixel = 0;
		uint32_t grad_dir = 0;

		// Perform convolution
		for (int i = 0; i < SOBEL_KERNEL_SIZE; i++)
#pragma HLS unroll
			for (int j = 0; j < SOBEL_KERNEL_SIZE; j++)
			{
#pragma HLS unroll
				h_pixel += window[i][j] * H_SOBEL_KERNEL[i][j];
				v_pixel += window[i][j] * V_SOBEL_KERNEL[i][j];
			}

		// Most precise: uses square root
		// 		_pixel = hls::sqrt(h_pixel * h_pixel + v_pixel * v_pixel);

		// Optimized: uses approximations
		_pixel = hls::abs(h_pixel) + hls::abs(v_pixel);

		// Clamp the pixel value
		if (_pixel > 255)
			_pixel = 255;

		// Compute the gradient direction
		//		float dir_rad = hls::atan2(v_pixel, h_pixel);
		// Tangent approximation
		if (h_pixel == 0)
			grad_dir = 2;
		else
		{
			// Approximation
			int16_t dir_angle = (v_pixel << 8) / h_pixel;

			// Direction: 0: (0 || 180), 1: (45 || -45), 2: (90 || -90), 3: (135 || -135)
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

		// Set output pixel data
		p_out.data = r2rgba(_pixel) | g2rgba(_pixel) | b2rgba(_pixel) | a2rgba(grad_dir);
	}
	else
	{
		// Set output pixel data
		p_out.data = p_in.data;
	}

	////////////////////////////////
	// Output interface

	// Write pixel to destination
	dst << p_out;

	// Copy previous pixel data to next output pixel
	p_out = p_in;

	// Increment X and Y counters
	if (p_in.last)
	{
		// Stored a row of pixels
		x = 0;
		y++;

		// Increment the counter index
		cnt = (cnt + 1) % SOBEL_KERNEL_SIZE;
	}
	else
		x++;
}

// Double thresholding
void double_threshold(pixel_stream &src, pixel_stream &dst)
{
// Interval maximum 1
#pragma HLS PIPELINE II = 1

	// Position counters
	static uint16_t x = 0;
	static uint16_t y = 0;

	// Input pixel data
	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	// End input interface
	////////////////////////////////

	// Output pixel data
	static pixel_data p_out;

	// Get pixel value
	uint8_t pixel = rgba2r(p_in.data);

	// Check for threshold
	if (pixel >= HIGH_THRESHOLD)
	{
		// Set output pixel data
		p_out.data = r2rgba(STRONG_EDGE) | g2rgba(STRONG_EDGE) | b2rgba(STRONG_EDGE);
	}
	else if (pixel > LOW_THRESHOLD)
	{
		// Set output pixel data
		p_out.data = r2rgba(WEAK_EDGE) | g2rgba(WEAK_EDGE) | b2rgba(WEAK_EDGE);
	}
	else
	{
		// Set output pixel data
		p_out.data = r2rgba(NO_EDGE) | g2rgba(NO_EDGE) | b2rgba(NO_EDGE);
	}

	////////////////////////////////
	// Output interface

	// Write pixel to destination
	dst << p_out;

	// Copy previous pixel data to next output pixel
	p_out = p_in;

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

// Non-maximum suppression
void non_max_sup(pixel_stream &src, pixel_stream &dst)
{
// Interval maximum 1
#pragma HLS PIPELINE II = 1

	// Position counters
	static uint16_t x = 0;
	static uint16_t y = 0;
	static uint8_t cnt = 0; // Counter to store the current line in the line buffer

	// Line buffer to store the input data rows
	static uint32_t buffer[3][WIDTH];
#pragma HLS ARRAY_RESHAPE variable = buffer factor = 3 dim = 1 cyclic
#pragma HLS dependence variable = buffer false

	// Line buffer to store the input data rows (Gradient direction)
	static uint8_t buffer_grad[3][WIDTH];
#pragma HLS ARRAY_PARTITION variable = buffer_grad complete dim = 1
#pragma HLS dependence variable = buffer_grad false

	// Sliding window to store the input data for the convolution
	static uint32_t window[3][3];
#pragma HLS ARRAY_PARTITION variable = window complete dim = 0

	// Sliding window to store the input data for the convolution (Gradient direction)
	static uint8_t window_grad[3][3];
#pragma HLS ARRAY_PARTITION variable = window_grad complete dim = 0

	// Input pixel data
	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	if (p_in.user)
		x = y = 0;

	// End input interface
	////////////////////////////////

	// Output pixel data
	static pixel_data p_out;

	// Get pixel value
	uint8_t pixel = rgba2r(p_in.data);
	// Get gradient direction
	uint8_t grad_dir = rgba2a(p_in.data);

	// Store pixel value in buffer: storing multiple lines
	if (x < WIDTH)
	{
		buffer[cnt][x] = pixel;
		buffer_grad[cnt][x] = grad_dir;
	}

	// Slide the windows
	for (int i = 0; i < 3; i++)
#pragma HLS unroll
		for (int j = 0; j < 2; j++)
		{
#pragma HLS UNROLL
			window[i][j] = window[i][j + 1];
			window_grad[i][j] = window_grad[i][j + 1];
		}

	// Fill the new column of the window: separate for loops to allow HLS to optimize
	for (int i = 0; i < 3; ++i)
	{
#pragma HLS unroll
		uint8_t index = (cnt + i + 1) % 3;
		if (index == cnt)
			window_grad[i][2] = grad_dir;
		else
			window_grad[i][2] = buffer_grad[index][x];
	} // end for

	// Fill the new column of the window (Gradient direction)
	for (int i = 0; i < 3; ++i)
	{
#pragma HLS unroll
		uint8_t index = (cnt + i + 1) % 3;
		if (index == cnt)
			window[i][2] = pixel;
		else
			window[i][2] = buffer[index][x];
	} // end for

	// Check if there are enough data to perform the convolution
	if (y >= 2)
	{

		// Value to compare with the center pixel of the window and the reference pixel
		uint8_t ref1, ref2, _pixel;

		// Get the gradient direction of center pixel of the window
		uint8_t _grad_dir = window_grad[1][1];
		// #pragma HLS dependence variable = window_grad false

		// (0 or 180 degrees)
		if (_grad_dir == 0)
		{
			ref1 = window[1][0]; // [i][j-1] - right center
			ref2 = window[1][2]; // [i][j-1] - right center
		}
		else if (_grad_dir == 3)
		{
			// 135
			ref1 = window[0][0];
			ref2 = window[2][2];
		}
		else if (_grad_dir == 2)
		{
			// 90
			ref1 = window[0][1];
			ref2 = window[2][1];
		}
		else
		{
			// 45
			ref1 = window[0][2];
			ref2 = window[2][0];
		}

		if (window[1][1] < ref1 || window[1][1] < ref2)
			_pixel = 0;
		else
			_pixel = window[1][1];

		// Set output pixel data
		p_out.data = r2rgba(_pixel) | b2rgba(_pixel) | g2rgba(_pixel);
	}
	else
	{
		// Set output pixel data
		p_out.data = p_in.data;
	}

	////////////////////////////////
	// Output interface

	// Write pixel to destination
	dst << p_out;

	p_out = p_in;

	p_out.last = 0;

	// Increment X and Y counters
	if (p_in.last)
	{
		p_out.last = 1;
		// Stored a row of pixels
		x = 0;
		y++;

		// Increment the buffer counter
		cnt = (cnt + 1) % 3;
	}
	else
		x++;
}

// Edge tracking
void edge_tracking(pixel_stream &src, pixel_stream &dst)
{
// Interval maximum 1
#pragma HLS PIPELINE II = 1

	// Position counters
	static uint16_t x = 0;
	static uint16_t y = 0;
	static uint8_t cnt = 0; // Counter to store the current line in the line buffer

	// Line buffer to store the input data rows
	static uint8_t buffer[SOBEL_KERNEL_SIZE][WIDTH]; // Gaussian mask
#pragma HLS ARRAY_PARTITION variable = buffer complete dim = 1
#pragma HLS dependence variable = buffer false

	// Sliding window to store the input data for the convolution
	static uint8_t window[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE]; // Window
#pragma HLS ARRAY_PARTITION variable = h_window complete dim = 0

	// Input pixel data
	pixel_data p_in;

	// Load input data from source
	src >> p_in;

	// Reset X and Y counters on user signal
	if (p_in.user)
		x = y = 0;

	// End input interface
	////////////////////////////////

	// Output pixel data
	static pixel_data p_out;

	// Get pixel value
	uint8_t pixel = rgba2r(p_in.data);

	// Store pixel value in buffer: storing multiple lines
	if (x < WIDTH)
		buffer[cnt][x] = pixel;

	// Slide the window
	for (int i = 0; i < SOBEL_KERNEL_SIZE; i++)
#pragma HLS unroll
		for (int j = 0; j < SOBEL_KERNEL_SIZE - 1; j++)
#pragma HLS UNROLL
			window[i][j] = window[i][j + 1];

	// Fill the new column of the window
	for (int i = 0; i < SOBEL_KERNEL_SIZE; ++i)
	{
#pragma HLS unroll
		uint8_t index = (cnt + i + 1) % SOBEL_KERNEL_SIZE;
		if (index == cnt)
			window[i][SOBEL_KERNEL_SIZE - 1] = pixel;
		else
			window[i][SOBEL_KERNEL_SIZE - 1] = buffer[index][x];
	}

	// Set strong edge
	const uint32_t strong_edge = r2rgba(STRONG_EDGE) | g2rgba(STRONG_EDGE) | b2rgba(STRONG_EDGE);

	// Check if there are enough data to perform the convolution
	if (y >= 2)
	{
		// Check if the pixel is a strong edge
		bool strong_pixel_close = false;
		// Check if the pixel is not an edge
		bool pixel_no_edge = false;

		for (int i = 0; i < 3; i++)
		{
#pragma HLS unroll
			for (int j = 0; j < 3; j++)
			{
#pragma HLS unroll
				strong_pixel_close |= window[i][j];
				if (i == 1 && j == 1)
					pixel_no_edge = !((window[i][j] == WEAK_EDGE) | (window[i][j] == STRONG_EDGE));
			}
		}

		// Check if the pixel is an edge
		if (!pixel_no_edge)
		{
			// Check if the pixel is a strong edge
			if (strong_pixel_close)
				p_out.data = strong_edge;
			else
				p_out.data = NO_EDGE;
		}
		else
		{
			p_out.data = NO_EDGE;
		}
	}
	else
	{
		// Set output pixel data
		p_out.data = NO_EDGE;
	}

	////////////////////////////////
	// Output interface

	// Write pixel to destination
	dst << p_out;

	// Copy previous pixel data to next output pixel
	p_out = p_in;

	// Increment X and Y counters
	if (p_in.last)
	{
		// Stored a row of pixels
		x = 0;
		y++;

		// Increment the buffer counter
		cnt = (cnt + 1) % SOBEL_KERNEL_SIZE;
	}
	else
		x++;
}

// Stream function: main function
void stream(pixel_stream &src, pixel_stream &dst, int frame)
{
	// Dataflow
#pragma HLS DATAFLOW disable_start_propagation

// Input/Output interfaces
#pragma HLS INTERFACE ap_ctrl_none port = return
#pragma HLS INTERFACE axis port = &src
#pragma HLS INTERFACE axis port = &dst

	// FIFOs
	static pixel_stream gray, sobel, gauss, n_max, db_thresh;

	// FIFOs depth
#pragma HLS STREAM variable = gray depth = 3
#pragma HLS STREAM variable = gauss depth = 3
#pragma HLS STREAM variable = sobel depth = 3
#pragma HLS STREAM variable = n_max depth = 3
#pragma HLS STREAM variable = db_thresh depth = 3

	// Process pixel

	// 0. rgb2gray
	rgb2gray(src, gray);

	// 1. Gaussian blur
	gaussian(gray, gauss);

	// 2. Sobel
	Sobel(gauss, sobel);

	// 3. Non-maximum suppression
	non_max_sup(sobel, n_max);

	// 4. Double threshold
	double_threshold(n_max, db_thresh);

	// 5. Edge tracking
	edge_tracking(db_thresh, dst);
}
