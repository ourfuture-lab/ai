// Haar������� - ����ʶ��  
#include <opencv2/opencv.hpp>  
int main()  
{  
	// ����Haar������������  
	// haarcascade_frontalface_alt.xmlϵOpenCV�Դ��ķ�����  
	const char *pstrCascadeFileName = "haarcascade_frontalface_alt.xml";  
	CvHaarClassifierCascade *pHaarCascade = NULL;  
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeFileName);  

	// ����ͼ��  
	const char *pstrImageName = "h1.jpg";  
	IplImage *pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_UNCHANGED);  

	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);  

	// ����ʶ������  
	if (pHaarCascade != NULL)  
	{
		CvMemStorage *pcvMStorage = cvCreateMemStorage(0);  
		cvClearMemStorage(pcvMStorage);  
		// ʶ��  
		CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage); 

		// ���  
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

	const char *pstrWindowsTitle = "����ʶ��";  
	cvNamedWindow(pstrWindowsTitle, CV_WINDOW_AUTOSIZE);  
	cvShowImage(pstrWindowsTitle, pSrcImage);  

	cvWaitKey(0);  

	cvDestroyWindow(pstrWindowsTitle);  
	cvReleaseImage(&pSrcImage);   
	cvReleaseImage(&pGrayImage);  
	return 0;  
}