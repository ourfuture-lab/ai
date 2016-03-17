/*
本demo用于视频中检测人脸，在Windows下已调通，代码做异常处理后可以直接使用
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui//highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace cv;

int main()
{

	CascadeClassifier mFaceDetector;

	mFaceDetector.load( "haarcascade_frontalface_alt.xml" );

	//打开视频文件
	//VideoCapture capture("21.avi");
	//0 open default camera
	VideoCapture capture(0);
	//检查视频是否打开
	if(!capture.isOpened())
		return 1;

	// 得到帧率
	//double rate= capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	Mat frame; // 现在的视频帧
	Mat mElabImage;//备份frame图像

	// 两帧之间的间隔时间
	//int delay= 1000/rate;
	// 循环播放所有的帧
	while (!stop) {
		// 读下一帧
		if (!capture.read(frame))
			break;
		//frame.copyTo( mElabImage );
		//检测脸
		vector< Rect > faceVec;
		mFaceDetector.detectMultiScale( frame, faceVec );

		for(int i=0; i<faceVec.size(); i++ )
		{
			rectangle( frame, faceVec[i], CV_RGB(255,0,0), 1 );
		}

		//在窗口中显示图像
		imshow("Extracted Frame",frame);
		// 按任意键停止视频播放
		//if (waitKey(delay)>=0)
		//    stop= true;
		waitKey(0);
	}
	// 关闭视频文件
	capture.release();
	return 0;
}