# A Canny Edge Detection for Video Processing on FPGA
The Canny edge detector uses a multi-stage algorithm to detect a wide range of edges in images and has been widely applied in various computer vision systems. 
## Authors
- Raffaele Meloni ([GitHub](https://github.com/rameloni), [LinkedIn](https://www.linkedin.com/in/raffaele-meloni-886a1a208/))
- Leslie Xu ([GitHub](https://github.com/Lesliehsyu), [LinkedIn](https://www.linkedin.com/in/leslie-shijie-xu-0a157b1a2/))
- Amirreza Movahedin ([GitHub](https://github.com/AmirrezaMov), [LinkedIn](https://www.linkedin.com/in/amirreza-movahedin-893652151/))

## Deliverables
<!-- :white_large_square: and :heavy_check_mark: -->
| Deliverable                              | Location                                         |
| ---------------------------------------- | ------------------------------------------------ |
| Python implementation (software)         | [py-canny/](py-canny)                            |
| HLS implementation (hardware)            | [hls-canny/](hls-canny)                          |
| Bitstreams                               | [hls-canny/deliverables](hls-canny/deliverables) |
| Jupyter notebook for Pynq-Z1 and Pynq-Z2 | [jupyter-canny/](jupyter-canny)                  |

## Index
- [Setup](#setup)
- [Algorithm](#algorithm)
- [Software Implementation](#software-implementation)
- [Hardware Implementation in HLS](#hardware-implementation-in-hls)
- [CV2 python vs HLS](#cv2-python-vs-hls)



# Setup

# Algorithm
The Canny edge detection algorithm has 5 stages (plus a preprocessing stage):
- **Grayscale conversion** - Converting the input image to grayscale.
- **Noise Reduction** - Using Gaussian filter to blur the input image and remove the existing noise inside the image.
- **Gradient calculation** - Calculating the intensity gradients of the image.
- **Non-maximum suppression** - Determining the appropriate threshold to avoid recognizing edge wrongly.
- **Double threshold** - Applying a double threshold to determine potential edges.
- **Edge Tracking by Hysteresis** - Pruning edges with weak values and not connected to strong edges.

![canny-pipeline](pictures/canny-pipeline-.png)


# Software Implementation
<!-- :white_large_square: and :heavy_check_mark: -->
The software implementation of the Canny edge detection algorithm is done in Python. The code is tested on the PYNQ-Z1 and PYNQ-Z2 boards.
Each stage of the algorithm is implemented in a separate file. A source directory contains all the input images which are read and processed by the code. 
In the [canny.py](py-canny/canny.py) file, the images are processed, shown and stored for each substep.


# Hardware Implementation in **HLS**
The hardware implementation of the Canny edge detection algorithm is done in **HLS**. The code is tested on the PYNQ-Z1 and PYNQ-Z2 boards. 
The hardware realization allows to implement a video processing pipeline. Following steps explains how a canny edge detection is implemented in HLS.

Each pipeline stage is implemented taking care the latency. Divisions and floating point operations are avoided if possible, since they introduce a lot of latency which might exceeds the timing requirements (7ns).

## High-level block diagram
<!-- :white_large_square: and :heavy_check_mark: -->
Video processing pipeline is a sequence of processing blocks that are connected in a chain. Each block is responsible for a specific step.
HDMI modules handle video frames as streams, therefore FIFOs between pipeline steps are used for flow control.
The following diagram shows the high-level block diagram of the Canny edge detection algorithm in hardware.

![canny-hw-block-diagram](pictures/canny-hw-pipeline.png)

## **Convolution** in hardware
Hardware handles streams of pixels and it is
not possible to store full frame completely. Therefore there is no way to iterate an input frame enterely. 
We implemented a line buffer and sliding window from scratch to perform convolution. 
The line buffer stores the input frame line by line.

![line-buffer](pictures/line-buffer.png)

The sliding window is a 2D array that is moved over the line buffer to perform the convolution.

![sliding-window](pictures/sliding-window.png)

----------
## HLS substages implementation
### 0. Grayscale conversion
It is a preprocessing step that converts the input image to grayscale. The HLS code is implemented in the [grayscale.cpp](hls-canny/grayscale.cpp) file.
There are multiple ways to convert an image to grayscale. The HLS code uses the following formula:
    
    Gray = Red ∗ 0.25 + Green ∗ 0.5 + Blue ∗ 0.25

We chose to use this approximation instead of other well known approaches since it avoids floating point operations and divisions. In fact, it can be implemented in hardware through shift operations which are particurarly suited for FPGAs:

    Gray = (Red + Green << 1 + Blue) >> 2

| Input image                  | Grayscale image                          |
| ---------------------------- | ---------------------------------------- |
| ![input](pictures/input.jpg) | ![grayscale](pictures/hls/grayscale.png) |

### 1. Noise reduction -Ggaussian blur
The Canny algorithm is extremely sensitive to noise. Noise reduction allows to detect better the edges in the image.
The Gaussian blur is a common technique used to reduce noise in images using a convolution kernel.

$K(x, y) = \frac{1}{\sqrt{2\pi \sigma^2}} e^{-\frac{x^2+y^2}{2\sigma^2}} =\frac{1}{\sqrt{2\pi}} e^{-\frac{x^2+y^2}{2}}$ for $\sigma = 1$

But it leads to a floating point kernel, which is not well suited for hardware implementation. Therefore, we decided to use an approximation which provides a good trade-off between accuracy and performance. This approximation exploits shift operations and integer arithmetic.
$$\begin{equation*}
    K_{approx} = \frac{1}{256}
    \begin{bmatrix}
    1 & 4 & 6 & 4 & 1 \\
    4 & 16 & 24 & 16 & 4 \\
    6 & 24 & 36 & 24 & 6 \\
    4 & 16 & 24 & 16 & 4 \\
    1 & 4 & 6 & 4 & 1
    \end{bmatrix}
\end{equation*}$$

| Input image (grayscaled)                 | Gaussian blur image                              |
| ---------------------------------------- | ------------------------------------------------ |
| ![grayscale](pictures/hls/grayscale.png) | ![gaussian-blur](pictures/hls/gaussian-blur.png) |

### 2. Gradient calculation - Sobel
This step of the pipeline, the intensity and direction of the edges are detected. 
An edge is a sudden change in the intensity of the image. 
The Sobel operator is used to calculate the intensity gradient of the image.
Two kernels are convolutioned with the image to calculate the gradient in the horizontal (y) and vertical (x) direction.
$$\begin{equation*}
    K_x=
    \begin{bmatrix}
    -1 & 0 & 1 \\
    -2 & 0 & 2 \\
    -1 & 0 & 1
    \end{bmatrix},\:
    K_y=
    \begin{bmatrix}
    1 & 2 & 1 \\
    0 & 0 & 0 \\
    -1 & -2 & -1
    \end{bmatrix}
\end{equation*}$$

The convolution prouduces two derivatives in the x and y direction ($I_x, I_y$). 
The gradient magnitude is calculated using an approximation of the Pythagorean theorem in order to save latency: 

$out(x, y) = \sqrt{I_x^2 + I_y^2} \approx |I_x| + |I_y|$.

The gradient direction is calculated using an approximation of the tangent function:

$tan(x, y) \approx \frac{I_y}{I_x}$ this value is compared with known values of the tangent function of 4 direction regions:
- Completely horizontal: 0° or 180° (*from -22.5° to 22.5°*)
- Completely vertical: 90° or 270° (*from 67.5° to 112.5°*)
- Positive slope: 45° or 225° (*from 22.5° to 67.5°*)
- Negative slope: 135° or 315° (*from 112.5° to 157.5°*)

| Input image (gaussian blur)                  | Sobel image                                   |
| -------------------------------------------- | --------------------------------------------- |
| ![grayscale](pictures/hls/gaussian-blur.png) | ![gradient-magnitude](pictures/hls/sobel.png) |


### 3. Non-maximum suppression
Ideally the final image should have thinner the edges.
The non-maximum suppression step is used to thin the edges in the image.
The gradient direction is used to determine the direction of the edge and the gradient magnitude is used to determine the strength of the edge.

Basing on the direction 2 neighboring pixels are compared to the processed pixel. 
If the intensity of either one of the 2 pixels is higher than the processed pixel, the output pixel (weaker) corresponding to the processed pixel is set to 0. Otherwise, the processed pixel remains its value.

The line buffer and the sliding window are used for the comparison in this step.

After this stage the output image results thinner compared to the previous one.

| Input image (sobel)                  | Non-maximum suppression image                                        |
| ------------------------------------ | -------------------------------------------------------------------- |
| ![grayscale](pictures/hls/sobel.png) | ![non-maximum-suppression](pictures/hls/non-maximum-suppression.png) |

### 4. Double threshold
Three kinds of edges are detected in the image: strong, weak and non-relevant:
- The strong edges contribute to the final edge.
- The non-relevant edges does not contribute to the final edge.
- The weak edges contribute to the final edge only if they are connected to a strong edge. (*This is checked in the next step*)

Two thresholds are used to classify the edges:
- The high threshold is used to classify the strong edges from the weak edges.
- The low threshold is used to classify the weak edges from the non-relevant edges. 

Neither line buffer nor sliding window are used in this step. Each pixel is processed independently, compared with the thresholds and the output is generated.

| Input image (non-maximum-suppression)                  | Double threshold image                                 |
| ------------------------------------------------------ | ------------------------------------------------------ |
| ![grayscale](pictures/hls/non-maximum-suppression.png) | ![double-threshold](pictures/hls/double_threshold.png) |

### 5. Edge tracking by hysteresis (**last pipeline stage**)
This step is used to connect the weak edges to the strong edges.
If the input pixel is strong or non-relevant, it stays the same in the output, 
but if the input pixel is weak, all 8 neighboring pixels of the input are checked: 
If none of the 8 neghboring pixels are strong, 
then the output corresponding to the input pixel is non-relevant (black), 
otherwise the output is strong (white).
```text
-------------
|   |   |   |
-------------
|   | x |   |           // x is the input pixel compared with the 8 neighboring pixels
-------------
|   |   |   |
-------------
```
This steps uses the line buffer and the sliding window to check the 8 neighboring pixels.

| Input image (double threshold)                  | Edge tracking by hysteresis image (**Final result**)   |
| ----------------------------------------------- | ------------------------------------------------------ |
| ![grayscale](pictures/hls/double_threshold.png) | ![edge-tracking-by-hysteresis](pictures/hls/canny.png) |

## Comparison between Canny with and without non-maximum suppression

| Canny image                        | Canny image (without non-maximum suppression) |
| ---------------------------------- | --------------------------------------------- |
| ![canny](pictures/hls/canny.png)   | ![canny](pictures/hls/canny-no-nms.png)       |
| ![canny](pictures/hls/canny-2.png) | ![canny](pictures/hls/canny-2-no-nms.png)     |
| ![canny](pictures/hls/canny-3.png) | ![canny](pictures/hls/canny-3-no-nms.png)     |


# CV2 python vs HLS

| Input image                    | CV2 Canny image                      | Our Canny image                   |
| ------------------------------ | ------------------------------------ | --------------------------------- |
| ![img](pictures/hls/img.jpg)   | ![cv2](pictures/hls/cv2-canny.png)   | ![ours](pictures/hls/canny.png)   |
| ![img](pictures/hls/img-2.jpg) | ![cv2](pictures/hls/cv2-canny-2.png) | ![ours](pictures/hls/canny-2.png) |
| ![img](pictures/hls/img-3.jpg) | ![cv2](pictures/hls/cv2-canny-3.png) | ![ours](pictures/hls/canny-3.png) |

