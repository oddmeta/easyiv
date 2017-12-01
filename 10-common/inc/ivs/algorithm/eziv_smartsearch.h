#ifndef _EZIV_SMART_SEARCH_H_
#define _EZIV_SMART_SEARCH_H_

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

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

#ifdef _EiC 
#define WIN32
#endif

typedef struct tagSmartSearchTemp
{
	CvRect	m_tRect;
	CvPoint m_tPoint;
	BOOL32	m_bUpdated;
}TSMARTSEARCHTEMP;

class CIntelliSearchObj
{
public:
	CIntelliSearchObj();

private:
	//进入相关资讯
	int		m_nInFrameNum;						//计数进入Frame数
	int		m_nInFrameCount;					//纪录进入点的Frame Number

	//离开相关资讯	
	int		m_nOutFrameNum;						//计数离开Frame数
	int		m_nOutFrameCount;					//纪录离开点的Frame Number
	//中心点位置
	CvPoint m_tCenterPt;						//记录中心点位置
	//判断信号
	BOOL32	m_bPeopleIn;						//人员进入信号
	BOOL32	m_bPeopleOut;						//人员离开信号
	BOOL32	m_bFaceDetect;						//是否侦测到脸
	BOOL32	m_bDataUpdate;						//资料是否有更新
	BOOL32	m_bOutputData;						//是否输出资料

	EM_EZIV_INTELLISEARCH_TYPE m_emIntelliType;

public:
	//////////////////////////////////////////////////////////////////////////
	//更新资料与控制信号
	//输出已记录资讯（测试用）
	void	PrintData		(int index);
	void	CountingIn		(int FrameCount);
	void	CountingOut		(int FrameCount);
	void	UpdatePosition	(CvRect PeopRect);						//更新已记录前景资讯
	void	ResetData		();										//重设所有资料

	void	FaceDetected	(){m_bFaceDetect = TRUE;};				//设定侦测到人脸信号
	void	ResetUpdate		(){m_bDataUpdate = FALSE;};				//重设Update信号

	//////////////////////////////////////////////////////////////////////////
	//回传资料与控制信号
	CvPoint	CenterPoint(){return m_tCenterPt;};						//回传中心点资讯
	BOOL32	Update(){return m_bDataUpdate;};						//回传Update信号
	BOOL32	In(){return m_bPeopleIn;};								//回传In信号
	BOOL32	Out(){return m_bPeopleOut;};							//回传Out信号
	BOOL32	DataOut(){return m_bOutputData;};						//回传是否输出信号, 回传人员是否可以取Data, 人员是否离开
	int		ReturnInFrameNum(){return m_nInFrameCount;};			//回传进入时的FrameNum,人员进入时的frame number
	BOOL32	ReturnFace(){return m_bFaceDetect;};					//回传过程中是否有侦测到人脸
	int		ReturnOutFrameNum(){return m_nOutFrameCount;};			//回传离开时的FrameNum
};

class EASYIV_API CIntelliSearch
{
public:
	CIntelliSearch();
	~CIntelliSearch();

public:
	BOOL32		Init(TEZIVINFO_INTELLISEARCH &tParam);								//Initialize configurations
	void		DetectAndDraw(IplImage *img );										//Find faces and draw them in the image
	BOOL32		Alarm();															//Trigger when detected people left the image
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);						//Get image array

public:
	void		Detect(IplImage *img2);												//Background subtraction
	void		UpdateBackgroundImage(int IntervalFrame, int select, double alpha);	//Update background image
	void		GetForeground( int background_thresh,IplImage *img);				//Get foreground image
	void		CoutTempInfo(int index);											//Output temp info
	int			ReturnObjectNum();
	void		ReturnData(int *InNum,int *OutNum,BOOL32 *face);
	void		FindMinDistance(int temp , CvPoint TempPoint,int people,CvPoint PeoplePoint);	//Find object in min distance

#ifdef _ENABLE_ENHANCEMENT_
	//光线调整参数
	double		GAMA_TRAN(IplImage* frame,double standrand,double gama,int shift);
	//void		SetAlignmentMode(int modenumber,double newparemeter1,double newparemeter2);
	void		SetAlignmentMode(EM_EZIV_IMAGE_ENHANCE modenumber, TEZIVGamaTrans &tGamaTrans, TEZIVHistEqualize &tHistEqualize);
	void		ImageAlignment(IplImage* frame);
	void		EQUALIZE_HIST(IplImage* frame,int type,int low,int high,IplImage* img_transform);
	void		myEqulizeHist(IplImage* img,int low,int high,IplImage* img_transform);
	//光线调整参数
#endif
private:
	TEZIVINFO_INTELLISEARCH	m_tConfig;

	CIntelliSearchObj*		m_pcPeopleInfo;			//储存人员资讯	
	TSMARTSEARCHTEMP*		m_ptTempInfo;			//暂存画面中物件资讯	
	CvMemStorage*			m_pMemStorage;			//分类器储存空间
	CvHaarClassifierCascade*m_pCascade;				//训练好的分类器

	IplImage*		m_ptFrameBG;					//背景frame
	IplImage*		m_ptFrameFG;					//前景frame
	IplImage*		m_ptFrameCopy;					//暂存frame
	IplImage*		m_ptFrameObj;					//前景区域画面

	BOOL32			m_bAlarm;						//侦测到人脸时发出信号
	int				m_nObjectsInFrame;				//目前frame中物体个数
	BOOL32			m_bOutputData;					//人员离开的话输出动线记录
	int				m_nOutputObjectNum;				//输出物件数目
	BOOL32			m_bFaceOverlap;					//人脸是否重叠
	int				m_bUpdateCount;					//计数更新frame
	int				m_nFrameCount;					//计数frame

	//计算距离
	int				m_nMinDistance;					//最小距离
	int				m_nMinIndex;					//最小距离的物件编号

	int				m_nInitROI_Y;					//ROI起始点Y座标
	int				m_nROIMaxCoordY,m_nROIMaxCoordX;		//frame的宽与高

	double			m_dFaceArea;					//启动人脸侦测的前景区块大小
	CvScalar		m_tScalarLoDiff,m_tScalarUpDiff;//
	CvPoint			m_tSeedPoint;					//着色起始点座标
	CvConnectedComp m_tConnectedComp;				//前景连续区域资讯
	CvRect			m_tPeopleRect;					//人形区域

	//光线调整参数
	//double search_range;
#ifdef _ENABLE_ENHANCEMENT_
	double			m_dPreGama;
	double*			m_pdGamaColor;
#endif
	IplImage*		m_ptNewBG;						//背景frame 
	IplImage*		m_ptHSVImageFG;
	IplImage*		m_ptHSVImageBG;					//前景影像
	//光线调整参数
	//int detect,detect2;							//侦测信号
	//int initial_y,initial_size;					//face image大小与位置
	//int initial_y2,initial_size2;					//face image大小与位置2
	//int last_size;								//face image最终大小
	//int length;									//同一个人两张face image间移动单位距离变化的大小
};

#endif //_EZIV_SMART_SEARCH_H_
