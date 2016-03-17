#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//ָ�뷽ʽ��������
void colorReducePtr(Mat &inputImg, Mat &outputImg, int div);
//��������ʽ��������
void colorReduceIterator(Mat &inputImg, Mat &outputImg, int div);
//��̬��ַ����
void colorReduceDynamic(Mat &inputImg, Mat &outputImg, int div);

int main()
{
	Mat srcImg = imread("2.jpg");
	imshow("��ԭʼͼ��", srcImg);

	//��ԭʼͼ��Ĳ������������Ч��ͼ
	Mat dstImgPtr, dstImgIt, dstImgDy;
	dstImgPtr.create(srcImg.rows, srcImg.cols, srcImg.type());
	dstImgIt.create(srcImg.rows, srcImg.cols, srcImg.type());
	dstImgDy.create(srcImg.rows, srcImg.cols, srcImg.type());

	double time0;
	//********************ָ�뷽ʽ****************************
	//��¼��ʼʱ��
	time0 = static_cast<double>(getTickCount());
	//������ɫ�ռ�����������ָ�룩
	colorReducePtr(srcImg, dstImgPtr, 32);
	//��������ʱ��
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout<<"ָ�뷽ʽ����ʱ��Ϊ��"<<time0<<"��"<<endl;
	//��ʾЧ��ͼ
	imshow("��ָ��Ч��ͼ��", dstImgPtr);
	//********************ָ�뷽ʽ****************************


	//********************��������ʽ****************************
	time0 = static_cast<double>(getTickCount());
	//������ɫ�ռ�����������ָ�룩
	colorReduceIterator(srcImg, dstImgIt, 32);
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout<<"��������ʽ����ʱ��Ϊ��"<<time0<<"��"<<endl;
	//��ʾЧ��ͼ
	imshow("��������Ч��ͼ��", dstImgIt);
	//********************��������ʽ****************************


	//********************��̬��ַ��ʽ****************************
	time0 = static_cast<double>(getTickCount());
	//������ɫ�ռ�����������ָ�룩
	colorReduceDynamic(srcImg, dstImgDy, 32);
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout<<"��̬��ַ��ʽ����ʱ��Ϊ��"<<time0<<"��"<<endl;
	//��ʾЧ��ͼ
	imshow("����̬��ַЧ��ͼ��", dstImgDy);
	//********************��̬��ַ��ʽ****************************



	waitKey();

}


//��ָ���������
void colorReducePtr(Mat &inputImg, Mat &outputImg, int div)
{
	//����Դͼ����ʱͼ��
	outputImg = inputImg.clone();
	//����
	int rowNumber = outputImg.rows;
	//���� * ͨ���� = ÿһ�е�Ԫ�ظ���
	int colNumber = outputImg.cols * outputImg.channels();

	//˫��ѭ����������������ֵ
	for (int i = 0; i < rowNumber; i++)//��ѭ��
	{
		uchar* data = outputImg.ptr<uchar>(i);//��ȡ��i�е��׵�ַ

		for (int j = 0; j < colNumber; j++)//��ѭ��
		{
			//��ʼ����ÿ������
			data[j] = data[j]/div*div + div/2;
		}
	}
}

//�õ�������ʽ
void colorReduceIterator(Mat &inputImg, Mat &outputImg, int div)
{
	outputImg = inputImg.clone();
	//��ʼλ�õ�����
	Mat_<Vec3b>::iterator it = outputImg.begin<Vec3b>();
	//��ֹλ�õ�����
	Mat_<Vec3b>::iterator itend = outputImg.end<Vec3b>();

	//��ȡ��ɫͼ������
	for (;it != itend; it++)
	{
		(*it)[0] = (*it)[0]/div * div + div /2;
		(*it)[1] = (*it)[1]/div * div + div /2;
		(*it)[2] = (*it)[2]/div * div + div /2;
	}

}

//�ö�̬��ַ����
void colorReduceDynamic(Mat &inputImg, Mat &outputImg, int div)
{
	outputImg = inputImg.clone();
	int rowNumber = outputImg.rows;
	int colNumber = outputImg.cols;

	for (int i = 0; i < rowNumber; i++)
	{
		for (int j = 0; j < colNumber; j++)
		{
			//��ɫͨ��
			outputImg.at<Vec3b>(i,j)[0] = 
				outputImg.at<Vec3b>(i,j)[0]/div*div + div/2;
			//��ɫͨ��
			outputImg.at<Vec3b>(i,j)[1] = 
				outputImg.at<Vec3b>(i,j)[1]/div*div + div/2;
			//��ɫͨ��
			outputImg.at<Vec3b>(i,j)[2] = 
				outputImg.at<Vec3b>(i,j)[2]/div*div + div/2;
		}
	}
}