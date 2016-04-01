/*
①采集第1幅图像时，则根据第1帧的大小信息进行矩阵、图像的初始化，并且将第1帧图像进行灰度化处理，并转化为矩阵，作为背景图像及矩阵；
②不是第1幅图像时，则把当前帧进行灰度化处理，并转化为矩阵，作为当前帧的图像及矩阵。
③用当前帧的图像矩阵和背景帧的图像矩阵做差算出前景图矩阵并对其进行二值化以便计算它与背景帧差别较大的像素个数，也就是二值化后零的个数。
④当第1帧的异物大于1W个像数点则需要将当前帧存储为第1帧，并且将系统的状态转为1（即采集第2帧）；
⑤第1帧和第2帧的异物都大于1W个像数点时，将当前帧存储为第2帧，通过判断第1帧和第2帧的差值来确定两帧是否连续，若连续则将系统状态转为2（即采集第三帧），若不连续则报警，并把系统状态转为0（即采集背景帧）；
⑥当第1帧和第2帧的异物都大于1W个像数点,而第3帧没有时则报警；
⑦若连续3帧的异物都大于1W个像数点时，将当前帧存储为第三帧，通过判断第二帧和第三帧的差值来确定两帧是否连续，若连续则将更新背景，若不连续则报警，然后把系统状态转为0（即采集背景帧）。
*/
#include <opencv2/opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <core/core.hpp>
#include <time.h>

using namespace cv;
using namespace std;

#define PIXEL_CHANGE 20000  //帧变化


