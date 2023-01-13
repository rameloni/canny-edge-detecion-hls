# Canny Edge Detection on Pynq - Group 3 - Reconfigurable Computing Design
The Canny edge detector uses a multi-stage algorithm to detect a wide range of edges in images and has been widely applied in various computer vision systems. 

## Algorithm
The Canny edge detection algorithm has 5 stages:
- **Noise Reduction** - Using Gaussian filter to blur the input image and remove the existing noise inside the image.
- **Gradient calculation** - Calculating the intensity gradients of the image.
- **Non-maximum suppression** - Determining the appropriate threshold to avoid recognizing edge wrongly.
- **Double threshold** - Applying a double threshold to determine potential edges.
- **Edge Tracking by Hysteresis** - Pruning edges with weak values and not connected to strong edges.

## Resources
- **Canny edge detection based on Open CV**: [https://ieeexplore.ieee.org/abstract/document/8265710](https://ieeexplore.ieee.org/abstract/document/8265710).
- **An improved Canny edge detection algorithm**: [https://ieeexplore.ieee.org/abstract/document/6885761](https://ieeexplore.ieee.org/abstract/document/6885761). 

| Used?                | Resource                                                             | Link                                                                                                           |
| -------------------- | -------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
| :white_large_square: | **Canny edge detection based on Open CV**                            | [https://ieeexplore.ieee.org/abstract/document/8265710](https://ieeexplore.ieee.org/abstract/document/8265710) |
| :white_large_square: | **An improved Canny edge detection algorithm**                       | [https://ieeexplore.ieee.org/abstract/document/6885761](https://ieeexplore.ieee.org/abstract/document/6885761) |
| :white_large_square: | **Real-time Canny edge detection parallel implementation for FPGAs** | [https://ieeexplore.ieee.org/abstract/document/5724558](https://ieeexplore.ieee.org/abstract/document/5724558) |

# Project Deliverables
<!-- :white_large_square: and :heavy_check_mark: -->

| Done                 | Deliverable            |
| -------------------- | ---------------------- |
| :white_large_square: | Python code (software) |
| :white_large_square: | HLS code (hardware)    |
| :white_large_square: | Report                 |

The codes must be provided in such a manner to allow for quick download to the PYNQ board for quick verification of the achieved results. This means including the bitstream, the hardware handoff file, and a Jupyter notebook to interface to the hardware.

# Software
<!-- :white_large_square: and :heavy_check_mark: -->

| Done                 | Stage                       | Function Name                                       | Tested on Pynq       |
| -------------------- | --------------------------- | --------------------------------------------------- | -------------------- |
| :white_large_square: | Noise reduction             | *Put here the python function name (to keep track)* | :white_large_square: |
| :white_large_square: | Gradient calculation        | *Put here the python function name (to keep track)* | :white_large_square: |
| :white_large_square: | Non-maximum suppression     | *Put here the python function name (to keep track)* | :white_large_square: |
| :white_large_square: | Double threshold            | *Put here the python function name (to keep track)* | :white_large_square: |
| :white_large_square: | Edge Tracking by Hysteresis | *Put here the python function name (to keep track)* | :white_large_square: |

## Other details
- Video examples on the PYNQ can be used as a starting point.
- **Numpy** and/or **OpenCV** can be used to implement the video processing in Python.
- OpenCV provides functionality to load images and video, then it is also possible towork on this assignment without the PYNQ board by using input data other than an HDMI stream.
- Write the code for the video processing in a separate file from the notebook. The notebook will call this file then.

## Performance recording
Once the algorithm is implemented in Python:
- Performance can be recorded by measuring the framerate of the output stream. Do that on Pynq.
- Video examples already give a way to do this.

# Hardware
| Done                 | Stage                       | Block Name                                    | Tested on Pynq       |
| -------------------- | --------------------------- | --------------------------------------------- | -------------------- |
| :heavy_large_square: | Noise reduction             | *noise_reduction.py* | :white_large_square: |
| :white_large_square: | Gradient calculation        | *Put here the HLS block name (to keep track)* | :white_large_square: |
| :white_large_square: | Non-maximum suppression     | *Put here the HLS block name (to keep track)* | :white_large_square: |
| :white_large_square: | Double threshold            | *Put here the HLS block name (to keep track)* | :white_large_square: |
| :white_large_square: | Edge Tracking by Hysteresis | *Put here the HLS block name (to keep track)* | :white_large_square: |

## Add custom IP
- Use the example `run.tcl` tcl file to setup the IP diagram on Vivado. `source run.tcl`
- Insert the custom IP there in the video module of the block design.
- Add another AXI master to the interconnect IP already present in the video block.
- Connect the IP to **142 MHz** clock and the corresponding **reset**.

## Other details
- When writing the HLS code, note that it is not software to be executed, but rather a **description of hardware to be generated**.
- Variables will become either wires or registers.
- **Keep track** of how each variable is used. **Think** about hoe the code will be synthesized.
- **HLS also includes several OpenCV functions that can be used in hardware**. These functions provide high-level functionality.
- Target clock **142 MHz**, set the solution synthesis settings using 7ns. *Default frequency of resolution 1128x720 pixels*
- `streamulator` is useful fot testing.
- To use the streamulator both the .cpp and .h files have to be added to the HLS project as simulator source

## Hardware testing
- The student should be confident about whether the code will produce a useful result, to avoid excess time spent waiting. 
- A good idea is to add a control port which allows certain parts of the algorithm to be turned on and off; this can even be left in to provide control of the final block.
- To test a processing block design, the HDMI ports have to be connected directly to each other in Python.
- When done testing, it should be noted that the HDMI connection has to be closed.
- Measuring performance using only the framerate (Video source has 60 fps) **will not yield improvements**. **Other metrics** are needed.
- For example, adding more stages to a video processing pipeline does not impact throughput, but instead adds to the total latency. This can be also used as a metric.
- Also the total utilized area of one's design.

# Report
Link to overleaf document: [report](https://www.overleaf.com/2341325728xzbrcdhtqtpd).

| Done                 | Part                                      |
| -------------------- | ----------------------------------------- |
| :white_large_square: | Descritption of the project functionality |
| :white_large_square: | Experimental setup                        |
| :white_large_square: | Experimental results                      |
| :white_large_square: | Reflection on chosen approach             |
| :white_large_square: | Reflection on achievements                |
