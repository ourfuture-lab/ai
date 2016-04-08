import cv2
import numpy as np
from matplotlib import pyplot as plt

mean_filter = np.ones((3,3))

x = cv2.getGaussianKernel(5,10)

gaussian = x*x.T
scharr = np.array([[-3, 0, 3],
[-10,0,10],
[-3, 0, 3]])