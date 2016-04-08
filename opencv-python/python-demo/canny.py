from cv2 import *
import numpy as np

def nothing():
    pass



img = imread('google.jpg')

namedWindow('canny')
createTrackbar('l', 'canny', 0, 5000, nothing)
createTrackbar('h', 'canny', 0, 5000, nothing)

while(True):
    l = getTrackbarPos('l', 'canny')
    h = getTrackbarPos('h', 'canny')
    edges = Canny(img, l, h)
    imshow('canny', edges)
    k=waitKey(1)&0xFF
    if k==27:
        break


destroyAllWindows()




