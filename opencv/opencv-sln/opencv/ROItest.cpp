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

	//����һ��Mat���Ͳ���������ROI����
	Mat roiImg = srcImg(Rect(220, 220, logo.cols, logo.rows));
	
	Mat roiImg2 = srcImg(Rect(320, 320, logo.cols, logo.rows));

	//������Ĥ�������ǻҶ�ͼ����0Ϊ�Ҷ�ͼ
	Mat mask = imread("logo.jpg", 0);

	//����Ĥ���Ƶ�ROI��roiΪһС�飬��logo���Ƶ�roiʱ������mask��Ĥ
	logo.copyTo(roiImg, mask);
	logo.copyTo(roiImg2);

	//��ʾ���
	imshow("��Ч��ͼ�����ϱ�����Ĥ���±߲�����Ĥ", srcImg);

	waitKey();
	return 0;
}