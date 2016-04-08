import cv2 as cv
import numpy as np

def nothing():
    pass

cap=cv.VideoCapture(0)

cv.namedWindow('img')

cv.createTrackbar('LowH', 'img', 0, 179, nothing)
cv.createTrackbar('LowS', 'img', 0, 255, nothing)
cv.createTrackbar('LowV', 'img', 0, 255, nothing)

cv.createTrackbar('HighH', 'img', 0, 179, nothing)
cv.createTrackbar('HighS', 'img', 0, 255, nothing)
cv.createTrackbar('HighV', 'img', 0, 255, nothing)

while(True):
    ret, frame = cap.read()

    hsv=cv.cvtColor(frame, cv.COLOR_BGR2HSV)



    LowH=cv.getTrackbarPos('LowH', 'img')
    LowS=cv.getTrackbarPos('LowS', 'img')
    LowV=cv.getTrackbarPos('LowV', 'img')
    HighH=cv.getTrackbarPos('HighH', 'img')
    HighS=cv.getTrackbarPos('HighS', 'img')
    HighV=cv.getTrackbarPos('HighV', 'img')


    lower=np.array([LowH, LowS, LowV])
    upper=np.array([HighH, HighS, HighV])

    mask = cv.inRange(hsv, lower, upper)

    res=cv.bitwise_and(frame, frame, mask=mask)

    cv.imshow('img2', res)

    k=cv.waitKey(500)&0xFF
    if k==27:
        break

cv.destroyAllWindows()