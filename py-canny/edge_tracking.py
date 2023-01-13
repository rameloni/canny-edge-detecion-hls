# STEP5:    Edge tracking by hysteresis thresholding
# Input:    Prepared image
# Output:   Image with detected edges

# Import libraries
import numpy as np
import cv2

# This function performs the edge tracking by hysteresis thresholding.
# This is the last step of the edge detection process.
# It uses the high and low threshold values to detect the edges.
def edge_tracking(in_image, weak, strong):
    # Get the image shape
    rows, cols = in_image.shape

    # Create a copy of the image
    out_image = np.zeros((rows, cols))

    # Create two images to store the strong and weak edges
    strong_edge_img = np.zeros((rows, cols))
    weak_edge_img = np.zeros((rows, cols))

    for i in range(1, rows-1):
        for j in range(1, cols-1):
            # If the pixel is strong
            if in_image[i, j] == strong:
                in_image[i, j] = 255
            # If the pixel is weak
            elif in_image[i, j] == weak:
                # Check if the pixel is connected to a strong pixel
                if (in_image[i+1, j-1] == strong or in_image[i+1, j] == strong or in_image[i+1, j+1] == strong or
                    in_image[i, j-1] == strong or in_image[i, j+1] == strong or
                    in_image[i-1, j-1] == strong or in_image[i-1, j] == strong or in_image[i-1, j+1] == strong):
                    in_image[i, j] = 255
                # If not, set the black pixel
                else:
                    in_image[i, j] = 0
    
    return in_image

if __name__ == '__main__':
    # Path: edge_detection.py
    # Read the image
    img = cv2.imread('pictures/double-thresholding-image.png')
    # Convert the image to grayscale
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Apply the edge detection
    img_edge = edge_tracking(img_gray, 50, 100)
    # Show the image    
    cv2.imshow('Edge detection', img_edge)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
