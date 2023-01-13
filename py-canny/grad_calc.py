import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image

def sobel_filter(image):
    Kx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], np.float32)
    Ky = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]], np.float32)

    Ix = cv2.filter2D(image, -1, Kx)
    Iy = cv2.filter2D(image, -1, Ky)

    img_sobel = np.sqrt(np.square(Ix) + np.square(Iy))
    img_sobel = (img_sobel / np.max(img_sobel)) * 255

    return img_sobel


if __name__ == '__main__':

    img = Image.open("C:\\Users\\amirr\\Desktop\\Education\\TU Delft\\Q2\\Reconfigurable Computing Design\\edgedetection1.jpg")

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