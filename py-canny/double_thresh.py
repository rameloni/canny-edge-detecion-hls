import numpy as np
import cv2
import matplotlib.pyplot as plt
from PIL import Image
from grad_calc import sobel_filter

def threshold(img, lowThresholdRatio = 0.5, highThresholdRatio = 0.17):
    
    highThreshold = img.max() * highThresholdRatio
    lowThreshold = highThreshold * lowThresholdRatio
    
    M, N = img.shape
    res = np.zeros((M,N), dtype=np.uint8)
    
    weak = np.uint8(75)
    strong = np.uint8(255)
    
    strong_i, strong_j = np.where(img >= highThreshold)
    zeros_i, zeros_j = np.where(img < lowThreshold)
    
    weak_i, weak_j = np.where((img <= highThreshold) & (img >= lowThreshold))
    
    res[strong_i, strong_j] = strong
    res[weak_i, weak_j] = weak
    # res[zeros_i, zeros_j] = 0
    return (res, weak, strong)

if __name__ == '__main__':
    img = Image.open("C:\\Users\\amirr\\Desktop\\Education\\TU Delft\\Q2\\Reconfigurable Computing Design\\edgedetection1.jpg")

    img_arr = np.array(img)
    img_arr = cv2.cvtColor(img_arr, cv2.COLOR_BGR2GRAY)
    # img_arr = cv2.fastNlMeansDenoising(img_arr, None, 20, 7, 21) 

    img_sobel, theta = sobel_filter(img_arr)

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