int main88()
{
	IplImage* frameImg = NULL;//视频截取的一帧
	IplImage* frameImg1 = NULL;//第一帧
	IplImage* frameImg2 = NULL;//第二帧
	IplImage* frameImg3 = NULL;//第三帧

	IplImage* frameGrayImg = NULL;//当前帧的灰度图
	IplImage* backGrayImg = NULL;//当前背景的灰度图
	IplImage* backImgTran = NULL;//当前背景处理过的图像
	IplImage* frontImgTran = NULL;     //当前前景处理过的图像

	CvMat* frameMat = NULL; //pFrameMat为当前灰度矩阵
	CvMat* frontMat = NULL;//pFrMat为当前前景图矩阵，当前帧减去背景图
	CvMat* bg1 = NULL;
	CvMat* bg2 = NULL;
	CvMat* bg3 = NULL;
	CvMat* frontMatB = NULL;     //pFrMatB为二值化（0,1）的前景图
	CvMat* backMat = NULL;
	CvMat* zeroMat = NULL;    //用于计算bg1 - bg2 的值
	CvMat* zeroMatB = NULL;//用于计算pZeroMat阈值化后来判断有多少个零的临时矩阵 

	CvCapture* capture = NULL;

	int warningNum = 0; //检测到入侵的次数
	int frameCount = 0;//帧计数
	int status = 0;//状态标志位

	//创建窗口-*
	cvNamedWindow("video", 1);
	cvNamedWindow("background",1);//背景
	cvNamedWindow("foreground",1);//前景
	//使窗口有序排列
	cvMoveWindow("video", 30, 0);
	cvMoveWindow("background", 690, 0);
	cvMoveWindow("foreground", 30, 510);

	//打开摄像头

	if ( !(capture = cvCaptureFromCAM(0)))
	{
		cout<<"can not open camera.\n"<<endl;
		return -2;
	}
	else
	{
		cout<<"open camera！"<<endl;
	}

	time_t start;
	time(&start);        //time() 返回从1970年1月1号00：00：00开始以来到现在的秒数（有10为数字）。
	cout<<"start at: "<<start<<endl;

	//逐帧读取视频
	while(frameImg = cvQueryFrame(capture))
	{
		frameCount++;

		//如果是第一帧，需要申请内存，并初始化
		if (frameCount == 1)
		{
			backGrayImg = cvCreateImage(cvSize(frameImg->width, frameImg->height), IPL_DEPTH_8U,1);
			frameGrayImg = cvCreateImage(cvSize(frameImg->width, frameImg->height), IPL_DEPTH_8U,1);
			backImgTran = cvCreateImage(cvSize(frameImg->width,frameImg->height), IPL_DEPTH_8U,1);
			frontImgTran = cvCreateImage(cvSize(frameImg->width,frameImg->height), IPL_DEPTH_8U,1);

			backMat = cvCreateMat(frameImg->height, frameImg->width, CV_32FC1);
			zeroMat = cvCreateMat(frameImg->height, frameImg->width, CV_32FC1);
			frontMat = cvCreateMat(frameImg->height, frameImg->width, CV_32FC1);
			frontMatB = cvCreateMat(frameImg->height, frameImg->width, CV_8UC1);
			zeroMatB = cvCreateMat(frameImg->height, frameImg->width, CV_8UC1);
			frameMat = cvCreateMat(frameImg->height, frameImg->width, CV_32FC1);
			cvZero(zeroMat);
			//转化成单通道图像再处理
			cvCvtColor(frameImg, backGrayImg, CV_BGR2GRAY);
			//转换为矩阵
			cvConvert(frameGrayImg, backMat);//此时backMat是空的
		}
		else /* 不是第一帧的就这样处理 */
		{
			//frameImg为当前帧的灰度图
			cvShowImage("video", frameImg);
			cvCvtColor(frameImg, frameGrayImg, CV_BGR2GRAY);
			cvShowImage("video", frameImg);

			//frameMat为当前灰度矩阵
			cvConvert(frameGrayImg, frameMat);

			//pFrMat为前景图矩阵，当前帧减去背景图
			cvAbsDiff(frameMat, backMat, frontMat);

			//pFrMatB为二值化（0,1）的前景图
			cvThreshold(frontMat,frontMatB, 60, 1, CV_THRESH_BINARY);

			//将图像矩阵转化为图像格式，用以显示
			cvShowImage("background", backImgTran); 
			cvConvert(backMat, backImgTran);   
			cvShowImage("background", backImgTran); 

			cvShowImage("foreground", frontImgTran);
			cvConvert(frontMat, frontImgTran);  
			cvShowImage("foreground", frontImgTran);

			//显示图像
			cvShowImage("video", frameImg);
			cvShowImage("background", backImgTran); //显示背景
			cvShowImage("foreground", frontImgTran); //显示前景


			//以上是每抓取一帧都要做的工作，下面进行危险检测
			if (cvCountNonZero(frontMatB) > PIXEL_CHANGE && status == 0) //表示是第一帧的异物大于1W个像数点
			{/* 则需要将当前帧存储为第一帧 */
				frameImg1 = cvCloneImage(frameImg);
				bg1 = cvCloneMat(frontMat);
				status = 1;      //继续采集第2帧
			}
			else if (cvCountNonZero(frontMatB) < PIXEL_CHANGE && status == 1) // 表示第一帧的异物大于1W个像数点，而第二帧没有,则报警
			{
				cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				status = 0;
			}
			else if (cvCountNonZero(frontMatB) > PIXEL_CHANGE && status == 1)// 表示第一帧和第二帧的异物都大于1W个像数点
			{
				frameImg2 = cvCloneImage(frameImg);
				bg2 = cvCloneMat(frontMat);

				cvAbsDiff(bg1, bg2, zeroMat);
				cvThreshold(zeroMat,zeroMatB, 20, 1, CV_THRESH_BINARY);
				if (cvCountNonZero(zeroMatB) > 3000 ) //表示他们不连续，这样的话要报警
				{
					cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
					status = 0;	
				}
				else
				{
					status = 2;                   //继续采集第3帧
				}
			}
			else if (cvCountNonZero(frontMatB) < PIXEL_CHANGE && status == 2)//表示第一帧和第二帧的异物都大于1W个像数点,而第三帧没有
			{
				//报警
				cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				status = 0;
			}
			else if (cvCountNonZero(frontMatB) > PIXEL_CHANGE && status == 2)//表示连续3帧的异物都大于1W个像数点
			{
				frameImg3 = cvCloneImage(frameImg);
				bg3 = cvCloneMat(frontMat);

				cvAbsDiff(bg2, bg3, zeroMat);
				cvThreshold(zeroMat,zeroMatB, 20, 1, CV_THRESH_BINARY);
				if (cvCountNonZero(zeroMatB) > 3000 ) //表示他们不连续，这样的话要报警
				{
					cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				}
				else //表示bg2,bg3连续
				{
					cvReleaseMat(&backMat);
					backMat = cvCloneMat(frameMat); //更新背景
				}
				status = 0;                //进入下一次采集过程
			}

			//如果有按键事件，则跳出循环
			//此等待也为cvShowImage函数提供时间完成显示
			//等待时间可以根据CPU速度调整
			if ( cvWaitKey(2) >= 0 )
				break;
		}/* The End of the else */


	}

	cvDestroyWindow("video");
	cvDestroyWindow("background");
	cvDestroyWindow("foreground");

	//释放图像和矩阵
	cvReleaseImage(&frameGrayImg);
	cvReleaseImage(&backGrayImg);

	cvReleaseMat(&frameMat);
	cvReleaseMat(&frontMat);
	cvReleaseMat(&backMat);

	cvReleaseCapture(&capture);


	waitKey(0);


	//销毁窗口



	return 0 ;

}
