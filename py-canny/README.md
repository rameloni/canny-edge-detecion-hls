# Canny edge detection - Python implementation
To execute the code, run the following command:
```bash
$ cd py-canny
$ python3 canny.py
```
The code will read all the images in [pictures/origin/](pictures/origin/) folder, apply the Canny edge detection algorithm to them and save the results in [pictures/result/](pictures/result/) folder. In addition it shows the intermediate results of the algorithm, the final result and the execution time. 

## Execution time
The code has been tested on a computer with the following specifications:
- Model: Asus ROG Zephyrus G14 GA401Q (2020)
- OS: Ubuntu 20.04.2 LTS
- CPU: AMD Ryzen 7 5800HS @ 3.20GHz
- RAM: 16GB

The execution time of the code is shown in the following table:
| Image    | Execution time (s) |
| -------- | ------------------ |
| 1280x720 | 1.1                |
| 366x574  | 0.25               |
| 300x300  | 0.1                |
| 490x480  | 0.28               |
| 768x487  | 0.34               |
