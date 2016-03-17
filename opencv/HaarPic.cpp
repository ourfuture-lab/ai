/*
��demo����ͼƬ�м����������Windows���ѵ�ͨ���������쳣��������ֱ��ʹ��
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui//highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace cv;

int ddmain()
{

	CascadeClassifier mFaceDetector;
	//�����ĸ����������������ļ������Դ�opencv�İ�װĿ¼���ҵ�
	mFaceDetector.load( "haarcascade_frontalface_alt.xml" );

	//����Ƶ�ļ�
	Mat img = imread("h1.jpg");
	Mat mElabImage;//����frameͼ��
	
	img.copyTo( mElabImage );
	//�����
	vector< Rect> faceVec;
	mFaceDetector.detectMultiScale( img, faceVec);
	printf("%d", faceVec.size());
	for(int i = 0; i<faceVec.size();i++)
	{
		rectangle( mElabImage, faceVec[i], CV_RGB(255,0,0), 1 );
	}

	imshow("Extracted Frame",mElabImage);
	
	waitKey(0);
	return 0;
}