
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
	int missedFrame;	//追丢的frame数
	int grade;			//跟追踪到的物体距离分数
    CvPoint mean[200];	//轨迹点
	int circleFrame;	//在头部有圆形的张数
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
    IplImage*		m_pimgTemplate; //人的pattern
 
	CvPoint pt0,pt1,pt2,pt3;		//绘图时使用
	CvPoint pt4,pt5;
		
    IplImage*    hsv                ; //当前的frame
	IplImage*    hsv_head           ; //框人的部份
    IplImage*    newbg_model        ; //一开始的背景	
    IplImage*    templ_new          ; //scale之後新的pattern
    IplImage*    backgroundImg      ; //更新过後的背景
    IplImage*    backgroundImgModel ; //背景相减时使用
    IplImage*    foregroundImg      ; //前景 Gray Level
    IplImage*    result             ; //pattern比对的结果
    IplImage*    trace              ; //轨迹

	IplImage *regrayImg; //撷取人的上部
	IplImage *reBGRImg; //前景 BGR
	CvSeq    *circles; //记录圆的Sequence
	IplImage *mask;		//遮罩    可将前景部份滤掉  不做背景更新

	CvMemStorage* storage; //记录圆的Memory

	int a,b,FrameNum;          //in 人数 , out 人数 ,第几张frame
	int nWidth,nHeight,chnal,nWidthStep;
	int x1,y1,x2,y2;  //界线的座标		
    double nA,nB;        //y=nA*x+nB
	CvRect setROI;   //set inner ROI
	CvRect nrect;    //set outer ROI
	BOOL32 system_on;	//开始数人数
	BOOL32 SetThreshold_on; //人的大小,背景参数设定完成
	BOOL32 SetRoi_on; //ROI设定完成
	BOOL32 Circle_on; //使用找人头的功能
	int block_x1[10],block_y1[10],block_x2[10],block_y2[10]; //存框框左上及右下座标
	float nscale; //pattern scale的大小
	int nthreshold; //背景相减的threshold
	int center_x[1000],center_y[1000],CenterNum; //每个pattern比对到的中心 以及个数
	int ptr[10];  //轨迹中有几个转折点
	BOOL32 circle_ok[10]; //每次框人时是否有圆
	
public:
	struct Object humanObj[10]; //宣告10个物体

    void	SetThreshold(float scale,int threshold ); //读取人的大小及背景参数的值
	void	SetRoi(int sx,int sy,int sw,int sh,IplImage*  tmp_frame); 

    //		loadtempl();	//读取pattern
	void	loadframe(IplImage*  tmp_frame); //读取当前frame
	void	loadframe(BYTE* pImageArray);

	void	subimage();     //背景相减
	void	FindCircle();	//找圆
	void	Template_Match(); //比对人型样板
	int		GetPoint(int t,int k); //读取框人的座标
	void	ResetPoint(); //reset框人的座标
	int		getInOut(int k); // k=1取得in 人数 , k=2取得out 人数
	void	resetInOut();  //人数归零
	void	releaseimage(); //释放所有记忆体空间
};
