# Canny edge detection

# Step1: Noise reduction
# Step2: Gradient calculation
# Step3: Non-maximum suppression
# Step4: Double threshold
# Step5: Edge tracking by hysteresis

import cv2
import numpy as np
from matplotlib import pyplot as plt
import time

from noise_reduction import denoise
from grad_calc import sobel_filter
from non_maximum_suppression import non_max_sup
from double_thresh import threshold
from edge_tracking import edge_tracking

import os

if __name__ == '__main__':

    # Image path
    in_dir = './pictures/'
    orig_dir = in_dir + 'origin/'

    filenames = os.listdir(orig_dir)

    # Loop over all the images
    for filename in filenames:
        # Print the image name
        print(filename)

        # Read the image
        img = cv2.imread(orig_dir+filename)
        # img = cv2.imread('./pictures/img.png')

        # 0. Convert to grayscale
        start1 = time.time()
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        stop1 = time.time()

        ####################################################

        # Path denoise
        outpath = in_dir+'1_denoise/'
        if not os.path.exists(outpath):
            os.makedirs(outpath)

        # 1. Denoise the image
        start2 = time.time()
        img_den = denoise(img, size_kernel=5)
        stop2 = time.time()

        # Save the image
        cv2.imwrite(outpath+'denoised-'+filename, img_den)

        ####################################################

        # Path sobel
        outpath = in_dir+'2_sobel/'
        if not os.path.exists(outpath):
            os.makedirs(outpath)

        # 2. Gradient calculation
        start3 = time.time()
        img_sobel, theta = sobel_filter(img_den)
        stop3 = time.time()

        # Save the image
        cv2.imwrite(outpath+'sobel-'+filename, np.uint8(img_sobel))

        ####################################################

        # Path non-maximum suppression
        outpath = in_dir+'3_non_max/'
        if not os.path.exists(outpath):
            os.makedirs(outpath)

        # 3. Non-maximum suppression
        start4 = time.time()
        img_sup = non_max_sup(img_sobel, theta)
        stop4 = time.time()

        # Save the image
        cv2.imwrite(outpath+'non_max_sup'+filename, img_sup)

        ####################################################

        # Path double threshold
        outpath = in_dir+'4_thresh/'
        if not os.path.exists(outpath):
            os.makedirs(outpath)

        # 4. Double threshold
        start5 = time.time()
        img_thresh, weak, strong = threshold(img_sup)
        stop5 = time.time()

        # Save the image
        cv2.imwrite(outpath+'thresh-'+filename, img_thresh)

        ####################################################

        # Path edge tracking
        outpath = in_dir+'5_canny/'
        if not os.path.exists(outpath):
            os.makedirs(outpath)

        # 5. Edge tracking by hysteresis
        start6 = time.time()
        img_final = edge_tracking(img_thresh, weak, strong)
        stop6 = time.time()

        # Save the image
        cv2.imwrite(outpath+'canny-'+filename, img_final)

        ####################################################

        # Compare with cv2
        edges_cv2 = cv2.Canny(img, 100, 200, L2gradient=False)
        cv2.imwrite(outpath+'canny-'+filename+'cv2.png', edges_cv2)

        # Stop the timer for the whole process
        stop = time.time()

        # Print the time used
        runtime = stop1+start2+stop3+stop4+stop5+stop6 - \
            (start1+start2+start3+start4+start5+start6)
        print("time used: ", runtime)

        ####################################################
        # 7. Show the images

        # Gray scale: original
        plt.subplot(2, 3, 1)
        plt.imshow(img, cmap='gray')
        plt.title('0-Original')

        # Denoised
        plt.subplot(2, 3, 2)
        plt.imshow(img_den, cmap='gray')
        plt.title('1-Denoised image')

        # Sobel
        plt.subplot(2, 3, 3)
        plt.imshow(np.uint8(img_sobel), cmap='gray')
        plt.title('2-Sobel')

        # Non-maximum suppression
        plt.subplot(2, 3, 4)
        plt.imshow(img_sup, cmap='gray')
        plt.title('3-Non-maximum suppression')

        # Double threshold
        plt.subplot(2, 3, 5)
        plt.imshow(img_thresh, cmap='gray')
        plt.title('4-Double Thresholed')

        # Edge tracking
        plt.subplot(2, 3, 6)
        plt.imshow(img_final, cmap='gray')
        plt.title('5-Edge Tracking')

        # cv2
        cv2.imshow('Canny', img_final)
        cv2.imshow('Canny_cv2', edges_cv2)

        # Show the images
        plt.show()
