/*
����ͼ�񣬽���ͼ���ϣ���ʾͼ�񣬲����ͼ��jpg
*/

//#include <opencv2\core\core.hpp>
//#include <opencv2\highgui\highgui.hpp>
//using namespace cv;
//
//int main()
//{
//	Mat dog=imread("1.jpg");
//	namedWindow("��1������ͼ");
//	imshow("��1������ͼ", dog);
//
//	Mat image = imread("2.jpg");
//	Mat logo = imread("logo.jpg");
//	//��ʾ
//	namedWindow("��2��ԭͼ");
//	imshow("��2��ԭͼ", image);
//	namedWindow("��2��logoͼ");
//	imshow("��2��logoͼ", logo);
//
//	Mat imgROI;
//	//imgROI = image(Rect(600, 350, logo.cols, logo.rows));
//
//	imgROI = image(Range(350,350+logo.rows), Range(600, 600+logo.cols));
//	//logo�ӵ�ԭͼ��
//	addWeighted(imgROI, 0.5, logo, 0.3, 0, imgROI);
//
//	//��ʾ���
//	namedWindow("��2���ϳ�ͼ");
//	imshow("��2���ϳ�ͼ", image);
//
//	//���
//	imwrite("�ϳ�ͼ.jpg", image);
//	waitKey();
//
//	return 0;
//}