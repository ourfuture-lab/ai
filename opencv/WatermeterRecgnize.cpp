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
	double threshold;			//ģ��ƥ���ж��Ƿ�ƥ�����ֵ
	char* path;					//��ȡ����ͼ���·��
	Mat srcImg;					//����Դͼ��
	Mat srcResult;				//������ʾ
	int scanWindowW;			//ɨ�贰���
	int scanWindowH;			//ɨ�贰�߶�
	int srcImgWidth, srcImgHeight;		//ԭͼ���ȸ߶�
	int window_x,window_y;		//��ǰɨ�贰�����Ͻ�����
	int countnumbers;			//��ǰ�Ѿ�ʶ��������ָ���
	
	int adaptiveBiSize;			//����Ӧ��ֵ�����ⴰ�ڴ�С
	int adaptiveBiParam;		//����Ӧ��ֵ������ϵ��
	int medianBlurSize;			//��ֵ�˲����ڴ�С

	vector<int> numresult;		//�洢ʶ����
	vector<Mat> templatVec;		//�洢ģ��ͼ��
	int index[8];				//��¼ģ��ͼ����±�����

	
public:
	Picture(){
		threshold = 0.48;
		scanWindowW = 38;
		scanWindowH = SCAN_WINDOW_HEIGHT;
		countnumbers = 1;
		index[0]=0;				//ѵ�������±�����
		index[1]=1;
		index[2]=2;
		index[3]=5;
		index[4]=8;
	/*	index[5]=6;
		index[6]=8;
		index[7]=9;*/
		// ����Ӧ��ֵ������ֵ�˲�����
		adaptiveBiSize = 11;
		adaptiveBiParam= 19;
		medianBlurSize = 3;

	}
	void loadImage(){
		//���������ͼ��,������Ϊ�Ҷ�ͼ
		Mat mat = imread("234.jpg");
		
		if(!mat.data)  
		{  
			cout << "��ͼ��ʧ��"<< endl;  
			exit(0);  
		}
		namedWindow("original");
		imshow("original",mat);
		Mat mergeImg;	//�ϲ����ͼ��
		Mat matgray;	//�Ҷ�ͼ
		//�����洢��ͨ��ͼƬ������
		vector<Mat> splitBGR(mat.channels());
		//�ָ�ͨ�����洢��splitBGR��
		split(mat,splitBGR);
		//�Ը���ͨ���ֱ����ֱ��ͼ���⻯
		for(int i=0; i<mat.channels(); i++)
			equalizeHist(splitBGR[i],splitBGR[i]);
		//�ϲ�ͨ��
		merge(splitBGR,mergeImg);
		//rgbͼ��ת�ɻҶ�ͼ
		cvtColor(mergeImg,matgray,CV_RGB2GRAY);	
		srcImg = matgray;
		
	}
	virtual void loadTemplate(){
		for (int i=0;i<TRAIND_NUMBER_COUNT;i++){
		char path[100];
		sprintf(path,"train\\%d.bmp",index[i]);
		Mat tmp = imread(path,0);
		templatVec.push_back(tmp);	
		//ģ��ͼ���ֵ��
		//����Ӧ��ֵ��
		adaptiveThreshold(templatVec[i], templatVec[i], 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 31, 7);
		//�˲�
		medianBlur( templatVec[i], templatVec[i], 3);
		
		}
	};
	void preProcess(){          //����Ӧ��ֵ��&��ֵ�˲�
		Mat out;
		//����Ӧ��ֵ��
		adaptiveThreshold(srcImg, srcImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, adaptiveBiSize, adaptiveBiParam); 
		//��ֵ�˲�
		namedWindow("binary");
		imshow("binary",srcImg);
		medianBlur( srcImg, out, medianBlurSize);
		namedWindow("medianblur");
		imshow("medianblur",out);
		srcImg = out;
		srcResult = out;  //������ʾ
	}
	Mat scanWindow(int window_x,int window_y){
		return srcImg(Rect(window_x,window_y,scanWindowW,scanWindowH));
	}		//����ɨ�贰
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
		** ���ƶȼ��㷽��
		** 0��CV_TM_SQDIFF        ƽ����ƥ�䷨����õ�ƥ��ֵΪ0��ƥ��Խ�ƥ��ֵԽ��
		** 1��CV_TM_SQDIFF_NORMED ��һ��ƽ����ƥ�䷨
		** 2��CV_TM_CCORR         ���ƥ�䷨���÷������ó˷���������ֵԽ�����ƥ��̶�Խ��
		** 3��CV_TM_CCORR_NORMED  ��һ�����ƥ�䷨
		** 4��CV_TM_CCOEFF        ���ϵ��ƥ�䷨��1��ʾ������ƥ�䣻-1��ʾ����ƥ�䡣
		** 5��CV_TM_CCOEFF_NORMED ��һ�����ϵ��ƥ�䷨
		*/
		int methodType=1;
		//ѭ���ж�8�������ĸ�����ģ����Ϊ�ӽ�������ͼ��
		for (int i=0;i<TRAIND_NUMBER_COUNT;i++){
			templat = templatVec[i];
			templatW = templat.cols;  
			templatH = templat.rows;  
			if(srcW < templatW || srcH < templatH)  
			{  
				cout <<"ģ�岻�ܱ�ԭͼ���" << endl;  
				return 0;  
			}  
			resultW = srcW - templatW + 1;  
			resultH = srcH - templatH + 1;  
			result = cvCreateImage(cvSize(resultW, resultH), 1, 1);  
			
			matchTemplate(src, templat, result, methodType);   
			 
			minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc,Mat() );
			//����ȵ�ǰ��С��С���򴢴��ֵ,�±������
			if (minValue<currentMin){
				currentMin = minValue;
				currentIndex=i;
				matchLoc.x=minLoc.x+window_x;
				matchLoc.y=minLoc.y+window_y;
				curtemplatW = templatW;
				curtemplatH = templatH;
			}
		}

		//��Сֵ���趨��ֵС�����ж�ʶ����������
		if (currentMin<threshold){
			numresult.push_back(index[currentIndex]);
			cout<<"��"<<countnumbers<<"�������ǣ�"<<index[currentIndex]<<endl;

			countnumbers++;
			rectangle(srcResult, matchLoc, cvPoint(matchLoc.x + curtemplatW, matchLoc.y+ curtemplatH), cvScalar(0,0,255));
	
			window_x =matchLoc.x+curtemplatW-1;
			return true;
		}
		//����ֵ�����ж�Ϊ���ַ���ɨ�贰����һ����λ
		window_x++;
		return false;
    
	}
	
	virtual void processScan()
	{
		srcImgWidth = srcImg.cols;
		srcImgHeight = srcImg.rows;
		window_x = 0;
		window_y = 3;
		//��ʮ������ݴ���
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