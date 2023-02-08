from noise_reduction import gauss_kernel
import numpy as np

kernel = gauss_kernel(5, 1)
print(kernel)

# convert the kernel into a interger array
kernel = np.array(kernel * 4096, dtype=np.uint8)
print(kernel)
