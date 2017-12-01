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
	BOOL32		InputFrame(IplImage *img2);											//背景相减法
	void		DetectAndDraw(TEZIV_DETECT_RESULT& tResult);						//找出脸并标示出来
	BOOL32		Alarm();
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);

public:
	BOOL32		BeginDetectFace();
	void		UpdateBackgroundImage(int IntervalFrame, int select, double alpha);	//更新背景
	void		GetForeground(IplImage *img2,int background_thresh);				//取得前景

	void		SetArea(int width, int height, double area);
	void		SetScale(double scale_size);										//参数设定
	void		SetSearchRange(double search_percent);								//参数设定
	void		SetBackgroundSamples(int nInitBackground);

#ifdef _ENABLE_ENHANCEMENT_
	//光线调整参数
	double		GAMA_TRAN(IplImage* frame,double standrand,double gama,int shift);
	//void		SetAlignmentMode(EZIV_IMAGE_ENHANCE modenumber,double newparemeter1,double newparemeter2);
	void		SetAlignmentMode(EZIV_IMAGE_ENHANCE modenumber, TEZIVGamaTrans &tGamaTrans, TEZIVHistEqualize &tHistEqualize);
	void		ImageAlignment(IplImage* frame);
	void		EQUALIZE_HIST(IplImage* frame,int type,int low,int high,IplImage* img_transform);
	void		myEqulizeHist(IplImage* img,int low,int high,IplImage* img_transform);
	//光线调整参数
#endif

private:
	TEZIVINFO_FACEDETECT	m_tFaceParam;

	IplImage *m_ptFrameCopy;					//暂存frame
	IplImage *m_ptFrameBG;						//背景frame 
	IplImage *m_ptFrameFG;						//前景frame 
	IplImage *m_ptFrameObj;						//前景区域画面 

	BOOL32 m_bDetectedFace;						//侦测到人脸
	BOOL32 m_bDetectBegin;						//前景面积超过能侦测到人脸大小
	CvMemStorage* m_pcStorage;
	CvHaarClassifierCascade* m_pcClassifier;	//训练好的分类器
	s8 m_achHaarXML[512];						//文件名称
	CvPoint m_tROIPointLT,m_tROIPointRB;		//ROI左上点与右下点
	CvRect m_tROIRect;							//设定ROI大小

	//////////////////////////////////////////////////////////////////////////
	//限制人脸图片大小（依位置计算图片大小）//目前需要抓取同等大小的脸 所以取消
	//记录抓到的第一张脸的大小与y方向座标
#ifdef _ENABLE_SAME_SIZED_FACE_
	int detect,detect2;							//侦测信号
	int m_nInitPosY,m_nInitSize;				//face image大小与位置
	int m_nInitPosY2,m_nInitSize2;				//face image大小与位置2
	int last_size;								//face image最终大小
	int length;									//同一个人两张face image间移动单位距离变化的大小
#endif
	//////////////////////////////////////////////////////////////////////////

	BOOL32 m_bBackgroundFlag;					//背景是否已经建立好
	int m_nUpdateCount;							//计数更新frame	
	int m_nCount;								//计数frame	

	CvScalar m_tLoDiff,m_tUpDiff;				//	
	CvPoint m_tSeedPoint;						//着色起始点座标	
	CvConnectedComp m_tConnectedComp;			//前景连续区域信息
	CvRect m_tPeopleRect;						//人形区域

	IplImage *m_ptNewBG;					   //背景frame 
#ifdef _ENABLE_ENHANCEMENT_
	//光线调整参数start
	//int Mode;
	//double parameter1,parameter2;
	double m_dPreGama;
	double* m_pdGamaColor;
	//光线调整参数end
#endif
};

#endif //_EZIV_FACE_DETECT_H_
