#include <opencv2/core/core.hpp>
#include <opencv2/highgui//highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace cv;

int dfddmain()
{

	CascadeClassifier mFaceDetector;
	//载入人脸特征分类器文件，可以从opencv的安装目录中找到
	mFaceDetector.load( "haarcascade_frontalface_alt.xml" );

	//打开视频文件
	Mat img = imread("h2.jpg");
	Mat mElabImage;//备份frame图像
	
	img.copyTo( mElabImage );
	//检测脸
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