#ifndef _EZIV_FACE_DETECT_H_
#define _EZIV_FACE_DETECT_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <iostream>
using namespace std;

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#ifdef _EiC 
#define WIN32
#endif

class EASYIV_API CFaceDetect
{
public:
	CFaceDetect();
	~CFaceDetect();

public:
	BOOL32		Init(TEZIVINFO_FACEDETECT &tFaceParam);
	BOOL32		InputFrame(IplImage *img2);											//���������
	void		DetectAndDraw(TEZIV_DETECT_RESULT& tResult);						//�ҳ�������ʾ����
	BOOL32		Alarm();
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);

public:
	BOOL32		BeginDetectFace();
	void		UpdateBackgroundImage(int IntervalFrame, int select, double alpha);	//���±���
	void		GetForeground(IplImage *img2,int background_thresh);				//ȡ��ǰ��

	void		SetArea(int width, int height, double area);
	void		SetScale(double scale_size);										//�����趨
	void		SetSearchRange(double search_percent);								//�����趨
	void		SetBackgroundSamples(int nInitBackground);

#ifdef _ENABLE_ENHANCEMENT_
	//���ߵ�������
	double		GAMA_TRAN(IplImage* frame,double standrand,double gama,int shift);
	//void		SetAlignmentMode(EZIV_IMAGE_ENHANCE modenumber,double newparemeter1,double newparemeter2);
	void		SetAlignmentMode(EZIV_IMAGE_ENHANCE modenumber, TEZIVGamaTrans &tGamaTrans, TEZIVHistEqualize &tHistEqualize);
	void		ImageAlignment(IplImage* frame);
	void		EQUALIZE_HIST(IplImage* frame,int type,int low,int high,IplImage* img_transform);
	void		myEqulizeHist(IplImage* img,int low,int high,IplImage* img_transform);
	//���ߵ�������
#endif

private:
	TEZIVINFO_FACEDETECT	m_tFaceParam;

	IplImage *m_ptFrameCopy;					//�ݴ�frame
	IplImage *m_ptFrameBG;						//����frame 
	IplImage *m_ptFrameFG;						//ǰ��frame 
	IplImage *m_ptFrameObj;						//ǰ�������� 

	BOOL32 m_bDetectedFace;						//��⵽����
	BOOL32 m_bDetectBegin;						//ǰ�������������⵽������С
	CvMemStorage* m_pcStorage;
	CvHaarClassifierCascade* m_pcClassifier;	//ѵ���õķ�����
	s8 m_achHaarXML[512];						//�ļ�����
	CvPoint m_tROIPointLT,m_tROIPointRB;		//ROI���ϵ������µ�
	CvRect m_tROIRect;							//�趨ROI��С

	//////////////////////////////////////////////////////////////////////////
	//��������ͼƬ��С����λ�ü���ͼƬ��С��//Ŀǰ��Ҫץȡͬ�ȴ�С���� ����ȡ��
	//��¼ץ���ĵ�һ�����Ĵ�С��y��������
#ifdef _ENABLE_SAME_SIZED_FACE_
	int detect,detect2;							//����ź�
	int m_nInitPosY,m_nInitSize;				//face image��С��λ��
	int m_nInitPosY2,m_nInitSize2;				//face image��С��λ��2
	int last_size;								//face image���մ�С
	int length;									//ͬһ��������face image���ƶ���λ����仯�Ĵ�С
#endif
	//////////////////////////////////////////////////////////////////////////

	BOOL32 m_bBackgroundFlag;					//�����Ƿ��Ѿ�������
	int m_nUpdateCount;							//��������frame	
	int m_nCount;								//����frame	

	CvScalar m_tLoDiff,m_tUpDiff;				//	
	CvPoint m_tSeedPoint;						//��ɫ��ʼ������	
	CvConnectedComp m_tConnectedComp;			//ǰ������������Ϣ
	CvRect m_tPeopleRect;						//��������

	IplImage *m_ptNewBG;					   //����frame 
#ifdef _ENABLE_ENHANCEMENT_
	//���ߵ�������start
	//int Mode;
	//double parameter1,parameter2;
	double m_dPreGama;
	double* m_pdGamaColor;
	//���ߵ�������end
#endif
};

#endif //_EZIV_FACE_DETECT_H_
