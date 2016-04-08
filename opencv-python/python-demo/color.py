import cv2 as cv
import numpy as np

cap=cv.VideoCapture(0)

while(True):
    ret, frame = cap.read()

    hsv=cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    lower_blue=np.array([100, 90, 90])
    upper_blue=np.array([179, 255, 255])

    lower_red=np.array([160, 50, 50])
    upper_red=np.array([179, 255, 255])

    mask_blue=cv.inRange(hsv, lower_blue, upper_blue)
    mask_red=cv.inRange(hsv, lower_red, upper_red)

    res_blue=cv.bitwise_and(frame, frame, mask=mask_blue)
    res_red=cv.bitwise_and(frame, frame, mask=mask_red)

    cv.imshow('frame', frame)
    cv.imshow('res_blue', res_blue)
    cv.imshow('mask_blue', mask_blue)
    cv.imshow('res_red', res_red)

    k=cv.waitKey(5)&0xFF
    if k==27:
        break

cv.destroyAllWindows()