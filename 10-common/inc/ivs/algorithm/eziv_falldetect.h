
//---------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FALLDETECT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EASYIV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

//using namespace std;

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#ifdef _EiC 
#define WIN32
#endif

#ifdef 	FALLDETECT_EXPORTS
#define EASYIV_API    __declspec(dllexport)
#endif
#ifdef  FALLDETECT_IMPORTS
#define EASYIV_API    __declspec(dllimport)
#endif
#ifdef  FALLDETECT_SOURCE
#define EASYIV_API
#endif

struct COEF_ELLIPSE
{
	//The parameters of the ellipse=========
	int x ; //椭圆中心点x座标
	int y ; //椭圆中心点y座标
	int x_t_1 ;//椭圆中心点上一时刻x座标
	int y_t_1 ;//椭圆中心点上一时刻y座标
	int x_t_2 ;  //椭圆中心点前两时刻x座标
	int y_t_2 ;  //椭圆中心点前两时刻y座标
	double RegionSize;  //记录该前景区域的pixel数
	double a ; //椭圆短轴长
	double a_1;  //上一时刻椭圆短轴长
	double b ; //椭圆长轴长
	double b_1;  //上一时刻椭圆长轴长
	double theta ; //椭圆与x轴方向的夹角
	double theta_t_1 ;  //上一时刻的夹角
	double theta_t_2 ;  //前时刻的夹角
	//======================================

	//The foreground image and the motion history image=======
	IplImage* Img_MHI ; //Motion History Image
	IplImage* Img_peoples ;//The Foregrounde Image
	IplImage* Img_peoples_t_1 ;//The Foregrounde Image
	//========================================================

	//The conuter=====================================
	double C_motion ; //coefficient of motion
	int Fall_cof_count ; //累积符合条件的frame数
	int Usual_cof_count ; //累积正常情况的frame数
	int cof_t_1 ;//上一时刻的状态,1-unusual, -1-usual
	long int fal_time ;  //跌倒後维持在地板上的时间，若超过此时间人未站起来即为跌倒事件
	//================================================

	//operators=======================================
	BOOL32 check_updata ;//判断这些参数是否有被更新过, false = 未更新过, true = 更新过
	BOOL32 Fall_detected;//若有跌倒事件发生为true
	COEF_ELLIPSE* next_node;//用於link list，连结下一个节点用
	//================================================
};

// This class is exported from the eziv_falldetect.dll
class EASYIV_API CFallDetect
{
private:
	struct COEF_ELLIPSE* DataObj ;//人型物件的资料串结
	int nWidth  ;//the size of the frame
	int nHeight ;
	double pi   ;//定义圆周率
	int count   ;//计算现在的frame数,用於判断建立background的frame数
	int UpdateCount ;//updata background frequence
	int people_area ;//define the size of people region

	int Fill_val ;//The parameters of the function, cvFloodFill
	CvPoint seed_point;//They are used in the function, Find_object
	CvScalar new_val  ;//填充前景时的新值，用於找寻前景位置
	CvScalar lo_diff  ;//填充颜色所需要的参数
	CvScalar up_diff  ;//填充颜色所需要的参数
	CvConnectedComp comp;//填充完前景後所记录的资讯皆在此

	int peoples_detect_last_time ;//纪录上一时刻侦测到的人员数
	int peoples_detect_this_time ;//记录此时刻侦测到的人员数
	int coef_peoples_detect ;//记录前景detect的状态

	double a ;//椭圆的短轴长
	double b ;//椭圆的长轴长
	double theta ;//椭圆与水平方向的夹角

	int C_motionthr ;//动量的门槛值
	double Ratiothr ;//长轴短轴比的门槛值
	double Thetathr ;//椭圆与水平方向夹角的变化量门槛值
	double Center_Positionthr ;//中心点变化量门槛值

	int After_Falltim ;//跌到後维持低motion的时间长度门槛,用於判断跌倒事件的发生
	int Fall_detect ;//若有跌倒事件发生为1
	int Fall_num ;//纪录跌倒的次数

public:
	IplImage *Foreground ;//前景影像
	IplImage *Object ;//找出的人的影像
	IplImage *Background ;//背景模型
	IplImage *frame_copy ;//待处理的影像
	CFallDetect(int ,int ,int ,int ) ;        //参数化建构子
	virtual ~CFallDetect() ;                       //解构子

public:
	void DoDetect(BOOL32, int, int, double);
	void UpdateBackgroundImage(int, int, double) ;// 建背景影像与更新
	void GetForeground(char*, int ) ;// 取得前景影像
	void RGB_TO_YUV(u8 *,double* ,double* ,double *) ;//change the color space(RGB2YUV)
	void Find_Object() ;//find the position of the object,using opencv function, cvFloodFill
	void Ellipse_Parameter(IplImage*,int* ,int* ) ;//compute the parameters of the ellipse
	void Set_Node(IplImage* ,int ,int ,double) ;//set new node
	void Find_Match(struct COEF_ELLIPSE** ,int ,int ) ;//画面中有多人时，判断此时刻与上一时刻match的资料
	void Updata_Node(struct COEF_ELLIPSE** ,IplImage* ,int ,int ,double );
	void Motion_History_Image(IplImage* ,IplImage* ,struct COEF_ELLIPSE* ) ;//compute the motion history image
	int Fall_Detect(struct COEF_ELLIPSE *) ;//Detect Falls
	int Alarm();//若有跌倒事件发出警讯

	///////Add by Tsao 抓出参数以供调整/////////////////////
	double GetC_motionthr();
	void SetC_motionthr(int);

	double GetThetathr();
	void SetThetathr(int);

	double GetCt_Posthr();
	void SetCt_Posthr(int);

	///////Add by Tsao 将counter也放进元件里做处理的修改/////////////////////

	BOOL32 Ini_and_Det();
};