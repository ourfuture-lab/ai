/*
�ٲɼ���1��ͼ��ʱ������ݵ�1֡�Ĵ�С��Ϣ���о���ͼ��ĳ�ʼ�������ҽ���1֡ͼ����лҶȻ�������ת��Ϊ������Ϊ����ͼ�񼰾���
�ڲ��ǵ�1��ͼ��ʱ����ѵ�ǰ֡���лҶȻ�������ת��Ϊ������Ϊ��ǰ֡��ͼ�񼰾���
���õ�ǰ֡��ͼ�����ͱ���֡��ͼ������������ǰ��ͼ���󲢶�����ж�ֵ���Ա�������뱳��֡���ϴ�����ظ�����Ҳ���Ƕ�ֵ������ĸ�����
�ܵ���1֡���������1W������������Ҫ����ǰ֡�洢Ϊ��1֡�����ҽ�ϵͳ��״̬תΪ1�����ɼ���2֡����
�ݵ�1֡�͵�2֡�����ﶼ����1W��������ʱ������ǰ֡�洢Ϊ��2֡��ͨ���жϵ�1֡�͵�2֡�Ĳ�ֵ��ȷ����֡�Ƿ���������������ϵͳ״̬תΪ2�����ɼ�����֡�������������򱨾�������ϵͳ״̬תΪ0�����ɼ�����֡����
�޵���1֡�͵�2֡�����ﶼ����1W��������,����3֡û��ʱ�򱨾���
��������3֡�����ﶼ����1W��������ʱ������ǰ֡�洢Ϊ����֡��ͨ���жϵڶ�֡�͵���֡�Ĳ�ֵ��ȷ����֡�Ƿ��������������򽫸��±��������������򱨾���Ȼ���ϵͳ״̬תΪ0�����ɼ�����֡����
*/
#include <opencv2/opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <core/core.hpp>
#include <time.h>

using namespace cv;
using namespace std;

#define PIXEL_CHANGE 20000  //֡�仯


