import numpy as np
import cv2

img = cv2.imread('1.jpg')

cv2.namedWindow('img', cv2.WINDOW_NORMAL)
cv2.imshow('img', img)
k = cv2.waitKey(0)
cv2.destroyAllWindows()
if k == 27: #esc key
    pass
elif k == ord('s'):#s key
    cv2.imwrite('a.jpg', img)
