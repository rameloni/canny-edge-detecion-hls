import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image
from scipy import ndimage


def sobel_filter(image):

    # # Apply Sobelx in high output datatype 'float32'
    # # and then converting back to 8-bit to prevent overflow
    # sobelx_64 = cv2.Sobel(image,cv2.CV_32F,1,0,ksize=3)
    # absx_64 = np.absolute(sobelx_64)
    # sobelx_8u1 = absx_64/absx_64.max()*255
    # sobelx_8u = np.uint8(sobelx_8u1)

    # # Similarly for Sobely
    # sobely_64 = cv2.Sobel(image,cv2.CV_32F,0,1,ksize=3)
    # absy_64 = np.absolute(sobely_64)
    # sobely_8u1 = absy_64/absy_64.max()*255
    # sobely_8u = np.uint8(sobely_8u1)

    # # From gradients calculate the magnitude and changing
    # # it to 8-bit (Optional)
    # mag = np.hypot(sobelx_8u, sobely_8u)
    # mag = mag/mag.max()*255
    # mag = np.uint8(mag)

    # # Find the direction and change it to degree
    # theta = np.arctan2(sobely_64, sobelx_64)
    # return (mag, theta)
    im_shape = image.shape

    kernel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    kernel_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

    image = image.reshape(image.size)
    kernel_x = kernel_x.reshape(kernel_x.size)
    kernel_y = kernel_y.reshape(kernel_y.size)
    # Apply kernels to the image using 2D convolution
    im_x = np.abs(np.convolve(image, kernel_x, mode='same'))
    im_y = np.abs(np.convolve(image, kernel_y, mode='same'))

    im_x = im_x.reshape(im_shape)
    im_y = im_y.reshape(im_shape)

    # Combine the results to produce the final image
    im_sobel = np.sqrt(np.square(im_x) + np.square(im_y))

    # Normalize the image
    im_sobel = im_sobel / np.max(im_sobel) * 255
    theta = np.arctan2(im_y, im_x)

    return (im_sobel, theta)

    Kx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], np.float32)
    Ky = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]], np.float32)

    # This is the cause of the problem
    Ix = cv2.filter2D(image, -1, Kx)
    Iy = cv2.filter2D(image, -1, Ky)

    img_sobel = np.sqrt(np.square(Ix) + np.square(Iy))
    # img_sobel = np.hypot(Ix, Iy)
    img_sobel = img_sobel / np.max(img_sobel) * 255
    # print(Iy.shape)
    theta = np.arctan2(Iy, Ix)

    return (img_sobel, theta)


if __name__ == '__main__':

    img = Image.open(
        "C:\\Users\\amirr\\Desktop\\Education\\TU Delft\\Q2\\Reconfigurable Computing Design\\edgedetection1.jpg")

    img_arr = np.array(img)
    # img_arr = cv2.fastNlMeansDenoising(img_arr, None, 20, 7, 21)

    img_sobel = sobel_filter(img_arr)

    plt.subplot(1, 2, 1)
    plt.imshow(img_arr)
    plt.title('Original')

    plt.subplot(1, 2, 2)
    plt.imshow(np.uint8(img_sobel))
    plt.title('Sobel Filtered')

    plt.show()
