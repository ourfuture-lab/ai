import cv2
import numpy as np
from matplotlib import pyplot as plt
img = cv2.imread('1.jpg',0)
img2 = img.copy()
template = cv2.imread('2.jpg',0)
w, h = template.shape[::-1]

methods = ['cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',
'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 'cv2.TM_SQDIFF_NORMED']
for meth in methods:
img = img2.copy()
#exec �������ִ�д������ַ������ļ��е�Python ��䡣
# ���磬���ǿ���������ʱ����һ������Python ������ַ�����Ȼ��ʹ��exec ���ִ����Щ��䡣
#eval �����������洢���ַ����е���ЧPython ���ʽ
method = eval(meth)

res = cv2.matchTemplate(img,template,method)
min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
# ʹ�ò�ͬ�ıȽϷ������Խ���Ľ��Ͳ�ͬ
if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
    top_left = min_loc
else:
    top_left = max_loc
bottom_right = (top_left[0] + w, top_left[1] + h)
cv2.rectangle(img,top_left, bottom_right, 255, 2)
plt.subplot(121),plt.imshow(res,cmap = 'gray')
plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(img,cmap = 'gray')
plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
plt.suptitle(meth)
plt.show()