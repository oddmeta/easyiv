
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "math.h"
#include <iostream>
using namespace std;

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#ifndef TYPE_OBJECT
typedef struct tagIntrudeObj
{
	BOOL32	m_bExistFlag ;			// 0 no 1 yes
	int		m_nMissedFrame;			// ׷����frame��
	int		m_nGrade;				// ��׷�ٵ�������������
	CvPoint m_atTrackPoints[200];	// �켣��
}TIntrudeObj;
#define TYPE_OBJECT
#endif

class EASYIV_API IntruderDetect
{
public:
		IntruderDetect(int width, int height,int nChannels,int widthStep );
	~IntruderDetect();	

public:
	BOOL32		Init(TEZIVINFO_INTRUDER &tParam);
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);
	IplImage*	GetImage(int imgnum);

public:
	void		SetThreshold(float scale,int threshold ); 
	void		SetRoi(CvRect tROI);
	void		SetRoad(int p_x0,int p_y0,int p_x1,int p_y1);	 

	void		loadframe(IplImage*  tmp_frame);	//��ȡ��ǰframe
	void		loadframe(u8* pImageArray);

	void		subimage();							//�������
	void		Template_Match();					//�ȶ���������
	int			GetPoint(int t,int k);				//��ȡ���˵�����
	int			getInOut(EM_EZIV_INTRUDER_DIRECTION emDirection); // k=1ȡ��in ���� , k=2ȡ��out ����
	void		resetInOut();						//��������

	int			AlarmFun();

	BOOL32		m_bInitializing;

private:
	TEZIVINFO_INTRUDER	m_tConfig;

	IplImage*	m_ptTemplateImg;		//�˵�pattern
	IplImage*	m_ptFrameHSV;			//��ǰ��frame
	IplImage*	m_ptFrameBGModelNew;	//һ��ʼ�ı���	
	IplImage*	m_ptTemplateScaled;		//scale֮���µ�pattern
	IplImage*	m_ptFrameBG;			//���¹���ı���
	IplImage*	m_ptFrameBGModel;		//�������ʱʹ��
	IplImage*	m_ptFrameFG;			//ǰ�� Gray Level
	IplImage*	m_ptFrameResult;		//pattern�ȶԵĽ��
	IplImage*	m_ptFrameTrack;			//�켣

	IplImage*	m_ptFrameReGray;		//ߢȡ�˵��ϲ�
	IplImage*	m_ptFrameReBGR;			//ǰ�� BGR
	IplImage*	m_ptFrameMask;			//����    �ɽ�ǰ�������˵�  ������������

//	CvRect		m_tRectN;

	BOOL32		m_bStartCountHuman;		//��ʼ������

	TIntrudeObj m_atHumanObj[EZIV_MAX_INTRUDE_DETECT_OBJS];

	int			m_nCrossIn,m_nCrossOut;								//in ���� , out ���� 
	BOOL32		m_bCrossInAlarm;									//0901 Alarm����״̬ by Tsao
	BOOL32		m_bCrossOutAlarm;									//0901 Alarm����״̬ by Tsao

	int			m_anCrossInCount[EZIV_MAX_INTRUDE_DETECT_OBJS];		//0729 ���� ��¼���������� by Tsao
	int			m_anCrossOutCount[EZIV_MAX_INTRUDE_DETECT_OBJS];	//0729 ���� ��¼�������һ�߿������  by Tsao
	int			m_anCrossingIn[EZIV_MAX_INTRUDE_DETECT_OBJS];		//0729 ���� ��¼����������˲��    by Tsao
	int			m_anCrossingOut[EZIV_MAX_INTRUDE_DETECT_OBJS];		//0729 ���� ��¼�������һ�߿������˲��   by Tsao

	int			m_anBlockX1[EZIV_MAX_INTRUDE_DETECT_OBJS];
	int			m_anBlockY1[EZIV_MAX_INTRUDE_DETECT_OBJS];
	int			m_anBlockX2[EZIV_MAX_INTRUDE_DETECT_OBJS];
	int			m_anBlockY2[EZIV_MAX_INTRUDE_DETECT_OBJS]; //�������ϼ���������

	CvPoint		pt0,pt1,pt2,pt3;				//��ͼʱʹ��


	int FrameNum;		//�ڼ���frame
	int chnal,nWidthStep;
	CvPoint np2,np3;

	int center_x[1000],center_y[1000],CenterNum; //ÿ��pattern�ȶԵ������� �Լ�����
	int ptr[EZIV_MAX_INTRUDE_DETECT_OBJS];//�켣���м���ת�۵�
};
