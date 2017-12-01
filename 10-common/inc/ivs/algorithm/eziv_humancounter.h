
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <iostream>
using namespace std;

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

typedef u8 BYTE ;

#ifndef TYPE_OBJECT
struct Object
{
	BOOL32 existFlag ;	//0 no 1 yes
	int missedFrame;	//׷����frame��
	int grade;			//��׷�ٵ�������������
    CvPoint mean[200];	//�켣��
	int circleFrame;	//��ͷ����Բ�ε�����
};
#define TYPE_OBJECT
#endif

class EASYIV_API CHumanCounter
{
public:
	CHumanCounter(int width, int height,int nChannels,int widthStep ,int t=0);
	~CHumanCounter();

public:
	BOOL32		Init(TEZIVINFO_HUMANCOUNT &tFaceParam);
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);

private:
    TEZIVINFO_HUMANCOUNT	m_tConfig;
    //char			m_achConfPath[256];
    IplImage*		m_pimgTemplate; //�˵�pattern
 
	CvPoint pt0,pt1,pt2,pt3;		//��ͼʱʹ��
	CvPoint pt4,pt5;
		
    IplImage*    hsv                ; //��ǰ��frame
	IplImage*    hsv_head           ; //���˵Ĳ���
    IplImage*    newbg_model        ; //һ��ʼ�ı���	
    IplImage*    templ_new          ; //scale֮���µ�pattern
    IplImage*    backgroundImg      ; //���¹���ı���
    IplImage*    backgroundImgModel ; //�������ʱʹ��
    IplImage*    foregroundImg      ; //ǰ�� Gray Level
    IplImage*    result             ; //pattern�ȶԵĽ��
    IplImage*    trace              ; //�켣

	IplImage *regrayImg; //ߢȡ�˵��ϲ�
	IplImage *reBGRImg; //ǰ�� BGR
	CvSeq    *circles; //��¼Բ��Sequence
	IplImage *mask;		//����    �ɽ�ǰ�������˵�  ������������

	CvMemStorage* storage; //��¼Բ��Memory

	int a,b,FrameNum;          //in ���� , out ���� ,�ڼ���frame
	int nWidth,nHeight,chnal,nWidthStep;
	int x1,y1,x2,y2;  //���ߵ�����		
    double nA,nB;        //y=nA*x+nB
	CvRect setROI;   //set inner ROI
	CvRect nrect;    //set outer ROI
	BOOL32 system_on;	//��ʼ������
	BOOL32 SetThreshold_on; //�˵Ĵ�С,���������趨���
	BOOL32 SetRoi_on; //ROI�趨���
	BOOL32 Circle_on; //ʹ������ͷ�Ĺ���
	int block_x1[10],block_y1[10],block_x2[10],block_y2[10]; //�������ϼ���������
	float nscale; //pattern scale�Ĵ�С
	int nthreshold; //���������threshold
	int center_x[1000],center_y[1000],CenterNum; //ÿ��pattern�ȶԵ������� �Լ�����
	int ptr[10];  //�켣���м���ת�۵�
	BOOL32 circle_ok[10]; //ÿ�ο���ʱ�Ƿ���Բ
	
public:
	struct Object humanObj[10]; //����10������

    void	SetThreshold(float scale,int threshold ); //��ȡ�˵Ĵ�С������������ֵ
	void	SetRoi(int sx,int sy,int sw,int sh,IplImage*  tmp_frame); 

    //		loadtempl();	//��ȡpattern
	void	loadframe(IplImage*  tmp_frame); //��ȡ��ǰframe
	void	loadframe(BYTE* pImageArray);

	void	subimage();     //�������
	void	FindCircle();	//��Բ
	void	Template_Match(); //�ȶ���������
	int		GetPoint(int t,int k); //��ȡ���˵�����
	void	ResetPoint(); //reset���˵�����
	int		getInOut(int k); // k=1ȡ��in ���� , k=2ȡ��out ����
	void	resetInOut();  //��������
	void	releaseimage(); //�ͷ����м�����ռ�
};
