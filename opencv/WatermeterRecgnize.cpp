#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <iostream>  
#include <string>

#define TRAIND_NUMBER_COUNT 5
#define SCAN_WINDOW_HEIGHT 36
  
using namespace std;  
using namespace cv;  

class Picture{
protected:
	double threshold;			//模板匹配判断是否匹配的阈值
	char* path;					//读取测试图像的路径
	Mat srcImg;					//用作源图像
	Mat srcResult;				//用来显示
	int scanWindowW;			//扫描窗宽度
	int scanWindowH;			//扫描窗高度
	int srcImgWidth, srcImgHeight;		//原图像宽度高度
	int window_x,window_y;		//当前扫描窗口左上角坐标
	int countnumbers;			//当前已经识别出的数字个数
	
	int adaptiveBiSize;			//自适应二值化均衡窗口大小
	int adaptiveBiParam;		//自适应二值化计算系数
	int medianBlurSize;			//中值滤波窗口大小

	vector<int> numresult;		//存储识别结果
	vector<Mat> templatVec;		//存储模板图像
	int index[8];				//记录模板图像的下标索引

	
public:
	Picture(){
		threshold = 0.48;
		scanWindowW = 38;
		scanWindowH = SCAN_WINDOW_HEIGHT;
		countnumbers = 1;
		index[0]=0;				//训练样本下标索引
		index[1]=1;
		index[2]=2;
		index[3]=5;
		index[4]=8;
	/*	index[5]=6;
		index[6]=8;
		index[7]=9;*/
		// 自适应二值化和中值滤波参数
		adaptiveBiSize = 11;
		adaptiveBiParam= 19;
		medianBlurSize = 3;

	}
	void loadImage(){
		//读入待测试图像,并保存为灰度图
		Mat mat = imread("234.jpg");
		
		if(!mat.data)  
		{  
			cout << "打开图像失败"<< endl;  
			exit(0);  
		}
		namedWindow("original");
		imshow("original",mat);
		Mat mergeImg;	//合并后的图像
		Mat matgray;	//灰度图
		//用来存储各通道图片的向量
		vector<Mat> splitBGR(mat.channels());
		//分割通道，存储到splitBGR中
		split(mat,splitBGR);
		//对各个通道分别进行直方图均衡化
		for(int i=0; i<mat.channels(); i++)
			equalizeHist(splitBGR[i],splitBGR[i]);
		//合并通道
		merge(splitBGR,mergeImg);
		//rgb图像转成灰度图
		cvtColor(mergeImg,matgray,CV_RGB2GRAY);	
		srcImg = matgray;
		
	}
	virtual void loadTemplate(){
		for (int i=0;i<TRAIND_NUMBER_COUNT;i++){
		char path[100];
		sprintf(path,"train\\%d.bmp",index[i]);
		Mat tmp = imread(path,0);
		templatVec.push_back(tmp);	
		//模板图像二值化
		//自适应二值化
		adaptiveThreshold(templatVec[i], templatVec[i], 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 7);
		//滤波
		medianBlur( templatVec[i], templatVec[i], 3);
		
		}
	};
	void preProcess(){          //自适应二值化&中值滤波
		Mat out;
		//自适应二值化
		adaptiveThreshold(srcImg, srcImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, adaptiveBiSize, adaptiveBiParam); 
		//中值滤波
		namedWindow("binary");
		imshow("binary",srcImg);
		medianBlur( srcImg, out, medianBlurSize);
		namedWindow("medianblur");
		imshow("medianblur",out);
		srcImg = out;
		srcResult = out;  //用来显示
	}
	Mat scanWindow(int window_x,int window_y){
		return srcImg(Rect(window_x,window_y,scanWindowW,scanWindowH));
	}		//返回扫描窗
	bool match(Mat src){
		int srcW,srcH,templatW, templatH, curtemplatW,curtemplatH,resultH, resultW; 
		Mat templat,result;
		srcW = src.cols;  
		srcH = src.rows;
		double currentMin = 1;
		int    currentIndex=0;
		double minValue, maxValue;  
		Point minLoc, maxLoc,matchLoc; 
		/*
		** 相似度计算方法
		** 0：CV_TM_SQDIFF        平方差匹配法，最好的匹配值为0；匹配越差，匹配值越大
		** 1：CV_TM_SQDIFF_NORMED 归一化平方差匹配法
		** 2：CV_TM_CCORR         相关匹配法：该方法采用乘法操作；数值越大表明匹配程度越好
		** 3：CV_TM_CCORR_NORMED  归一化相关匹配法
		** 4：CV_TM_CCOEFF        相关系数匹配法：1表示完美的匹配；-1表示最差的匹配。
		** 5：CV_TM_CCOEFF_NORMED 归一化相关系数匹配法
		*/
		int methodType=1;
		//循环判断8个数字哪个数字模板最为接近被测试图像
		for (int i=0;i<TRAIND_NUMBER_COUNT;i++){
			templat = templatVec[i];
			templatW = templat.cols;  
			templatH = templat.rows;  
			if(srcW < templatW || srcH < templatH)  
			{  
				cout <<"模板不能比原图像大" << endl;  
				return 0;  
			}  
			resultW = srcW - templatW + 1;  
			resultH = srcH - templatH + 1;  
			result = cvCreateImage(cvSize(resultW, resultH), 1, 1);  
			
			matchTemplate(src, templat, result, methodType);   
			 
			minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc,Mat() );
			//如果比当前最小还小，则储存该值,下标和坐标
			if (minValue<currentMin){
				currentMin = minValue;
				currentIndex=i;
				matchLoc.x=minLoc.x+window_x;
				matchLoc.y=minLoc.y+window_y;
				curtemplatW = templatW;
				curtemplatH = templatH;
			}
		}

		//最小值比设定阈值小，则判断识别出这个数字
		if (currentMin<threshold){
			numresult.push_back(index[currentIndex]);
			cout<<"第"<<countnumbers<<"个数字是："<<index[currentIndex]<<endl;

			countnumbers++;
			rectangle(srcResult, matchLoc, cvPoint(matchLoc.x + curtemplatW, matchLoc.y+ curtemplatH), cvScalar(0,0,255));
	
			window_x =matchLoc.x+curtemplatW-1;
			return true;
		}
		//比阈值大则判定为非字符，扫描窗右移一个单位
		window_x++;
		return false;
    
	}
	
	virtual void processScan()
	{
		srcImgWidth = srcImg.cols;
		srcImgHeight = srcImg.rows;
		window_x = 0;
		window_y = 3;
		//加十以提高容错率
		bool last = false;
		while(window_x<srcImgWidth-scanWindowW+5)
		{
			if (window_x+scanWindowW>srcImgWidth)
			{
				window_x = srcImgWidth - scanWindowW;
				last = true;
			}
			Mat tmp = scanWindow(window_x,window_y);
			match(tmp);
			if (last) break;
		}
		window_x = 30;
		scanWindowH = SCAN_WINDOW_HEIGHT;
		window_y=srcImgHeight - scanWindowH;
		while (window_x<=srcImgWidth - scanWindowW-10){
			
			Mat tmp = scanWindow(window_x,window_y);
			match(tmp);
		}
		
	}

	
	void startRecognize(){
		loadImage();
		loadTemplate();
		preProcess();
		processScan();
	}
};

 
int main()  
{  
	Picture pic = Picture();
	pic.startRecognize();
	waitKey(0);
	
    return 0;  
}  