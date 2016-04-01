#include <opencv2\opencv.hpp>
using namespace cv;

int main44()
{
	VideoCapture capture(0);

	while(1)
	{
		Mat frame, edges;
		capture>>frame;

		//cvtColor(frame, edges, CV_BGR2GRAY);
		//blur(edges, edges, Size(7,7));
		//Canny(edges, edges, 0, 30, 3);

		imshow("¶ÁÈ¡ÊÓÆµ", frame);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}
