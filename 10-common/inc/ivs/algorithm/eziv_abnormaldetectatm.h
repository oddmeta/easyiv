#ifndef _EZIV_ABNORMAL_DETECT_ATM_H_
#define _EZIV_ABNORMAL_DETECT_ATM_H_

#include <stdio.h>

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#include <windows.h>
//#include <wctype.h>
/*
#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_
typedef struct _SYSTEMTIME
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDayOfWeek;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;
	unsigned short wMilliseconds;
}SYSTEMTIME;

typedef struct _SYSTEMTIME *PSYSTEMTIME;
typedef struct _SYSTEMTIME *LPSYSTEMTIME;

#endif // !_SYSTEMTIME
*/

#define MAX_ObjectNumber	10				//最大可允许的物体数量

#ifndef TYPE_OBJECTINFO
class EASYIV_API ObjectInformation
{
//存放物体的各项信息
public:
	ObjectInformation(){objImage = cvCreateImage( cvSize(300,300), IPL_DEPTH_8U, 3); }
	~ObjectInformation() { cvReleaseImage(&objImage); }

public:
	enum OBJSTATE
	{
		STATE_INVALID = -1,
		STATE_INIT,
		STATE_DETECTED,
		STATE_ALARM,
	};
	
	int X;
	int Y; 
	OBJSTATE State;
	int Time;
	int objWidth; 
	int objHeight;
	int cancel_object_flag;
	int object_selection_flag; 
	int Updata_CancelObject_count; 
	IplImage* objImage;
	SYSTEMTIME AlarmTime; 
};
#define TYPE_OBJECTINFO
#endif
/**** 各参数说明 ****//*
				 (01) X:物体的X座标 (左下角)
				 (02) Y:物体的Y座标 (左下角)
				 (03) State:物体目前的状态 ; 0→初始状态
				 1→找到物体
				 2→发出警告
				 -1→取消物体
				 (04) Time:纪录找到物体时的时间点
				 (05) objWidth:物体的宽度
				 (06) objHeight:物体的高度
				 (07) cancel_object_flag:判断是否执行取消物体警告的处理 ; 0→不执行
				 1→执行
				 (08) object_selection_flag:判断是否选择到该物体 ; 0→不执行
				 1→执行
				 (09) Updata_CancelObject_count:执行取消物体时，该物体被updata成背景的次数
				 (10) objImage:存物体的图
				 (11) AlarmTime:纪录发出警告时的时间点
				 */

class EASYIV_API CAbnormalATM
{
public:
	CAbnormalATM();
	~CAbnormalATM();

public:
	BOOL32				Init(TEZIVINFO_ABNORMAL &tAbnormalParam);
	IplImage*			GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);
	BOOL32				DetectAndDraw(TEZIV_DETECT_RESULT& tResult);

