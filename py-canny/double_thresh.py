import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image
from grad_calc import sobel_filter
from noise_reduction import denoise


def threshold(img, lowThresholdRatio=0.5, highThresholdRatio=0.17):

    # Calculate the high and low threshold
    highThreshold = img.max() * highThresholdRatio
    lowThreshold = highThreshold * lowThresholdRatio

    # Get the dimensions of the image
    M, N = img.shape
    res = np.zeros((M, N), dtype=np.uint8)

    # Strong and weak edges
    weak = np.uint8(75)
    strong = np.uint8(255)

    # Get the indices of the strong and weak edges
    strong_i, strong_j = np.where(img >= highThreshold)
    weak_i, weak_j = np.where((img <= highThreshold) & (img >= lowThreshold))
    # zeros_i, zeros_j = np.where(img < lowThreshold)

    # Set the strong edges
    res[strong_i, strong_j] = strong
    res[weak_i, weak_j] = weak

    # res[zeros_i, zeros_j] = 0

    return (res, weak, strong)


if __name__ == '__main__':

    img = cv2.imread('pictures/origin/img.png')

    # img_arr = np.array(img)
    img_arr = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # img_arr = cv2.fastNlMeansDenoising(img_arr, None, 20, 7, 21)

    # Apply denoise
    img_den = denoise(img_arr)
    
    # Apply the sobel filter
    img_sobel, theta = sobel_filter(img_arr)

    # Apply the double threshold
    img_dt, weak, strong = threshold(img_sobel)

    plt.subplot(1, 3, 1)
    plt.imshow(img_arr, cmap='gray')
    plt.title('Original')

    plt.subplot(1, 3, 2)
    plt.imshow(np.uint8(img_sobel), cmap='gray')
    plt.title('Sobel Filtered')

    plt.subplot(1, 3, 3)
    plt.imshow(img_dt, cmap='gray')
    plt.title('Double Thresholed')

    plt.show()
