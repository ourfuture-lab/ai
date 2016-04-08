from cv2 import *
import numpy as np

img = imread('1.jpg')
imgray = cvtColor(img, COLOR_BGR2GRAY)
ret, thresh = threshold(imgray, 127, 255, 0)

image, contours, hierarchy = findContours(thresh, RETR_TREE, CHAIN_APPROX_SIMPLE)

img = drawContours(img, contours, -1, (0,255,0), 3)

imshow('',img)
waitKey(0)