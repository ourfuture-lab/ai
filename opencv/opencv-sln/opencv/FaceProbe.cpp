// Haar特征检测 - 人脸识别  
#include <opencv2/opencv.hpp>  
int main()  
{  
	// 加载Haar特征检测分类器  
	// haarcascade_frontalface_alt.xml系OpenCV自带的分类器  
	const char *pstrCascadeFileName = "haarcascade_frontalface_alt.xml";  
	CvHaarClassifierCascade *pHaarCascade = NULL;  
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeFileName);  

	// 载入图像  
	const char *pstrImageName = "h1.jpg";  
	IplImage *pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_UNCHANGED);  

	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);  

	// 人脸识别与标记  
	if (pHaarCascade != NULL)  
	{
		CvMemStorage *pcvMStorage = cvCreateMemStorage(0);  
		cvClearMemStorage(pcvMStorage);  
		// 识别  
		CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage); 

		// 标记  
		for(int i = 0; i <pcvSeqFaces->total; i++)  
		{  
			CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);  
			CvPoint center;  
			int radius;  
			center.x = cvRound((r->x + r->width * 0.5));  
			center.y = cvRound((r->y + r->height * 0.5));  
			radius = cvRound((r->width + r->height) * 0.25);  
			cvCircle(pSrcImage, center, radius, CV_RGB(255, 0, 0), 1);  
		}  
		cvReleaseMemStorage(&pcvMStorage);  
	}  

	const char *pstrWindowsTitle = "人脸识别";  
	cvNamedWindow(pstrWindowsTitle, CV_WINDOW_AUTOSIZE);  
	cvShowImage(pstrWindowsTitle, pSrcImage);  

	cvWaitKey(0);  

	cvDestroyWindow(pstrWindowsTitle);  
	cvReleaseImage(&pSrcImage);   
	cvReleaseImage(&pGrayImage);  
	return 0;  
}