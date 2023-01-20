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

// const uint32_t GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE] = {{11, 53, 88, 53, 11},
//                                                                        {53, 239, 139, 239, 53},
//                                                                        {88, 139, 139, 139, 88},
//                                                                        {53, 239, 139, 239, 53},
//                                                                        {11, 53, 88, 53, 11}};

const uint32_t GAUSSIAN_MASK[GAUSSIAN_MASK_SIZE][GAUSSIAN_MASK_SIZE] = {{0, 0, 0, 0, 0},
                                                                        {0, 0, 0, 0, 0},
                                                                        {0, 0, 1, 0, 0},
                                                                        {0, 0, 0, 0, 0},
                                                                        {0, 0, 0, 0, 0}};

void canny(pixel_stream &src, pixel_stream &dst);

// Perform a conversion from RGB to grayscale
void rgb2gray(pixel_stream &src, pixel_stream &dst);

// Gaussian blur
void gaussian(pixel_stream &src, pixel_stream &dst);
