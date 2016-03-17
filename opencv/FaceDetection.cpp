#include "opencv2/opencv.hpp"
#include "highgui/highgui.hpp"
using namespace cv;


int maind(int argc, char** argv)
{
	CvHaarClassifierCascade * pCascade = 0;
	CvMemStorage * pStorage = 0;
	CvSeq * pFaceRectSeq;
	int i;

	//≥ı ºªØ
	IplImage * pInpImg = cvLoadImage("h2.jpg");
	pStorage = cvCreateMemStorage(0);
	pCascade = (CvHaarClassifierCascade *)cvLoad("haarcascade_frontalface_alt.xml");

	// validate that everything initialized properly 

	if (!pInpImg || !pStorage || !pCascade)
	{
		printf("Initialization failed: %s \n",  
			(!pInpImg)?  "didn't load image file" :
			(!pCascade)? "didn't load Haar cascade -- "  
			"make sure path is correct" :  
		"failed to allocate memory for data storage");  
		exit(-1); 
	}

	// detect faces in image 
	pFaceRectSeq = cvHaarDetectObjects(
			pInpImg, pCascade, pStorage,
			1.1,	 // increase search scale by 10% each pass 
			3,		 // drop groups of fewer than three detections  
			CV_HAAR_DO_CANNY_PRUNING,// skip regions unlikely to contain a face  
			cvSize(0, 0) // use XML default for smallest search scale  
		);


	// create a window to display detected faces  
	cvNamedWindow("Haar Window", CV_WINDOW_AUTOSIZE);

	// draw a rectangular outline around each detection  
	for (i = 0; i < (pFaceRectSeq ? pFaceRectSeq->total : 0); i++)
	{
		CvRect *r =(CvRect *)cvGetSeqElem(pFaceRectSeq, i);
		CvPoint pt1 = {r->x, r->y};
		CvPoint pt2 = {r->x + r->width, r->y +r->height};
		cvRectangle(pInpImg, pt1, pt2, CV_RGB(255, 0, 0), 1, 4, 0);
	}

	cvShowImage("Harrv Window", pInpImg);
	cvWaitKey(0);

	cvDestroyWindow("Haar Window");  

	// clean up and release resources  
	cvReleaseImage(&pInpImg);  
	if(pCascade) cvReleaseHaarClassifierCascade(&pCascade);  
	if(pStorage) cvReleaseMemStorage(&pStorage);  
	return 0;  
}