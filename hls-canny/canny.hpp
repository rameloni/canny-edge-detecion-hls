#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// Typedefs for the AXI4-Stream interface
typedef ap_axiu<32, 1, 1, 1> pixel_data;
typedef hls::stream<pixel_data> pixel_stream;



// Macros for extracting and combining RGBA components
#define rgba2r(v) ((v)&0xFF)
#define rgba2g(v) (((v)&0xFF00) >> 8)
#define rgba2b(v) (((v)&0xFF0000) >> 16)
#define rgba2a(v) (((v)&0xFF000000) >> 24)

#define r2rgba(v) ((v)&0xFF)
#define g2rgba(v) (((v)&0xFF) << 8)
#define b2rgba(v) (((v)&0xFF) << 16)
#define a2rgba(v) (((v)&0xFF) << 24)

// Image parameters
#define WIDTH 1280
#define HEIGHT 720

// Gaussian blur parameters
#define GAUSSIAN_MASK_SIZE 5

#define GAUSS_APPROX 1

// Sobel edge detection parameters
#define SOBEL_KERNEL_SIZE 3

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

// Sobel kernels
const int H_SOBEL_KERNEL[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE] = {{-1, 0, 1},
                                                                  {-2, 0, 2},
                                                                  {-1, 0, 1}};

const int V_SOBEL_KERNEL[SOBEL_KERNEL_SIZE][SOBEL_KERNEL_SIZE] = {{1, 2, 1},
                                                                  {0, 0, 0},
                                                                  {-1, -2, -1}};

// Thresholds for double thresholding
#define LOW_THRESHOLD 50
#define HIGH_THRESHOLD 100
#define STRONG_EDGE 255
#define WEAK_EDGE 127
#define NO_EDGE 0

// void canny(pixel_stream &src, pixel_stream &dst);

// Perform a conversion from RGB to grayscale
void rgb2gray(pixel_stream &src, pixel_stream &dst);

// Gaussian blur
void gaussian(pixel_stream &src, pixel_stream &dst);

// Sobel edge detection
void sobel(pixel_stream &src, pixel_stream &dst, ap_uint<2> &grad_dir);

// Non-maximum suppression
void non_max_sup(pixel_stream &src, pixel_stream &dst, ap_uint<2> &grad_dir);

// Double thresholding
void double_threshold(pixel_stream &src, pixel_stream &dst);

// Edge tracking
void edge_tracking(pixel_stream &src, pixel_stream &dst);