public:
	void				InputImage(IplImage *);

	/*
	FUNCTION			:初始化(建立背景视频)
	*/
	BOOL32				UnderInitializing();

	/*
	FUNCTION			:设定物体大小   只侦测比size_width及size_height大的物体
	INPUT
	- size				:输入物体的最小size
	- size_width		:输入物体的最小横向长度
	- size_height		:输入物体的最小纵向长度
	*/
	void SetObjectSize(int size_width, int size_height);

	/*
	FUNCTION			:设定发生警告时间间隔大小   注意与Temporal TrackBar的数字与参数alarmtime有关
	INPUT
	- time_index		:发生警告时间间隔大小
	*/
	void SetTimeInterval(int time_index);

	/*
	FUNCTION			:考虑该不该发警告
	INPUT
		CurFrame		:输入来源视频
		ptFrameBG		:输入背景视频
		ptFrameFG		:输入前景视频
		x				:座标X
		y				:座标Y
	OUTPUT
		TRUE			发alarm
		FALSE			不发alarm
	*/
	BOOL32 ObjectCase(IplImage* CurFrame, IplImage* ptFrameBG, IplImage* ptFrameFG, int x, int y);

	/*
	FUNCTION			:重新排序物体编号与其参数
	*/
	void Re_rankingObject();

	/*
	FUNCTION			:归零各项参数
	INPUT
	- index:第N个物体
	*/
	void ClearObjectInf(int index);

	/*
	FUNCTION			:判断是否显示警告
	INPUT
	- CurFrame			:输入来源视频
	- m_ptFrameBG		:输入背景视频   
	- m_ptFrameFG		:输入前景视频
	- IntervalTime		:间隔N秒的时间
	*/
	void ShowAlarm(IplImage* CurFrame, IplImage* ptFrameBG, IplImage* ptFrameFG, int IntervalTime);

	/*
	FUNCTION			:取得前景视频
	INPUT
	- sourceImg			:输入来源视频
	- backgroundImgModel:输入背景视频 
	- foregroundImg		:存放处理完的前景视频 
	- background_thresh	:判断为前景的threshold
	*/
	void GetForeground(IplImage* sourceImg, IplImage* backgroundImgModel, IplImage* foregroundImg, int background_thresh);

	/*
	FUNCTION			:建背景视频与更新
	INPUT
	- SourceImage		:输入来源视频
	- foregroundImg		:输入前景视频
	- m_ptFrameBG		:存放处理完的背景视频
	- IntervalFrame		:间隔多少张frame执行一次
	- select			:模式选择
		select=0 → 使用于初始，建背景模型
		select=1 → 使用于后续，间隔一段时间更新背景 (前景不更新)
	- alpha				:建背景模型时的权重
	*/
	void UpdateBackgroundImage(IplImage *SourceImage, IplImage *foregroundImg, IplImage* ptFrameBG, int IntervalFrame, int select, double alpha);

	/*
	FUNCTION			:找异常物体并记录其各项信息
	INPUT
	- CurFrame			:输入来源视频
	- foregroundImg		:输入前景视频   
	*/
	void FindObject(IplImage* CurFrame, IplImage* foregroundImg);

	/*
	FUNCTION			:手动取消物体
	INPUT:
	- CurFrame			:输入来源视频
	- m_ptFrameBG		:输入背景视频
	- ObjNumber			:第N个物体
	- UpdataInterval	:物体图像更新的次数
	*/
	void HandCancelObject(IplImage* CurFrame, IplImage* ptFrameBG, int ObjNumber, int UpdataInterval);

	/*
	FUNCTION			:自动取消物体
	INPUT				:
	- CurFrame			:输入来源视频
	- m_ptFrameBG		:输入背景视频   
	- index				:第N个物体
	- m_Minute			:间隔N分
	- m_Second			:间隔N秒
	- UpdataInterval	:物体图像更新的次数
	*/
	void AutomaticCancelObject(IplImage* CurFrame, IplImage* ptFrameBG, int index, int m_Minute, int m_Second, int UpdataInterval);

private:
	IplImage*	m_ptFrameCopy;					//当前frame
	IplImage*	m_ptFrameBG;					//背景
	IplImage*	m_ptFrameFG;					//前景
	IplImage*	m_ipmaxObjProb;					//Optical flow
	//IplImage*	m_ptFrameObjProbMax;			//光流

	IplImage*	m_ptFrameLast;					//储存先前frame信息   拿来做光流法判断用
	IplImage*	m_ptFrameObjProb;				//存放停滞不动统计的值
#ifdef _ABNORMAL_DETECT_ENABLE_RESIZE_
	IplImage*	m_ptFrameResize;				//存放resize image (320x240)
#endif

	// used for Find object
	IplImage*   m_ipdeNoiseForeground, *m_ipEditImage, *m_ipgCurFrame, *m_ipgLastFrame, *m_vex, *m_vey;
	//IplImage* m_ipldeNoiseForeground2,*m_ipleditImage, * m_iplgCurFrame,* m_iplgLastFrame,* vex,* vey,* maxObjProb;

	//used to UpdateBackground Image
	IplImage* m_ipTemp1, *m_ipTemp2;

	//used to Get Foreground Image
	IplImage* m_ipdeNoiseForeground1;		

	ObjectInformation ObjInf[MAX_ObjectNumber]; //宣告物体类别

private:
	TEZIVINFO_ABNORMAL	m_tAbnormalParam;

	int			m_nFrameCount;					//计算建背景(初始化)时当下所经frame张数
	//int		m_nFrameCountTotal;				//在Alarm function 计数当下所经frame张数 (暂不用)
	int			m_nUpdateCount;					//计数frame张数来做前景更新 (背景不更新)
	int			m_nUpdateCountBG;				//计数frame张数来做背景更新
	int			m_nFrameRate; 					//Frame rate

	//IplImage *m_ptFrameCur;					//090813 added by Jacky Wei
};

#endif //_EZIV_ABNORMAL_DETECT_ATM_H_
