/*
载入图像，进行图像混合，显示图像，并输出图像到jpg
*/

//#include <opencv2\core\core.hpp>
//#include <opencv2\highgui\highgui.hpp>
//using namespace cv;
//
//int main()
//{
//	Mat dog=imread("1.jpg");
//	namedWindow("【1】动漫图");
//	imshow("【1】动漫图", dog);
//
//	Mat image = imread("2.jpg");
//	Mat logo = imread("logo.jpg");
//	//显示
//	namedWindow("【2】原图");
//	imshow("【2】原图", image);
//	namedWindow("【2】logo图");
//	imshow("【2】logo图", logo);
//
//	Mat imgROI;
//	//imgROI = image(Rect(600, 350, logo.cols, logo.rows));
//
//	imgROI = image(Range(350,350+logo.rows), Range(600, 600+logo.cols));
//	//logo加到原图上
//	addWeighted(imgROI, 0.5, logo, 0.3, 0, imgROI);
//
//	//显示结果
//	namedWindow("【2】合成图");
//	imshow("【2】合成图", image);
//
//	//输出
//	imwrite("合成图.jpg", image);
//	waitKey();
//
//	return 0;
//}