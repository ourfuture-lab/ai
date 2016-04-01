#include "opencv2/opencv.hpp"
using namespace cv;

int main22()
{
	Mat srcImg = imread("2.jpg");
	Mat logo = imread("logo.jpg");

	if (!srcImg.data || !logo.data)
	{
		return false;
	}

	//定义一个Mat类型并给其设置ROI区域
	Mat roiImg = srcImg(Rect(220, 220, logo.cols, logo.rows));
	
	Mat roiImg2 = srcImg(Rect(320, 320, logo.cols, logo.rows));

	//加载掩膜（必须是灰度图），0为灰度图
	Mat mask = imread("logo.jpg", 0);

	//将掩膜复制到ROI，roi为一小块，将logo复制到roi时，利用mask掩膜
	logo.copyTo(roiImg, mask);
	logo.copyTo(roiImg2);

	//显示结果
	imshow("【效果图】，上边用掩膜，下边不用掩膜", srcImg);

	waitKey();
	return 0;
}