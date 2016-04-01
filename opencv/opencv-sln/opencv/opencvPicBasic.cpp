/*
opencvͼ���������
*/

/************ͼ������************/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main2()
{
	
	Mat scrImg = imread("1.jpg");
	imshow("��ԭʼͼ��", scrImg);
	waitKey(0);

	return 0;
}

/************ͼ��ʴ************/


int main3()
{
	Mat srcImg = imread("1.jpg");
	imshow("��ԭʼͼ����ʴ����", srcImg);
	Mat element = getStructuringElement(MORPH_RECT, Size(15,15));
	Mat dstImg;
	erode(srcImg, dstImg, element);
	imshow("��Ч��ͼ����ʴ����", dstImg);
	waitKey(0);
	
	return 0;
}

/************ͼ��ģ���˾�************/
int main4()
{
	Mat srcImg = imread("1.jpg");
	imshow("��ԭʼͼ����ֵ�˲�", srcImg);
	
	Mat dstImg;
	blur(srcImg, dstImg, Size(7,7));
	imshow("��Ч��ͼ����ֵ�˲�", dstImg);

	waitKey(0);
	return 0;
}


/************ͼ���Ե���************/
/*
����ͼ��
����ת�ɻҶ�
����blur����ͼ��ģ�����н���
Ȼ����canny�������б�Ե���
*/
int main5()
{
	Mat srcImg = imread("1.jpg");
	imshow("��ԭʼͼ��Canny��Ե���", srcImg);
	Mat dstImg, edge, grayImg;
	dstImg.create(srcImg.size(), srcImg.type());
	//תΪ�Ҷ�ͼ��
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	//ʹ��3X3�ں˽���
	imshow("���Ҷ�ͼ��Canny��Ե���", grayImg);
	blur(grayImg, edge, Size(3,3));
	imshow("������Ҷ�ͼ��Canny��Ե���", edge);
	//����Canny����
	Canny(edge, edge, 3, 9, 3);

	imshow("��Ч��ͼ��Canny��Ե���", edge);

	waitKey(0);
	return 0;
}


