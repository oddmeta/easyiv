
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
	int		m_nMissedFrame;			// 追丢的frame数
	int		m_nGrade;				// 跟追踪到的物体距离分数
	CvPoint m_atTrackPoints[200];	// 轨迹点
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

	void		loadframe(IplImage*  tmp_frame);	//读取当前frame
	void		loadframe(u8* pImageArray);

	void		subimage();							//背景相减
	void		Template_Match();					//比对人型样板
	int			GetPoint(int t,int k);				//读取框人的座标
	int			getInOut(EM_EZIV_INTRUDER_DIRECTION emDirection); // k=1取得in 人数 , k=2取得out 人数
	void		resetInOut();						//人数归零

	int			AlarmFun();

	BOOL32		m_bInitializing;

private:
	TEZIVINFO_INTRUDER	m_tConfig;

	IplImage*	m_ptTemplateImg;		//人的pattern
	IplImage*	m_ptFrameHSV;			//当前的frame
	IplImage*	m_ptFrameBGModelNew;	//一开始的背景	
	IplImage*	m_ptTemplateScaled;		//scale之後新的pattern
	IplImage*	m_ptFrameBG;			//更新过後的背景
	IplImage*	m_ptFrameBGModel;		//背景相减时使用
	IplImage*	m_ptFrameFG;			//前景 Gray Level
	IplImage*	m_ptFrameResult;		//pattern比对的结果
	IplImage*	m_ptFrameTrack;			//轨迹

	IplImage*	m_ptFrameReGray;		//撷取人的上部
	IplImage*	m_ptFrameReBGR;			//前景 BGR
	IplImage*	m_ptFrameMask;			//遮罩    可将前景部份滤掉  不做背景更新

//	CvRect		m_tRectN;

	BOOL32		m_bStartCountHuman;		//开始数人数

	TIntrudeObj m_atHumanObj[EZIV_MAX_INTRUDE_DETECT_OBJS];

	int			m_nCrossIn,m_nCrossOut;								//in 人数 , out 人数 
	BOOL32		m_bCrossInAlarm;									//0901 Alarm发动状态 by Tsao
	BOOL32		m_bCrossOutAlarm;									//0901 Alarm发动状态 by Tsao

	int			m_anCrossInCount[EZIV_MAX_INTRUDE_DETECT_OBJS];		//0729 新增 记录物体跨过边线 by Tsao
	int			m_anCrossOutCount[EZIV_MAX_INTRUDE_DETECT_OBJS];	//0729 新增 记录物体从另一边跨过边线  by Tsao
	int			m_anCrossingIn[EZIV_MAX_INTRUDE_DETECT_OBJS];		//0729 新增 记录物体跨过边线瞬间    by Tsao
	int			m_anCrossingOut[EZIV_MAX_INTRUDE_DETECT_OBJS];		//0729 新增 记录物体从另一边跨过边线瞬间   by Tsao

	int			m_anBlockX1[EZIV_MAX_INTRUDE_DETECT_OBJS];
	int			m_anBlockY1[EZIV_MAX_INTRUDE_DETECT_OBJS];
	int			m_anBlockX2[EZIV_MAX_INTRUDE_DETECT_OBJS];
	int			m_anBlockY2[EZIV_MAX_INTRUDE_DETECT_OBJS]; //存框框左上及右下座标

	CvPoint		pt0,pt1,pt2,pt3;				//绘图时使用


	int FrameNum;		//第几张frame
	int chnal,nWidthStep;
	CvPoint np2,np3;

	int center_x[1000],center_y[1000],CenterNum; //每个pattern比对到的中心 以及个数
	int ptr[EZIV_MAX_INTRUDE_DETECT_OBJS];//轨迹中有几个转折点
};
