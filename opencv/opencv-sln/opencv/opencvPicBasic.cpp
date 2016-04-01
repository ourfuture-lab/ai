/*
opencv图像基本操作
*/

/************图像载入************/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main2()
{
	
	Mat scrImg = imread("1.jpg");
	imshow("【原始图】", scrImg);
	waitKey(0);

	return 0;
}

/************图像腐蚀************/


int main3()
{
	Mat srcImg = imread("1.jpg");
	imshow("【原始图】腐蚀操作", srcImg);
	Mat element = getStructuringElement(MORPH_RECT, Size(15,15));
	Mat dstImg;
	erode(srcImg, dstImg, element);
	imshow("【效果图】腐蚀操作", dstImg);
	waitKey(0);
	
	return 0;
}

/************图像模糊滤镜************/
int main4()
{
	Mat srcImg = imread("1.jpg");
	imshow("【原始图】均值滤波", srcImg);
	
	Mat dstImg;
	blur(srcImg, dstImg, Size(7,7));
	imshow("【效果图】均值滤波", dstImg);

	waitKey(0);
	return 0;
}


/************图像边缘检测************/
/*
载入图像
将其转成灰度
再用blur进行图像模糊进行降噪
然后用canny函数进行边缘检测
*/
int main5()
{
	Mat srcImg = imread("1.jpg");
	imshow("【原始图】Canny边缘检测", srcImg);
	Mat dstImg, edge, grayImg;
	dstImg.create(srcImg.size(), srcImg.type());
	//转为灰度图像
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	//使用3X3内核降噪
	imshow("【灰度图】Canny边缘检测", grayImg);
	blur(grayImg, edge, Size(3,3));
	imshow("【降噪灰度图】Canny边缘检测", edge);
	//运行Canny算子
	Canny(edge, edge, 3, 9, 3);

	imshow("【效果图】Canny边缘检测", edge);

	waitKey(0);
	return 0;
}


