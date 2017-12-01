
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BLURDETECT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EASYIV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#include <stdio.h>
#include <time.h>

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#ifdef _EiC 
#define WIN32
#endif

class EASYIV_API CBlurDetect
{
	CvFont font;//字型格式
	char *PrintfString;//字串暂存Buffer

	CvMat *maskV;//垂直average filter
	CvMat *maskH;//水平average filter

	BOOL32 *Focus_DuringTime;//纪录Focus_Blur的时间
	BOOL32 *Image_DuringTime;//纪录Image_Blur的时间

	float Focus_Blur_Parameter; //画面失焦参数
	float Image_Blur_Parameter; //画面模糊参数

	double time_count;    //时间计算累计值
	time_t pre_time; //计时使用
	BOOL32 Time_Difference;//时间有差异时的旗标
	double initial_time;//初使时间设定
	int Alarm_time;//Alarm在多少时间内去计算
	float Alarm_time_percent;//时间内模糊情况超过多少百分比即发出警讯
	BOOL32 initial_flag;//初使旗标,是否initial使用

	int Average_filter_size;//平均filter的size
	int edge_threshold;//计算edge所使用的threshold
	int sobel_filter_size;//计算edge 所使用的sobel filter size
	float Min_Focus_Parameter;//纪录最小Focus_Parameter normalize用
	float Min_Image_Parameter;//纪录最小Image_Parameter normalize用
	float Focus_maximum;//Focus_Parameter最大值预设为
	float Image_maximum;//Image_Parameter最大值为多少预设为
	float Focus_percent_threshold;//Focus_Parameter 的threshold
	float Image_percent_threshold;//Image_Parameter 的threshold

	int Focus_NowNum;//记录目前Focus_DuringTime秒数位置
	float Focus_frame_number;//目前Focus秒数的Frame Number
	float Focus_Blur_number;//目前Focus秒数模糊的Frame Number
	float Focus_time_count;//目前有多少时间间隔是Focus Blur

	int Image_NowNum;//记录目前Image_DuringTime秒数位置
	float Image_frame_number;//目前Image秒数的Frame Number
	float Image_Blur_number;//目前Image秒数模糊的Frame Number
	float Image_time_count;//目前有多少时间间隔是Image Blur


	BOOL32 Focus_Blur_Alarm;//画面失焦警告旗标
	BOOL32 Image_Blur_Alarm;//画面模糊警告旗标
	BOOL32 Blur_Alarm;//有模糊状态旗标

public:

	IplImage *gray;    //将读入的Frame转成灰阶
	IplImage *edge;    //计算灰阶影像的边界值
	IplImage *blurV;   //垂直模糊影像
	IplImage *blurH;   //水平模糊影像
	IplImage *frame_msg;   //输出资讯图像

	CBlurDetect(int frame_Width,int frame_Height,int edge_thr,int sobel_size,int filter_size,double initial_second,float Focus_max_multiple,float Image_max,
		float Focus_threshold,float Image_threshold,int Alarm_second,float Alarm_percent);//初始化
	void SetImage(IplImage *frame,time_t now_time);//frame input 主架构运算
	float FocusBlurFeature(IplImage *gray);//计算画面失焦参数
	float ImageBlurFeature(IplImage *gray);//计算画面模糊参数
	BOOL32 UnderInitializing();//是否为初使的函数
	BOOL32 Focus_Blur_time(BOOL32 Focus_Result);//Focus Blur时间是否超过threshold
	BOOL32 Image_Blur_time(BOOL32 Image_Result);//Image Blur时间是否超过threshold
	void SetParameter(int edge_thr,int sobel_size,int filter_size,double initial_second,float Focus_max_multiple,float Image_max,
		float Focus_threshold,float Image_threshold,int Alarm_second,float Alarm_percent);//设定参数函数
	void ReInitializing();//重新初始化函数
	BOOL32 Alarm();//是否发生blur
	~CBlurDetect();//解构子释放记忆体
};
