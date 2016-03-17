/*
������ʹ��hog�����ӽ��������̽��
*/
#include "opencv2/opencv.hpp"
using namespace cv;
/*
1. hog��������opencv��ΪHOGDescriptor��

2. ���Ե��ø�������setSVMDetector���������ڶ�hog�������з����svmģ�͵�ϵ����ֵ������Ĳ���ΪHOGDescriptor::getDefaultPeopleDetector()ʱ��ʾ����ϵͳĬ�ϵĲ�������Ϊ��Щ�������úܶ�ͼƬѵ�������ġ�����

3.	������ͼƬ�������˼��ʱ����ͼƬ�Ĵ�С��һ��������Ҫ�õ���߶ȼ�⡣��������hog��ķ���detectMultiScale�������������£�

	 HOGDescriptor::detectMultiScale(const GpuMat& img, vector<Rect>& found_locations, doublehit_threshold=0, Size win_stride=Size(), Size padding=Size(), double scale0=1.05, int group_threshold=2)

	 ���ú�����ʾ�������ͼƬimg���ж�߶����˼�� imgΪ���������ͼƬ��found_locationsΪ��⵽Ŀ�������б�����3Ϊ�����ڲ�����Ϊ����Ŀ�����ֵ��Ҳ���Ǽ�⵽��������SVM���೬ƽ��ľ���;����4Ϊ��������ÿ���ƶ��ľ��롣
	   �������ǿ��ƶ���������������5Ϊͼ������Ĵ�С������6Ϊ����ϵ��������������ÿ�����ӵı���������7Ϊ����ֵ����У��ϵ������һ��Ŀ�걻������ڼ�����ʱ���ò�����ʱ�����˵������ã�Ϊ0ʱ��ʾ����������á�

4.  ���Լ�������Ŀ����ο�Ҫ����һЩ������������˵2��Ŀ���Ƕ���ţ���ѡ����������Ǹ���
5.  ��Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,������Ҫ����Щ���ο��С�ߴ���һЩ������


*/




int main()
{
	Mat img = imread("renqun.jpg");

	vector<Rect> found, found_filtered;
    HOGDescriptor people_dectect_hog;
    //����Ĭ�ϵ��Ѿ�ѵ�����˵�svmϵ����Ϊ�˴μ���ģ��
    people_dectect_hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    //�������ͼƬimg���ж�߶����˼��
    //imgΪ���������ͼƬ��foundΪ��⵽Ŀ�������б�����3Ϊ�����ڲ�����Ϊ����Ŀ�����ֵ��Ҳ���Ǽ�⵽��������SVM���೬ƽ��ľ���;
    //����4Ϊ��������ÿ���ƶ��ľ��롣�������ǿ��ƶ���������������5Ϊͼ������Ĵ�С������6Ϊ����ϵ����������ͼƬÿ�γߴ��������ӵı�����
    //����7Ϊ����ֵ����У��ϵ������һ��Ŀ�걻������ڼ�����ʱ���ò�����ʱ�����˵������ã�Ϊ0ʱ��ʾ����������á�
    people_dectect_hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);

    //��Դ���п��Կ���:
    //#define __SIZE_TYPE__ long unsigned int
    //typedef __SIZE_TYPE__ size_t;
    //���,size_t��һ��long unsigned int����
    size_t i, j;
    for (i = 0; i < found.size(); i++ )
        {
            Rect r = found[i];

            //��������for������ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵�
           //��,��ȡ���������Ǹ����ο����found_filtered��
            for(j = 0; j <found.size(); j++)
                if(j != i && (r&found[j])==r)
                    break;
            if(j == found.size())
               found_filtered.push_back(r);
        }

    //��ͼƬimg�ϻ������ο�,��Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ
    //��һЩ����
    for(i = 0; i <found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(img, r.tl(), r.br(), Scalar(255, 0, 0), 0);
    }

	imshow("t",img);
	waitKey(0);
	return 0;
    //imwrite("../hog_test_result.jpg", img);
}