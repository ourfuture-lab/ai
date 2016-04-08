from cv2 import *
import numpy as np

img=imread('1.jpg')
x=0
while(x<1.5):
    x+=0.01
    res=resize(img, None, fx=x, fy=x, interpolation=INTER_CUBIC)
    imshow('res', res)
    if waitKey(1) & 0xFF ==27:
        break

rows, cols, channels = res.shape

x=0
y=0
while(x<100):
    x+=0.5;
    y+=0.2
    M=np.float32([[1,0,x],[0,1,y]])
    dst=warpAffine(res, M, (cols, rows))
    imshow('res', dst)
    if waitKey(1) & 0xFF == 27:
        break

while(x>0):
    x-=0.5
    y-=0.2
    M=np.float32([[1,0,x],[0,1,y]])
    dst=warpAffine(res, M, (cols, rows))
    imshow('res', dst)
    if waitKey(1) & 0xFF == 27:
        break

x=0
while(x<360):
    x+=0.5
    M=getRotationMatrix2D((cols/2, rows/2), x, 1)
    dst=warpAffine(res, M, (cols, rows))
    imshow('res', dst)
    if waitKey(1) & 0xFF == 27:
        break


pts1=np.float32([[50,50],[200,50],[50,200]])
pts2=np.float32([[10,100],[200,50],[100,250]])
M=getAffineTransform(pts1, pts2)

dst=warpAffine(img, M, (cols, rows))

imshow('dst', dst)



waitKey(0)
destroyAllWindows()