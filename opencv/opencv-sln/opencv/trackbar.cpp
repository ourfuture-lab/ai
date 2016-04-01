#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#define WINDOW_NAME "【线性混合示例】"

const int g_nMaxAlphaValue = 100;
int g_nAlphaValueSlider;
double g_dAlphaValue;
double g_dBetaValue;

Mat g_srcImg1, g_srcImg2, g_dstImg;

///*响应滑动条的回调函数*/
void on_Trackbar(int, void *)
{
	g_dAlphaValue =(double)g_nAlphaValueSlider/g_nMaxAlphaValue;
	g_dBetaValue =(double)(1.0 - g_dAlphaValue);

	//根据alpha和beta值进行线性混合
	addWeighted(g_srcImg1, g_dAlphaValue, g_srcImg2, g_dBetaValue, 0, g_dstImg);

	imshow(WINDOW_NAME, g_dstImg);
}

int main66()
{
	g_srcImg1 = imread("2.jpg");
	g_srcImg2 = imread("2.png");

	if (!g_srcImg1.data)
	{
		printf("读取第1幅图片错误，请确定目录下是否有图片");
		return -1;
	}
	if (!g_srcImg2.data)
	{
		printf("读取第2幅图片错误，请确定目录下是否有图片");
		return -1;
	}

	//滑动条初始值为70
	g_nAlphaValueSlider = 70;
	//创建窗体
	namedWindow(WINDOW_NAME, 1);

	//创建滑动条
	char TrackbarName[50];
	sprintf(TrackbarName,"透明值 %d", g_nMaxAlphaValue );
	createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValueSlider,
		g_nMaxAlphaValue, on_Trackbar);

	on_Trackbar(g_nAlphaValueSlider, 0);

	waitKey();

	return 0;
}

