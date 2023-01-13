# STEP1: NOISE REDUCTION
# Input:    Starting image
# Output:   Noise reducted image


# Import libraries
import numpy as np
import cv2


# Gaussian kernel: G(x, y) = 1/(2*pi*sigma^2)*exp[-(x^2+y^2)/(2*sigma^2)]
# This function create a 2D Gaussian kernel, and it is normalized so that the sum of all its values is 1.
# This allows to maintain the same brightness in the image.
def gauss_kernel(size_kernel=3, sigma=1):
    # G(x, y) = 1/(2*pi*sigma^2)*exp[-(x^2+y^2)/(2*sigma^2)]
    # Create the kernel
    kernel = np.zeros((size_kernel, size_kernel))
    # Get the center of the kernel
    center = size_kernel // 2

    # Fill the kernel
    for i in range(size_kernel):
        for j in range(size_kernel):
            x, y = i - center, j - center
            kernel[i, j] = np.exp(-(x**2 + y**2) / (2*sigma**2))

    # Normalize the kernel
    return kernel / kernel.sum()

    
# This function performs the noise reduction.
# This is a prelimanry step which is needed before detecting edge of the image.
# It uses the Gaussian smoothing filter in order to reduce noise.
def denoise_conv_manual(in_image, size_kernel=3, sigma=1):
    # Create a kernel
    kernel = gauss_kernel(size_kernel, sigma)

    filtered_image = np.zeros(in_image.shape)

    pad_width = size_kernel // 2

    # print("pad_width", pad_width)

    # Pad the image
    img_pad = np.pad(in_image, ((pad_width, pad_width), (pad_width, pad_width)), mode='reflect')
    # print("img shape", np.shape(in_image))
    # print("img_pad shape", np.shape(img_pad))
    
    # Convolution
    for i in range(in_image.shape[0]):
        for j in range(in_image.shape[1]):
            filtered_image[i, j] = (kernel * img_pad[i:i+size_kernel, j:j+size_kernel]).sum()
    
    return filtered_image

def denoise(in_image, size_kernel=3, sigma=1):
     # Create a kernel
    kernel = gauss_kernel(size_kernel, sigma)

    # Apply the kernel as a convolution through the filter2D function
    filtered_image = cv2.filter2D(in_image, -1, kernel)
    return filtered_image

# SCRIPT
# Read the image
image = cv2.imread('pictures/Noise-Image.1.jpg')
# Convert the image to grayscale
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# Denoise the image
out_image = denoise(gray)
# Save the image
cv2.imwrite('pictures/gauss-Image.1.jpg', out_image)

# Denoise the image manually (without using the filter2D function) in order to compare the results
cv2.imwrite('pictures/gauss-Image-manual-conv.1.jpg', denoise_conv_manual(gray))