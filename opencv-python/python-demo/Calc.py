import cv2 as cv
#import numpy as np

img1=cv.imread('21.jpg')
img2=cv.imread('22.png')

dst=cv.addWeighted(img1,0.2, img2,0.8, 0)
cv.imshow('img1',img1)
cv.imshow('dst',dst)

img3=cv.imread('22.png')
img4=cv.imread('1.jpg')

rows, cols, channels = img4.shape
roi = img3[0:rows, 0:cols]

img4gray=cv.cvtColor(img4, cv.COLOR_BGR2GRAY)
ret, mask = cv.threshold(img4gray, 175, 255, cv.THRESH_BINARY)
mask_inv = cv.bitwise_not(mask)

img3_bg = cv.bitwise_and(roi, roi, mask=mask)
img4_fg = cv.bitwise_and(img4, img4, mask=mask_inv)

dst = cv.add(img3_bg, img4_fg)
img3[0:rows, 0:cols] = dst

cv.imshow('',img3)

cv.waitKey(0)
cv.destroyAllWindows()