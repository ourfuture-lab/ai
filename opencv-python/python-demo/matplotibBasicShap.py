import numpy as np
import cv2
from matplotlib import pyplot as plt

img = cv2.imread('2.jpg')
plt.imshow(img, cmap = 'blue', interpolation='bicubic')
plt.xticks([]), plt.yticks([])
plt.show()