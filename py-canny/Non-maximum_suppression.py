import numpy as np
import cv2
import matplotlib.pyplot as plt



def sobel_filter(image):
    Kx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], np.float32)
    Ky = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]], np.float32)

    Ix = cv2.filter2D(image, -1, Kx)
    Iy = cv2.filter2D(image, -1, Ky)

    img_sobel = np.sqrt(np.square(Ix) + np.square(Iy))
    img_sobel = (img_sobel / np.max(img_sobel)) * 255
    print(Iy.shape)
    theta = np.arctan2(Iy, Ix)
    return (img_sobel,theta)




def non_max_sup(img, D):
    # pass

    x,y = img.shape
    res = np.zeros((x, y), dtype=np.int32)
    print(x, y)
    angle = D * 180. / np.pi
    # for i in angle:
    #     if i < 0:
    #         i += 180
    print(res.shape)
    angle[angle < 0] += 180
    for i in range(1, x-1):
        for j in range(1, y-1):
            ref1 = 255
            ref2 = 255
            # 0 swing 45
            if (0 <= angle[i,j] < 22.5) or (157.5 <= angle[i,j] <= 180):
                ref1 = img[i, j+1]
                ref2 = img[i, j-1]
            # 45 swing 45
            elif (22.5 <= angle[i,j] <67.5):
                ref1 = img[i+1, j]
                ref2 = img[i-1, j]
            elif (67.5 <= angle[i, j] < 112.5):
                ref1 = img[i - 1, j + 1]
                ref2 = img[i + 1, j - 1]
            elif (112.5 <= angle[i, j] < 157.5):
                ref1 = img[i - 1, j - 1]
                ref2 = img[i + 1, j + 1]
            if (img[i,j] >= ref1) and (img[i,j] >= ref2):
                res[i,j] = img[i,j]
            else:
                res[i,j] = 0
    return res


img = cv2.imread("img.png")
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
(img, theta) = sobel_filter(img)
for i in range(1,img.shape[1]-1):
    print(i)
res_img = non_max_sup(img, theta)
print(res_img)
plt.imshow(res_img, cmap = 'gray')
plt.show()