//#include "opencv2/opencv.hpp"
//using namespace cv;
//
//#define WINDOW_NAME "【程序窗口】"
//
//void on_MouseHandle(int event, int x, int y, int flags, void *param);
//void DrawRectangle(Mat &img, Rect box);
//void ShowHelpText();
//
//Rect g_rectangle;
//bool g_bDrawingBox = false;
//RNG g_rng(12345);
//
//int main()
//{
//	g_rectangle = Rect(-1, -1, 0, 0);
//	Mat srcImg(600, 800, CV_8UC3), tempImg;
//	srcImg.copyTo(tempImg);
//
//	g_rectangle = Rect(-1, -1, 0, 0);
//	srcImg = Scalar::all(0);
//	
//	//设置鼠标操作回调函数
//	namedWindow(WINDOW_NAME);
//	setMouseCallback(WINDOW_NAME, on_MouseHandle, (void*)&srcImg);
//
//	//程序主循环，当进行绘制的标识符为真时，进行绘制
//	while(1)
//	{
//		srcImg.copyTo(tempImg);
//		
//		if (g_bDrawingBox)
//		{				
//			DrawRectangle(tempImg, g_rectangle);			
//		}
//
//		imshow(WINDOW_NAME, tempImg);
//		if (waitKey(10) == 27)
//		{
//			break;
//		}
//
//	}
//	return 0;
//}
//
//void on_MouseHandle(int event, int x, int y, int flags, void* params)
//{
//	Mat &img = *(Mat*)params;
//	switch(event)
//	{
//		case EVENT_MOUSEMOVE:
//		{
//			if (g_bDrawingBox)
//			{
//				g_rectangle.width = x-g_rectangle.x;
//				g_rectangle.height = y -g_rectangle.y;
//			}
//			break;
//		}
//			
//
//		case EVENT_LBUTTONDOWN:
//			{
//				g_bDrawingBox = true;
//				g_rectangle = Rect(x, y, 0, 0);
//				break;
//			}
//			
//		case EVENT_LBUTTONUP:
//			{
//				g_bDrawingBox= false;
//				if (g_rectangle.width < 0)
//				{
//					g_rectangle.x += g_rectangle.width;
//					g_rectangle.width *= -1;
//				}
//
//				if (g_rectangle.height < 0)
//				{
//					g_rectangle.y += g_rectangle.height;
//					g_rectangle.height *= -1;
//				}
//
//				DrawRectangle(img, g_rectangle);
//
//				break;
//			}
//	}
//}
//
//void DrawRectangle(Mat &img, Rect box)
//{
//	rectangle(img, box.tl(), box.br(), Scalar(g_rng.uniform(0,255), g_rng.uniform(0,255), g_rng.uniform(0,255)));
//}
//
