import cv2 as cv
import numpy as np

from matplotlib import pyplot as plt

img = cv.imread('2.jpg')

px=img[100,100]
print px

blue=img[100,100,0]
print blue
#cv.imshow('1', img)

img[100,100] = [255,255,255]
print img[100,100]

print img.item(10,10,2)
img.itemset((10,10,2),200)
print img.item(10,10,2)

print img.shape
print img.size
print img.dtype

gray=cv.cvtColor(img, cv.COLOR_BGR2GRAY)
print gray.shape
print gray.size
print gray.dtype

#cv.imshow('',gray)

img2=img.copy()
face=img[ 447:691,196:495]
img2[0:244, 0:299]=face

#cv.imshow('img2', img2)

img3=img.copy()
b,g,r=cv.split(img3)
#cv.imshow('b',b)
#cv.imshow('g',g)
#cv.imshow('r',r)

b2=img[:,:,0]
#cv.imshow('b2', b2)

img4=img.copy()
img4[:,:,2]=0
#cv.imshow('img4', img4)


BLUE=[255,0,0]
img5=img.copy()
replicate = cv.copyMakeBorder(img5, 10,10,10,10,cv.BORDER_REPLICATE)
reflect=cv.copyMakeBorder(img5,10,10,10,10,cv.BORDER_REFLECT)
reflect101=cv.copyMakeBorder(img5,10,10,10,10,cv.BORDER_REFLECT_101)
wrap=cv.copyMakeBorder(img5,10,10,10,10,cv.BORDER_WRAP)
constant=cv.copyMakeBorder(img5, 10,10,10,10,cv.BORDER_CONSTANT, value=BLUE)

plt.subplot(231), plt.imshow(img5, 'gray'), plt.title('original')
#plt.subplot(232), plt.imshow(replicate, 'replicate'),plt.title('replicate')


plt.show()




cv.waitKey(0)
