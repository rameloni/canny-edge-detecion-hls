// Streamulator test platform

#include "canny_streamemulator.h"


/* Load image from file into pixel stream
 *
 * filename - path to input image
 * stream   - output pixel stream
 * rep      - number of times to repeat the image
 */
void loadFrame (const std::string &filename, pixel_stream &stream, int rep)
{
	cv::Mat srcImg;

	// Read input image
	srcImg = cv::imread(filename);
	if (srcImg.data == NULL)
	{
		std::cout << "##### Invalid input image: " << filename << std::endl;
		throw;
	}

	// A necessary conversion to obtain the right format
	cv::cvtColor(srcImg, srcImg, CV_BGR2RGBA);

	// Write input data
	for (int i=0; i<rep; i++)
		cvMat2AXIvideo(srcImg, stream);
}


/* Generate frame with a single solid color
 * color  - color in BGR format
 * stream - output pixel stream
 * rep    - number of times to repeat the image
 */
void fillFrame (int color, pixel_stream &stream, int rep)
{
	pixel_data pixel;

	for (int row=0; row<HEIGHT; row++)
		for (int col=0; col<WIDTH; col++)
		{
			if (row == 0 && col == 0)
				pixel.user = 1;
			else
				pixel.user = 0;

			if (col == WIDTH-1)
				pixel.last = 1;
			else
				pixel.last = 0;

			pixel.data = color;
			stream << pixel;
		}
}


/* Process image stream
 *
 * src - source (input) stream
 * dst - destination (output) stream
 */
void processStream (pixel_stream &src, pixel_stream &dst)
{
	static int pixel = 0;
	static int frame = 0;

	// Call stream processing function
	while (!src.empty())
	{
		stream(src, dst, frame);

		pixel++;
		if (pixel >= WIDTH*HEIGHT)
		{
			pixel = 0;
			frame++;
			//std::cout << "Frame: " << frame << std::endl; // uncomment to see frame progress
		}
	}
}


/* Save raw pixel stream to file
 *
 * src      - input pixel stream
 * filename - path to output image
 */
void saveRawStream (pixel_stream &src, const std::string &filename)
{
	std::vector<ap_uint<32>> pixeldata;
	pixel_data pixel;

	// Copy source data
	while(!src.empty())
	{
		src >> pixel;
		pixeldata.push_back(pixel.data | 0xFF000000);
	}

	// Compute height
	int height = pixeldata.size() / WIDTH;

	cv::Mat dstImg(height, WIDTH, CV_8UC4, pixeldata.data());
	cv::cvtColor(dstImg, dstImg, CV_RGBA2BGR);
	cv::imwrite(filename, dstImg);
}


/* Synchronize video stream
 *
 * src - input pixel stream
 * dst - output pixel stream
 *
 * returns number of pixels before sync signal (ie processing delay)
 */
int syncStream (pixel_stream &src, pixel_stream &dst)
{
	pixel_data pixel;

	// Can't sync empty stream
	if (src.empty())
		return -1;

	// Find start of frame
	int i = -1;
	do
	{
		src >> pixel;
		i++;

		if (src.empty())
		{
			std::cout << "##### Unable to sync frame, maybe missing a pixel.user assignment?" << std::endl;
			throw;
		}
	} while (pixel.user != 1);

	// Copy rest of stream
	dst << pixel;
	while (!src.empty())
	{
		src >> pixel;
		dst << pixel;
	}

	return i;
}


/* Save a single frame from pixel stream
 *
 * src        - input pixel stream
 * filename   - path to output image
 */
void saveFrame (pixel_stream &src, const std::string &filename)
{
	std::vector<ap_uint<32>> pixeldata;
	pixel_data pixel;

	// Read frame, keep track of lines missing pixel.last signal
	int missing_last = 0;
	int rows;

	for (rows=0; rows<HEIGHT; rows++)
		for (int cols=0; cols<WIDTH; cols++)
		{
			if (!src.empty()) // don't read from empty stream
				src >> pixel;
			else
				goto endloop;

			pixeldata.push_back(pixel.data | 0xFF000000); // OR with full alpha channel

			if (cols == WIDTH-1 && pixel.last != 1) // check pixel.last
				missing_last++;


		}
endloop:

	if (missing_last > 0)
		std::cout << "Number of lines missing pixel.last signal: " << missing_last << std::endl;

	// Save image by converting data array to matrix
	if (!filename.empty())
	{
		cv::Mat saveImg(HEIGHT, WIDTH, CV_8UC4, pixeldata.data()); //replace HEIGHT by rows
		cv::cvtColor(saveImg, saveImg, CV_RGBA2BGR);
		cv::imwrite(filename, saveImg);
	}
}


int main ()
{
	pixel_stream srcStream, procStream, syncedStream;

	std::ifstream frames_list(FRAMES_LIST);
	std::string filename;
	int reps, delay = -1, frame = 0;

	while (frames_list >> filename && frames_list >> reps)
	{
		std::cout << filename << " " << reps << std::endl;
		if (filename == "-")
			fillFrame(0, srcStream, reps);
		else
			loadFrame(INPUT_DIR + filename, srcStream, reps);

		processStream(srcStream, procStream);

#ifndef RAW_FILENAME
		delay = syncStream(procStream, syncedStream);

		for (int i=0; i<reps; i++)
		{
			std::ostringstream os;
			os << OUTPUT_DIR;
			os << std::setfill('0') << std::setw(4) << frame;
			os << "_" << filename << ".png";

			if (filename == "-")
				saveFrame(syncedStream, "");
			else
				saveFrame(syncedStream, os.str());
			frame++;
		}
#endif
	}
#ifdef RAW_FILENAME
	saveRawStream(procStream, RAW_FILENAME);
#endif

	std::cout << "HLS delay: " << delay << " pixels"
			  << " (" << (float)delay/WIDTH << " lines)" << std::endl;

	return 0;
}

