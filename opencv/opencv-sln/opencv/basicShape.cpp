#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

#define WINDOW_WIDTH 600

//��Բ
void DrawEllipse(Mat img, double angle)
{
	//angle����ת�Ƕ�
	//�߿�2
	int thickness = 2;
	//���ͣ���ͨ����
	int lineType = 8;

	//Scalar����ɫ��255,129,0Ϊ��ɫ

	ellipse(img,
		Point(WINDOW_WIDTH/2, WINDOW_WIDTH/2),
		Size(WINDOW_WIDTH/4, WINDOW_WIDTH/16),
		angle,
		0,
		360,
		Scalar(255,129,0),
		thickness,
		lineType);
}

//Բ
void DrawFilledCircle(Mat img, Point center)
{
	//�ߴ�Ϊ-1����ʵ�ĵ�
	int thickness = 1;
	int lineType = 8;

	circle(img, center,WINDOW_WIDTH/32, Scalar(0,0,255),thickness,lineType);
}

//�����
void DrawPolygo(Mat img)
{
	int lineType = 8;

	Point rootPoints[1][20];
	rootPoints[0][0] = Point (WINDOW_WIDTH /4 , 7 * WINDOW_WIDTH/8);
	rootPoints[0][1] = Point(3*WINDOW_WIDTH/4, 7*WINDOW_WIDTH/8);
	rootPoints[0][2] = Point(3*WINDOW_WIDTH/4, 13*WINDOW_WIDTH/16);
	rootPoints[0][3] = Point(11*WINDOW_WIDTH/16, 13*WINDOW_WIDTH/16);
	rootPoints[0][4] = Point(19*WINDOW_WIDTH/32, 3*WINDOW_WIDTH/8);
	rootPoints[0][5] = Point(3*WINDOW_WIDTH/4, 3*WINDOW_WIDTH/8);
	rootPoints[0][6] = Point(3*WINDOW_WIDTH/4, WINDOW_WIDTH/8);
	rootPoints[0][7] = Point(26*WINDOW_WIDTH/40, WINDOW_WIDTH/8);
	rootPoints[0][8] = Point(26*WINDOW_WIDTH/40, WINDOW_WIDTH/4);
	rootPoints[0][9] = Point(22*WINDOW_WIDTH/40, WINDOW_WIDTH/4);
	rootPoints[0][10] = Point(22*WINDOW_WIDTH/40, WINDOW_WIDTH/8);
	rootPoints[0][11] = Point(18*WINDOW_WIDTH/40, WINDOW_WIDTH/8);
	rootPoints[0][12] = Point(18*WINDOW_WIDTH/40, WINDOW_WIDTH/4);
	rootPoints[0][13] = Point(14*WINDOW_WIDTH/40, WINDOW_WIDTH/4);
	rootPoints[0][14] = Point(14*WINDOW_WIDTH/40, WINDOW_WIDTH/8);
	rootPoints[0][15] = Point(WINDOW_WIDTH/4, WINDOW_WIDTH/8);
	rootPoints[0][16] = Point(WINDOW_WIDTH/4, 3 * WINDOW_WIDTH/8);
	rootPoints[0][17] = Point(13* WINDOW_WIDTH/32, 3 * WINDOW_WIDTH/8);
	rootPoints[0][18] = Point(5* WINDOW_WIDTH/16, 13 * WINDOW_WIDTH/16);
	rootPoints[0][19] = Point(WINDOW_WIDTH/4, 13 * WINDOW_WIDTH/16);

	const Point* ppt[1] = {rootPoints[0]};
	int npt[] = {20};

	fillPoly(img, ppt, npt, 1, Scalar(255,255,255), lineType);
}

//��
void DrawLine(Mat img, Point start, Point end)
{
	int thinkness = 1;
	int lineType = 8;

	line(img,start, end, Scalar(0, 0, 255), thinkness,lineType);
}


int main11()
{
	//�հ�
	Mat img1 = Mat::zeros(WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3) ;
	Mat img2 = Mat::zeros(WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);
	
	//����Բ
	DrawEllipse(img1, 0);
	DrawEllipse(img1, 90);
	DrawEllipse(img1, 45);
	DrawEllipse(img1, 135);
	imshow("����Բ��", img1);

	//��Բ
	DrawFilledCircle(img2, Point(WINDOW_WIDTH/2, WINDOW_WIDTH/2));
	imshow("��Բ��", img2);

	//�������
	DrawPolygo(img2);
	imshow("������Ρ�", img2);

	//����
	DrawLine(img2, Point(20,20), Point(WINDOW_WIDTH -20, WINDOW_WIDTH - 20));
	imshow("���ߡ�", img2);

	waitKey();
	return 0;
}