#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#define WINDOW_NAME "�����Ի��ʾ����"

const int g_nMaxAlphaValue = 100;
int g_nAlphaValueSlider;
double g_dAlphaValue;
double g_dBetaValue;

Mat g_srcImg1, g_srcImg2, g_dstImg;

///*��Ӧ�������Ļص�����*/
void on_Trackbar(int, void *)
{
	g_dAlphaValue =(double)g_nAlphaValueSlider/g_nMaxAlphaValue;
	g_dBetaValue =(double)(1.0 - g_dAlphaValue);

	//����alpha��betaֵ�������Ի��
	addWeighted(g_srcImg1, g_dAlphaValue, g_srcImg2, g_dBetaValue, 0, g_dstImg);

	imshow(WINDOW_NAME, g_dstImg);
}

int main66()
{
	g_srcImg1 = imread("2.jpg");
	g_srcImg2 = imread("2.png");

	if (!g_srcImg1.data)
	{
		printf("��ȡ��1��ͼƬ������ȷ��Ŀ¼���Ƿ���ͼƬ");
		return -1;
	}
	if (!g_srcImg2.data)
	{
		printf("��ȡ��2��ͼƬ������ȷ��Ŀ¼���Ƿ���ͼƬ");
		return -1;
	}

	//��������ʼֵΪ70
	g_nAlphaValueSlider = 70;
	//��������
	namedWindow(WINDOW_NAME, 1);

	//����������
	char TrackbarName[50];
	sprintf(TrackbarName,"͸��ֵ %d", g_nMaxAlphaValue );
	createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValueSlider,
		g_nMaxAlphaValue, on_Trackbar);

	on_Trackbar(g_nAlphaValueSlider, 0);

	waitKey();

	return 0;
}