int main88()
{
	IplImage* frameImg = NULL;//��Ƶ��ȡ��һ֡
	IplImage* frameImg1 = NULL;//��һ֡
	IplImage* frameImg2 = NULL;//�ڶ�֡
	IplImage* frameImg3 = NULL;//����֡

	IplImage* frameGrayImg = NULL;//��ǰ֡�ĻҶ�ͼ
	IplImage* backGrayImg = NULL;//��ǰ�����ĻҶ�ͼ
	IplImage* backImgTran = NULL;//��ǰ�����������ͼ��
	IplImage* frontImgTran = NULL;     //��ǰǰ���������ͼ��

	CvMat* frameMat = NULL; //pFrameMatΪ��ǰ�ҶȾ���
	CvMat* frontMat = NULL;//pFrMatΪ��ǰǰ��ͼ���󣬵�ǰ֡��ȥ����ͼ
	CvMat* bg1 = NULL;
	CvMat* bg2 = NULL;
	CvMat* bg3 = NULL;
	CvMat* frontMatB = NULL;     //pFrMatBΪ��ֵ����0,1����ǰ��ͼ
	CvMat* backMat = NULL;
	CvMat* zeroMat = NULL;    //���ڼ���bg1 - bg2 ��ֵ
	CvMat* zeroMatB = NULL;//���ڼ���pZeroMat��ֵ�������ж��ж��ٸ������ʱ���� 

	CvCapture* capture = NULL;

	int warningNum = 0; //��⵽���ֵĴ���
	int frameCount = 0;//֡����
	int status = 0;//״̬��־λ

	//��������-*
	cvNamedWindow("video", 1);
	cvNamedWindow("background",1);//����
	cvNamedWindow("foreground",1);//ǰ��
	//ʹ������������
	cvMoveWindow("video", 30, 0);
	cvMoveWindow("background", 690, 0);
	cvMoveWindow("foreground", 30, 510);

	//������ͷ

	if ( !(capture = cvCaptureFromCAM(0)))
	{
		cout<<"can not open camera.\n"<<endl;
		return -2;
	}
	else
	{
		cout<<"open camera��"<<endl;
	}

	time_t start;
	time(&start);        //time() ���ش�1970��1��1��00��00��00��ʼ���������ڵ���������10Ϊ���֣���
	cout<<"start at: "<<start<<endl;

	//��֡��ȡ��Ƶ
	while(frameImg = cvQueryFrame(capture))
	{
		frameCount++;

		//����ǵ�һ֡����Ҫ�����ڴ棬����ʼ��
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
			//ת���ɵ�ͨ��ͼ���ٴ���
			cvCvtColor(frameImg, backGrayImg, CV_BGR2GRAY);
			//ת��Ϊ����
			cvConvert(frameGrayImg, backMat);//��ʱbackMat�ǿյ�
		}
		else /* ���ǵ�һ֡�ľ��������� */
		{
			//frameImgΪ��ǰ֡�ĻҶ�ͼ
			cvShowImage("video", frameImg);
			cvCvtColor(frameImg, frameGrayImg, CV_BGR2GRAY);
			cvShowImage("video", frameImg);

			//frameMatΪ��ǰ�ҶȾ���
			cvConvert(frameGrayImg, frameMat);

			//pFrMatΪǰ��ͼ���󣬵�ǰ֡��ȥ����ͼ
			cvAbsDiff(frameMat, backMat, frontMat);

			//pFrMatBΪ��ֵ����0,1����ǰ��ͼ
			cvThreshold(frontMat,frontMatB, 60, 1, CV_THRESH_BINARY);

			//��ͼ�����ת��Ϊͼ���ʽ��������ʾ
			cvShowImage("background", backImgTran); 
			cvConvert(backMat, backImgTran);   
			cvShowImage("background", backImgTran); 

			cvShowImage("foreground", frontImgTran);
			cvConvert(frontMat, frontImgTran);  
			cvShowImage("foreground", frontImgTran);

			//��ʾͼ��
			cvShowImage("video", frameImg);
			cvShowImage("background", backImgTran); //��ʾ����
			cvShowImage("foreground", frontImgTran); //��ʾǰ��


			//������ÿץȡһ֡��Ҫ���Ĺ������������Σ�ռ��
			if (cvCountNonZero(frontMatB) > PIXEL_CHANGE && status == 0) //��ʾ�ǵ�һ֡���������1W��������
			{/* ����Ҫ����ǰ֡�洢Ϊ��һ֡ */
				frameImg1 = cvCloneImage(frameImg);
				bg1 = cvCloneMat(frontMat);
				status = 1;      //�����ɼ���2֡
			}
			else if (cvCountNonZero(frontMatB) < PIXEL_CHANGE && status == 1) // ��ʾ��һ֡���������1W�������㣬���ڶ�֡û��,�򱨾�
			{
				cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				status = 0;
			}
			else if (cvCountNonZero(frontMatB) > PIXEL_CHANGE && status == 1)// ��ʾ��һ֡�͵ڶ�֡�����ﶼ����1W��������
			{
				frameImg2 = cvCloneImage(frameImg);
				bg2 = cvCloneMat(frontMat);

				cvAbsDiff(bg1, bg2, zeroMat);
				cvThreshold(zeroMat,zeroMatB, 20, 1, CV_THRESH_BINARY);
				if (cvCountNonZero(zeroMatB) > 3000 ) //��ʾ���ǲ������������Ļ�Ҫ����
				{
					cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
					status = 0;	
				}
				else
				{
					status = 2;                   //�����ɼ���3֡
				}
			}
			else if (cvCountNonZero(frontMatB) < PIXEL_CHANGE && status == 2)//��ʾ��һ֡�͵ڶ�֡�����ﶼ����1W��������,������֡û��
			{
				//����
				cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				status = 0;
			}
			else if (cvCountNonZero(frontMatB) > PIXEL_CHANGE && status == 2)//��ʾ����3֡�����ﶼ����1W��������
			{
				frameImg3 = cvCloneImage(frameImg);
				bg3 = cvCloneMat(frontMat);

				cvAbsDiff(bg2, bg3, zeroMat);
				cvThreshold(zeroMat,zeroMatB, 20, 1, CV_THRESH_BINARY);
				if (cvCountNonZero(zeroMatB) > 3000 ) //��ʾ���ǲ������������Ļ�Ҫ����
				{
					cout<<"NO."<<warningNum++<<" warning!!!!"<<endl;
				}
				else //��ʾbg2,bg3����
				{
					cvReleaseMat(&backMat);
					backMat = cvCloneMat(frameMat); //���±���
				}
				status = 0;                //������һ�βɼ�����
			}

			//����а����¼���������ѭ��
			//�˵ȴ�ҲΪcvShowImage�����ṩʱ�������ʾ
			//�ȴ�ʱ����Ը���CPU�ٶȵ���
			if ( cvWaitKey(2) >= 0 )
				break;
		}/* The End of the else */


	}

	cvDestroyWindow("video");
	cvDestroyWindow("background");
	cvDestroyWindow("foreground");

	//�ͷ�ͼ��;���
	cvReleaseImage(&frameGrayImg);
	cvReleaseImage(&backGrayImg);

	cvReleaseMat(&frameMat);
	cvReleaseMat(&frontMat);
	cvReleaseMat(&backMat);

	cvReleaseCapture(&capture);


	waitKey(0);


	//���ٴ���



	return 0 ;

}
