# Step1: Noise reduction
# Step2: Gradient calculation
# Step3: Non-maximum suppression
# Step4: Double threshold
# Step5: Edge tracking by hysteresis

import cv2
import numpy as np
from matplotlib import pyplot as plt

from noise_reduction import denoise
from grad_calc import sobel_filter
from non_maximum_suppression import non_max_sup
from double_thresh import threshold
from edge_tracking import edge_tracking

if __name__ == '__main__':
    img = cv2.imread('pictures/Noise-Image.2.png')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # 1. Denoise the image
    img = denoise(img, 10)
    cv2.imwrite('pictures/denoised-Image.1.jpg', img)
    # 2. Gradient calculation
    img_sobel, theta = sobel_filter(img)
    cv2.imwrite('pictures/sobel-Image.1.jpg', np.uint8(img_sobel))
    # 3. Non-maximum suppression
    img_sup = non_max_sup(img_sobel, theta)
    cv2.imwrite('pictures/non-maximum-Image.1.jpg', img_sup)
    # 4. Double threshold
    img_thresh, weak, strong = threshold(img_sup)
    cv2.imwrite('pictures/double-threshold-Image.1.jpg', img_thresh)
    # 5. Edge tracking by hysteresis
    img_final = edge_tracking(img_thresh, weak, strong)
    # 6. Save the image
    cv2.imwrite('pictures/canny-Image.1.jpg', img_final)


