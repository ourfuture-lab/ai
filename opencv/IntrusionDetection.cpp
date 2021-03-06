/*
本程序为openc在入侵检测中的应用demo，代码已经在Windows下调通
opencv版本为2.4.7
*/
#include <opencv2/opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <core/core.hpp>
#include <time.h>

using namespace cv;
using namespace std;


int main()
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

	CvCapture* pCapture = NULL;

	int warningNum = 0; //检测到入侵的次数
	int nFrmNum = 0;//帧计数
	int status = 0;//状态标志位

	//创建窗口
	cvNamedWindow("video", 1);
	cvNamedWindow("background",1);//背景
	cvNamedWindow("foreground",1);//前景
	//使窗口有序排列
	cvMoveWindow("video", 30, 0);
	cvMoveWindow("background", 360, 0);
	cvMoveWindow("foreground", 690, 0);

	//打开摄像头     从摄像头取出码流可以使海康、大唐等网络或者模拟摄像头

	if ( !(pCapture = cvCaptureFromCAM(-1)))
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
	while(frameImg = cvQueryFrame(pCapture))
	{
		nFrmNum++;

		//如果是第一帧，需要申请内存，并初始化
		if (nFrmNum == 1)
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
			cvConvert(frameGrayImg, backMat);
		}
		else /* 不是第一帧的就这样处理 */
		{
			//pFrImg为当前帧的灰度图
			cvCvtColor(frameImg, frameGrayImg, CV_BGR2GRAY);

			//pFrameMat为当前灰度矩阵
			cvConvert(frameGrayImg, frameMat);

			//pFrMat为前景图矩阵，当前帧减去背景图
			cvAbsDiff(frameMat, backMat, frontMat);

			//pFrMatB为二值化（0,1）的前景图
			cvThreshold(frontMat,frontMatB, 60, 1, CV_THRESH_BINARY);

			//将图像矩阵转化为图像格式，用以显示
			cvConvert(backMat, backImgTran);   
			cvConvert(frontMat, frontImgTran);  

			//显示图像
			cvShowImage("video", frameImg);
			cvShowImage("background", backImgTran); //显示背景
			cvShowImage("foreground", frontImgTran); //显示前景


			//以上是每抓取一帧都要做的工作，下面进行危险检测
			if (cvCountNonZero(frontMatB) > 5000 && status == 0) //表示是第一帧的异物大于1W个像数点
			{/* 则需要将当前帧存储为第一帧 */
				frameImg1 = cvCloneImage(frameImg);
				bg1 = cvCloneMat(frontMat);
				status = 1;      //继续采集第2帧
			}
			else if (cvCountNonZero(frontMatB) < 5000 && status == 1) // 表示第一帧的异物大于1W个像数点，而第二帧没有,则报警
			{
				cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				status = 0;
			}
			else if (cvCountNonZero(frontMatB) > 5000 && status == 1)// 表示第一帧和第二帧的异物都大于1W个像数点
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
			else if (cvCountNonZero(frontMatB) < 5000 && status == 2)//表示第一帧和第二帧的异物都大于1W个像数点,而第三帧没有
			{
				//报警
				cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				status = 0;
			}
			else if (cvCountNonZero(frontMatB) > 5000 && status == 2)//表示连续3帧的异物都大于1W个像数点
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

	cvReleaseCapture(&pCapture);


	waitKey(0);


	//销毁窗口



	return 0 ;

}
