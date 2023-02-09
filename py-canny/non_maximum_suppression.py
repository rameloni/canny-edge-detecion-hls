import numpy as np
import cv2
import matplotlib.pyplot as plt

from grad_calc import sobel_filter
from noise_reduction import denoise


def non_max_sup(img, D):

    # Get the image shape
    x, y = img.shape

    # Create a zero matrix
    res = np.zeros((x, y), dtype=np.uint8)

    # Get the angle
    angle = D * 180.0 / np.pi

    angle = np.rad2deg(D)
    angle[angle < 0] += 180

    # Perform non-maximum suppression
    for i in range(1, x-1):
        for j in range(1, y-1):
            # ref1 = 255
            # ref2 = 255
            # 0 swing 45
            if (0 <= angle[i, j] < 22.5) or (157.5 <= angle[i, j] <= 180):
                ref1 = img[i, j+1]
                ref2 = img[i, j-1]
            # 45 swing 45
            elif (22.5 <= angle[i, j] < 67.5):
                ref1 = img[i+1, j]
                ref2 = img[i-1, j]
            elif (67.5 <= angle[i, j] < 112.5):
                ref1 = img[i - 1, j + 1]
                ref2 = img[i + 1, j - 1]
            elif (112.5 <= angle[i, j] < 157.5):
                ref1 = img[i - 1, j - 1]
                ref2 = img[i + 1, j + 1]
                
            if (img[i, j] >= ref1) and (img[i, j] >= ref2):
                res[i, j] = img[i, j]
            else:
                res[i, j] = 0

    return res


if __name__ == '__main__':

    # Read the image
    img = cv2.imread("pictures/origin/img.png")

    # Convert to grayscale
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Apply noise reduction
    img_den = denoise(img, size_kernel=5)

    # Apply sobel filter
    img_sobel, theta = sobel_filter(img_den)

    # Apply non-maximum suppression
    res_img = non_max_sup(img_sobel, theta)

    # img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    plt.subplot(2, 2, 1)
    plt.imshow(img, cmap='gray')
    plt.title('Original')

    plt.subplot(2, 2, 2)
    plt.imshow(img_den, cmap='gray')
    plt.title('Denoised image')

    plt.subplot(2, 2, 3)
    plt.imshow(img_sobel, cmap='gray')
    plt.title('Sobel Filtered')

    plt.subplot(2, 2, 4)
    plt.imshow(res_img, cmap='gray')
    plt.title('Non-maximum Suppression')

    plt.show()
