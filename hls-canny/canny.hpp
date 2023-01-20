#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<32, 1, 1, 1> pixel_data;
typedef hls::stream<pixel_data> pixel_stream;

#define rgba2r(v) ((v)&0xFF)
#define rgba2g(v) (((v)&0xFF00) >> 8)
#define rgba2b(v) (((v)&0xFF0000) >> 16)
#define r2rgba(v) ((v)&0xFF)
#define g2rgba(v) (((v)&0xFF) << 8)
#define b2rgba(v) (((v)&0xFF) << 16)

#define WIDTH 1280
#define HEIGHT 720

#define GAUSSIAN_MASK_SIZE 5

#define GAUSS_APPROX 1

// Approximations source: https://www.researchgate.net/publication/325768087_Gaussian_filtering_for_FPGA_based_image_processing_with_High-Level_Synthesis_tools
#if GAUSS_APPROX == 0
#define GAUSS_NORMALIZE(pixel) ((pixel) / 273)
#define GAUSSIAN_MASK_SUM 273
const uint32_t GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE] = {{1, 4, 7, 4, 1},
                                                                        {4, 16, 26, 16, 4},
                                                                        {7, 26, 41, 26, 7},
                                                                        {4, 16, 26, 16, 4},
                                                                        {1, 4, 7, 4, 1}};
#elif GAUSS_APPROX == 1
#define GAUSS_NORMALIZE(pixel) ((pixel) >> 8)
#define GAUSSIAN_MASK_SUM 256
const uint32_t GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE] = {{1, 4, 6, 4, 1},
                                                                        {4, 16, 24, 16, 4},
                                                                        {6, 24, 36, 24, 6},
                                                                        {4, 16, 24, 16, 4},
                                                                        {1, 4, 6, 4, 1}};
#elif GAUSS_APPROX == 2
#define GAUSS_NORMALIZE(pixel) ((pixel) >> 12)
#define GAUSSIAN_MASK_SUM 4096
const uint32_t GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE] = {{11, 53, 88, 53, 11},
                                                                        {53, 239, 139, 239, 53},
                                                                        {88, 139, 139, 139, 88},
                                                                        {53, 239, 139, 239, 53},
                                                                        {11, 53, 88, 53, 11}};
#endif


void canny(pixel_stream &src, pixel_stream &dst);

// Perform a conversion from RGB to grayscale
void rgb2gray(pixel_stream &src, pixel_stream &dst);

// Gaussian blur
void gaussian(pixel_stream &src, pixel_stream &dst);
