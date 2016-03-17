/*
��demo������Ƶ�м����������Windows���ѵ�ͨ���������쳣��������ֱ��ʹ��
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

	//����Ƶ�ļ�
	//VideoCapture capture("21.avi");
	//0 open default camera
	VideoCapture capture(0);
	//�����Ƶ�Ƿ��
	if(!capture.isOpened())
		return 1;

	// �õ�֡��
	//double rate= capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	Mat frame; // ���ڵ���Ƶ֡
	Mat mElabImage;//����frameͼ��

	// ��֮֡��ļ��ʱ��
	//int delay= 1000/rate;
	// ѭ���������е�֡
	while (!stop) {
		// ����һ֡
		if (!capture.read(frame))
			break;
		//frame.copyTo( mElabImage );
		//�����
		vector< Rect > faceVec;
		mFaceDetector.detectMultiScale( frame, faceVec );

		for(int i=0; i<faceVec.size(); i++ )
		{
			rectangle( frame, faceVec[i], CV_RGB(255,0,0), 1 );
		}

		//�ڴ�������ʾͼ��
		imshow("Extracted Frame",frame);
		// �������ֹͣ��Ƶ����
		//if (waitKey(delay)>=0)
		//    stop= true;
		waitKey(0);
	}
	// �ر���Ƶ�ļ�
	capture.release();
	return 0;
}