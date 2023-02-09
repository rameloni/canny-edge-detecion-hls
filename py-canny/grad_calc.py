import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image
from scipy import ndimage


def sobel_filter(image):
    # Find the direction and change it to degree
    # theta = np.arctan2(sobely_64, sobelx_64)
    # return (mag, theta)

    # Read the image shape
    im_shape = image.shape

    # Define the kernels
    kernel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    kernel_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

    # Reshape the kernels as 1D arrays
    image = image.reshape(image.size)
    kernel_x = kernel_x.reshape(kernel_x.size)
    kernel_y = kernel_y.reshape(kernel_y.size)

    # Apply kernels to the image using 2D convolution
    im_x = np.abs(np.convolve(image, kernel_x, mode='same'))
    im_y = np.abs(np.convolve(image, kernel_y, mode='same'))

    # Reshape the image to its original shape
    im_x = im_x.reshape(im_shape)
    im_y = im_y.reshape(im_shape)

    # Combine the results to produce the final image
    im_sobel = np.sqrt(np.square(im_x) + np.square(im_y))

    # Normalize the image
    im_sobel = im_sobel / np.max(im_sobel) * 255

    # Find the direction and change it to degree
    theta = np.arctan2(im_y, im_x)

    return (im_sobel, theta)


if __name__ == '__main__':

    img = cv2.imread('pictures/origin/img.png')

    # img_arr = np.array(img)
    img_arr = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # img_arr = cv2.fastNlMeansDenoising(img_arr, None, 20, 7, 21)

    # Apply the sobel filter
    (img_sobel, grad_dir) = sobel_filter(img_arr)

    plt.subplot(1, 2, 1)
    plt.imshow(img_arr, cmap='gray')
    plt.title('Original')

    plt.subplot(1, 2, 2)
    plt.imshow(np.uint8(img_sobel), cmap='gray')
    plt.title('Sobel Filtered')

    plt.show()
