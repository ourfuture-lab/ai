#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ExtendHoughCircle.h"
#include <iostream>
#include <list>
using namespace cv;
using namespace std;
using namespace ExtendCV;


#define eps 0.0000000001
#define PI acos(-1.0)
int dcmp(double x){
	if(fabs(x)<eps)return 0;
	else return x<0 ? -1:1;
}
double Dot(Point A,Point B){return A.x*B.x+A.y*B.y;}//�������
double Length(Point A){return sqrt(Dot(A,A));}//����ģ��
double Cross(Point A,Point B){return A.x*B.y-A.y*B.x;}//�������
double Angle(Point A,Point B){return acos(Dot(A,B)/Length(A)/Length(B));}//�������ļн�
double DistanceToLine(Point P,Point A,Point B)//�㵽ֱ�ߵľ���
{
	Point v1=B-A,v2=P-A;
	return fabs(Cross(v1,v2))/Length(v1);//������Ӿ���ֵ�Ǵ��з���ľ���
}
double DistancetoSegment(Point P,Point A,Point B){//�㵽�߶εľ���
	if(A==B)return Length(P-A);
	Point v1=B-A,v2=P-A,v3=P-B;
	if(dcmp(Dot(v1,v2))<0)return  Length(v2);
	else if(dcmp(Dot(v1,v3))>0)return Length(v3);
	else return fabs(Cross(v1,v2))/Length(v1);
}
//-----------------------------------------------------------------------------
class MyLine{
public:
	int id;//���
	int k;//��б��[0-360)
	int l;//����
public:
	MyLine(int ID=0,int K=0,int L=0){id=ID,k=K,l=L;}//���캯��
	bool operator<(const MyLine &A){return k<A.k;}//�ض���С�ں�
	void print(){printf("id: %3d  k: %3d��  l: %3d\n",id,k,l);}//�������
};//�Զ���ֱ��
//-----------------------------------------------------------------------------
int main()
{
	Mat src = imread("time_5.jpg",0);//��ȡͼƬ��mat
	Mat temp,dst;//src-->temp-->dst(srcΪԭͼ��temp��src��canny��ȡ��Ե��ͼ��
	//�����������任��dst�����ս��ͼ��Ҫ��temp�Ҷ�ͼ�Ļ����ϱ�Ϊ��ɫͼ���ܳ��ֻ���Ч����

	Canny(src, temp, 10, 140, 3);//��ȡ��Ե(�������Ե��ȡ�ͻ��˷Ѿ޴�ʱ�䣩
	cvtColor(temp, dst, CV_GRAY2BGR);//����Ե��ȡ�ĻҶ�ͼת��ΪBGRͼ���ڻ���

	imshow("temp", temp);
	imshow("dst0", dst);

	//������Բ������  
	//std::vector<Vec3f> circles; 
	//����Hough�任���Բ  
	//����Ϊ�������ͼ�񣬼��������ⷽ�����������Ψһ��,�ۼ����ķֱ��ʣ�����Բ��ľ��룬canny���޵����ޣ������Զ���Ϊ���޵�һ�룩��Բ������Ҫ����С��ͶƱ����������С�뾶  
	//HoughCircles(temp,circles,CV_HOUGH_GRADIENT,2,50,200,100,100,300);  

	vector<circle_found> circles;
	FindCircles(src,circles,1,1,183,332,10,200,210,0.5);

	//�ҳ�Բ�̣���Ϊ���Ĳ�һ���ǵģ����Լ��˼�������������
	int pos=0;
	int max=-1;
	for(size_t i = 0; i < circles.size(); i++ )
	{  
		Vec3f f=circles[i].circle;
		if(f[2]>max && f[0]+f[2]<temp.rows && f[0]-f[2]>=0 && f[1]+f[2]<temp.cols && f[1]-f[2]>0)
		{
			max=f[2];
			pos=i;
		}
	} 
	Point center(circles[pos].circle[0],circles[pos].circle[1]);//�ҵ���Բ��
	int   radius= circles[pos].circle[2];//�ҵ��İ뾶
	circle(dst,center,radius,Scalar(255),2);  
	

	list<MyLine> list_MyLine;
	vector<Vec4i> lines2;//�߶μ��
	HoughLinesP(temp, lines2, 1, CV_PI/180, 50, 50, 10 );
	for( size_t i = 0; i < lines2.size(); i++ )
	{
		Vec4i l = lines2[i];
		Point A(l[0], l[1]),B(l[2], l[3]);
		if(DistancetoSegment(center,A,B)<30)//����Բ�ĵ�ָ��ľ�����ֵ�˵������߶�
		{
			bool down=(A.y+B.y-2*center.y>0);//�жϳ����ڹ�Բ�ĵ�ˮƽ���ϲ������²�
			if(A.x==B.x){//б��Ϊ��������
				list_MyLine.push_back(MyLine(i,90+(down?180:0),Length(Point(A.x-B.x,A.y-B.y))));
			}else if(A.y==B.y){//ˮƽ�����
				list_MyLine.push_back(MyLine(i,A.x+B.x-2*center.x>0 ? 0:180,Length(Point(A.x-B.x,A.y-B.y))));
			}else{
				if(down){
					if(A.y>center.y)
						list_MyLine.push_back(MyLine(i,360-(int)(atan2((float)(A.y-B.y),(float)(A.x-B.x))*180/PI),Length(Point(A.x-B.x,A.y-B.y))));
					else 
						list_MyLine.push_back(MyLine(i,360-(int)atan2((float)(B.y-A.y),(float)(B.x-A.x))*180/PI,Length(Point(A.x-B.x,A.y-B.y))));
				}else{
					if(A.y<center.y)
						list_MyLine.push_back(MyLine(i,abs(atan2((float)(A.y-B.y),(float)(A.x-B.x))*180/PI),Length(Point(A.x-B.x,A.y-B.y))));
					else 
						list_MyLine.push_back(MyLine(i,abs(atan2((float)(B.y-A.y),(float)(B.x-A.x))*180/PI),Length(Point(A.x-B.x,A.y-B.y))));
				}
			}
			line(dst,A,B, Scalar(0,0,i*20+40), 2, CV_AA);
		}	
	}

	//���ݽǶ���������ָ��
	int now_k,pre_k=720;//��ǰ�߶εĽǶȺ�ǰһ���߶εĽǶ�
	int num=0;//ָ����������Ϊ2��3��
	int Du[3]={0};//3��ָ��Ķ�����ÿ���ƽ����
	int Le[3]={0};//Le[i]=Le_ping[i]*0.2+le_max[i]*0.8;
	int Le_ping[3]={0};//3��ָ��ĳ��ȣ�ÿ���ƽ����
	int Le_max[3]={0};//3��ָ��ĳ��ȣ�ÿ�������ģ�
	int t_num=0;//ÿ�����������ƽ���ã�
	MyLine now_Line;
	list_MyLine.push_back(MyLine(99,888,0));//�����в���һ���ұ߽��������㴦��
	list_MyLine.sort();
	while(!list_MyLine.empty())
	{
		now_Line=list_MyLine.front();
		now_k=now_Line.k;
		if(abs(now_k-pre_k)>10)//�����Ƕ�֮��С��10�������ͬһ��
		{
			if(num!=0){//�Ա���Ķ����ͳ�����ƽ��
				Du[num-1]/=t_num;
				Le_ping[num-1]/=t_num;
				Le[num-1]=Le_ping[num-1]*0.2+Le_max[num-1]*0.8;
			}
			if(now_k==888)break;//�ұ߽�ֱ������
			t_num=0;//����ͳ����һ��
			num++;//��������1
			cout<<"---------------------------\n";//����ָ���
		}
		t_num++;//���ڶ�һ����
		Du[num-1]+=now_Line.k;
		Le_ping[num-1]+=now_Line.l;
		if(now_Line.l>Le_max[num-1])Le_max[num-1]=now_Line.l;
		now_Line.print();
		list_MyLine.pop_front();
		pre_k=now_k;
	}
	cout<<"---------------------------\n\n";

	cout<<"---------------------------\n";
	int t;
	for(int i=0;i<num-1;i++){
		for(int j=i+1;j<num;j++){
			if(Le[i]>Le[j]){
				t=Le[i],Le[i]=Le[j],Le[j]=t;
				t=Du[i],Du[i]=Du[j],Du[j]=t;
			}//if end
		}//for end
	}//for end
	char s[3][10]={"hour  :","minute:","second:"};
	for(int i=0;i<num;i++)
		printf("%s  k: %3d��  l: %3d\n",s[i],Du[i],Le[i]);
	cout<<"---------------------------\n";
	if(num==2)printf("time is: %2d:%2d\n",(360-Du[0]+90)%360/30,(360-Du[1]+90)%360/6);
	else if(num==3)printf("time is: %2d:%2d:%2d\n",(360-Du[0]+90)%360/30,(360-Du[1]+90)%360/6,(360-Du[2]+90)%360/6);
	cout<<"---------------------------\n";

	imshow("src", src);
	imshow("dst", dst);
	waitKey();	

	return 0;
}


