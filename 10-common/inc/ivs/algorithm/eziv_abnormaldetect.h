/*** 此为Abnormal Detect 的header file ***/

#include <stdio.h>
//#include <WinBase.h>
//#include <WTypes.h>

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#define  objectNumber  5 //可允许的物件数量

#ifndef TYPE_OBJECTINFO
class EASYIV_API ObjectInformation
{
	//存放物件的各项资讯
public:
	ObjectInformation() { objImage = cvCreateImage( cvSize(300,300), IPL_DEPTH_8U, 3); }
	int X;
	int Y; 
	int State;
	int Time;
	int objWidth; 
	int objHeight;
	int cancel_object_flag;
	int object_selection_flag; 
	int Updata_CancelObject_count; 
	IplImage* objImage;
	SYSTEMTIME_ AlarmTime; 
	~ObjectInformation() { cvReleaseImage(&objImage); }
};
#define TYPE_OBJECTINFO
#endif
/**** 各参数说明****//*
 (01) X:物件的X座标(左下角)
 (02) Y:物件的Y座标(左下角)
 (03) State:物件目前的状态; 0→初始状态
	 		 				 1→找到物件
							 2→发出警告
						    -1→取消物件
 (04) Time:纪录找到物件时的时间点
 (05) objWidth:物件的宽度
 (06) objHeight:物件的高度
 (07) cancel_object_flag:判断是否执行取消物件警告的处理; 0→不执行
														  1→执行
 (08) object_selection_flag:判断是否选择到该物件; 0→不执行
												   1→执行
 (09) Updata_CancelObject_count:执行取消物件时，该物件被updata成背景的次数
 (10) objImage:存物件的图
 (11) AlarmTime:纪录发出警告时的时间点
*/

#ifndef TYPE_ABNORMALDETECT
class EASYIV_API CAbnormalDetect
{
	//异常物件的类别成员与函式
private:
    int nWidth, nHeight; //frame的宽与高
	int nSeconds1; //静止物体持续多少秒才算是预警物件
	int nSeconds2; //静止物体持续多少秒才算是异常物件
	int nSeconds3; //异常物件框选多少秒後取消该框选

	int mFrameCount;              //计算建背景(初始化)时当下所经frame张数
	//int totalFrameCount;        //在Alarm function 计数当下所经frame张数(暂不用)
	int UpdateCount;              //计数frame张数来做前景更新(背景不更新)
	int BG_UpdateCount;           //计数frame张数来做背景更新
	int FrameIntervalCount;       //间隔多少张frame执行一次Optical flow
	int FrameIntervalCountValue;  //根据多少张frame执行一次Optical flow的threshold
	int WindowSize;               //计算光流值时，撷取的区块大小
	int BoundarySpace;            //设定边界范围


	int nFrameRate; 	         //Frame rate
	int Train_BgModel_number;    //计算建背景所需的frame张数
	double weight;               //计算初始时背景模型的权重值

	//IplImage *CurFrame; //090813 add
	//IplImage* maxObjProb;

	CvSize nScale; //只侦测比这个值大的物件


public:
	IplImage* CurrentImage;     //当前frame
	IplImage* BackgroundImage;  //背景
	IplImage* ForegroundImage;  //前景
	IplImage* LastFrame;        //储存先前frame资讯  拿来做光流法判断用
	IplImage* ObjcetProb;       //存放停滞不动统计的值
	IplImage* pImage1;          //存放resize image (320x240)
	CAbnormalDetect(int, int, int, int, int); //建构子	
	~CAbnormalDetect(); //解构子

	ObjectInformation ObjInf[objectNumber]; //宣告物件类别

	//////////////////////////////////////////////////////////////////////////
	// Add by Bruse
	// UpdateBackgroundImage:
	IplImage* m_ipTemp1;
	IplImage* m_ipTemp2;

	// GetForeground
	IplImage* m_ipDeNoiseForeground1;

	// FindObject
	IplImage* m_ipDeNoiseForeground2;
	IplImage* m_ipEditImage;
	IplImage* m_ipGCurFrame;
	IplImage* m_ipGLastFrame;
	IplImage* m_ipVex;
	IplImage* m_ipVey;
	IplImage* m_ipMaxObjProb;
	


	//void SetObjectSize(CvSize);  //设定物件大小
	void SetObjectSize(int,int); //设定物件大小  (可由Spatial TrackBar来调整物件大小)
	void SetTimeInterval(int); //设定设定发生警告时间间隔大小(可由Temporal TrackBar来调整间隔时间大小  须与AlarmTime 作配合)
	void SetImage(IplImage *); //设定影像资料
	IplImage* GetImage(int);   //取得影像资料

	BOOL32 UnderInitializing(); //初始化(建立背景影像)
	BOOL32 Alarm(); //判断是否有警告

	void UpdateBackgroundImage(IplImage *, IplImage *, IplImage*, int, int, double); //建背景影像与更新
	void GetForeground(IplImage*, IplImage*, IplImage*, int); //取得前景影像
    void FindObject(IplImage*, IplImage*); //找异常物件并记录其各项资讯
	void ShowAlarm(IplImage*, IplImage*, IplImage*, int); //判断是否显示警告
	void HandCancelObject(IplImage* , IplImage* , int , int); //处理手动取消物件时的程序
	void AutomaticCancelObject(IplImage*, IplImage*, int, int, int, int); //处理自动取消物件时的程序
	void ClearObjectInf(int); //归零各项参数
	void Re_rankingObject(); //重新排序物件编号与其参数

	BOOL32 ObjectCase(IplImage*, IplImage*, IplImage*, int, int); //考虑该不该发警告

};
#define TYPE_ABNORMALDETECT
#endif

