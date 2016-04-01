#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//指针方式访问像素
void colorReducePtr(Mat &inputImg, Mat &outputImg, int div);
//迭代器方式访问像素
void colorReduceIterator(Mat &inputImg, Mat &outputImg, int div);
//动态地址计算
void colorReduceDynamic(Mat &inputImg, Mat &outputImg, int div);

int main1()
{
	Mat srcImg = imread("2.jpg");
	imshow("【原始图像】", srcImg);

	//按原始图像的参数规格来创建效果图
	Mat dstImgPtr, dstImgIt, dstImgDy;
	dstImgPtr.create(srcImg.rows, srcImg.cols, srcImg.type());
	dstImgIt.create(srcImg.rows, srcImg.cols, srcImg.type());
	dstImgDy.create(srcImg.rows, srcImg.cols, srcImg.type());

	double time0;
	//********************指针方式****************************
	//记录起始时间
	time0 = static_cast<double>(getTickCount());
	//调用颜色空间缩减函数（指针）
	colorReducePtr(srcImg, dstImgPtr, 32);
	//计算运行时间
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout<<"指针方式调用时间为："<<time0<<"秒"<<endl;
	//显示效果图
	imshow("【指针效果图】", dstImgPtr);
	//********************指针方式结束****************************


	//********************迭代器方式****************************
	time0 = static_cast<double>(getTickCount());
	//调用颜色空间缩减函数（指针）
	colorReduceIterator(srcImg, dstImgIt, 32);
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout<<"迭代器方式调用时间为："<<time0<<"秒"<<endl;
	//显示效果图
	imshow("【迭代器效果图】", dstImgIt);
	//********************迭代器方式结束****************************


	//********************动态地址方式****************************
	time0 = static_cast<double>(getTickCount());
	//调用颜色空间缩减函数（指针）
	colorReduceDynamic(srcImg, dstImgDy, 32);
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout<<"动态地址方式调用时间为："<<time0<<"秒"<<endl;
	//显示效果图
	imshow("【动态地址效果图】", dstImgDy);
	//********************动态地址方式结束****************************



	waitKey();
	return 0 ;

}


//用指针访问像素
void colorReducePtr(Mat &inputImg, Mat &outputImg, int div)
{
	//复制源图像到临时图像
	outputImg = inputImg.clone();
	//行数
	int rowNumber = outputImg.rows;
	//列数 * 通道数 = 每一行的元素个数
	int colNumber = outputImg.cols * outputImg.channels();

	//双重循环，遍历所有像素值
	for (int i = 0; i < rowNumber; i++)//行循环
	{
		uchar* data = outputImg.ptr<uchar>(i);//获取第i行的首地址

		for (int j = 0; j < colNumber; j++)//列循环
		{
			//开始处理每个像素
			data[j] = data[j]/div*div + div/2;
		}
	}
}

//用迭代器方式
void colorReduceIterator(Mat &inputImg, Mat &outputImg, int div)
{
	outputImg = inputImg.clone();
	//初始位置迭代器
	Mat_<Vec3b>::iterator it = outputImg.begin<Vec3b>();
	//终止位置迭代器
	Mat_<Vec3b>::iterator itend = outputImg.end<Vec3b>();

	//存取彩色图像像素
	for (;it != itend; it++)
	{
		(*it)[0] = (*it)[0]/div * div + div /2;
		(*it)[1] = (*it)[1]/div * div + div /2;
		(*it)[2] = (*it)[2]/div * div + div /2;
	}

}

//用动态地址计算
void colorReduceDynamic(Mat &inputImg, Mat &outputImg, int div)
{
	outputImg = inputImg.clone();
	int rowNumber = outputImg.rows;
	int colNumber = outputImg.cols;

	for (int i = 0; i < rowNumber; i++)
	{
		for (int j = 0; j < colNumber; j++)
		{
			//蓝色通道
			outputImg.at<Vec3b>(i,j)[0] = 
				outputImg.at<Vec3b>(i,j)[0]/div*div + div/2;
			//绿色通道
			outputImg.at<Vec3b>(i,j)[1] = 
				outputImg.at<Vec3b>(i,j)[1]/div*div + div/2;
			//红色通道
			outputImg.at<Vec3b>(i,j)[2] = 
				outputImg.at<Vec3b>(i,j)[2]/div*div + div/2;
		}
	}
}