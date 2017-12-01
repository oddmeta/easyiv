#pragma warning(disable: 4819)
#pragma warning(disable: 4996)

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
//#include <fstream>
using namespace std;

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FALLDETECT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EASYIV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

//using namespace std;

#ifdef _EiC 
#define WIN32
#endif

#define TrainFrams		50							//# of frame for background model training
#define Max_peoples		10							//可侦测到的人数上限值
#define WWidth			320							//调整输出的影片尺寸
#define HHeight			240  						//调整输出的影片尺寸
#define W				20							//ROI比ROD大W宽度

struct Code
{
	int freq;
	double R;
	double G;
	double B;
	double Range;
	double Sum;
};

struct Color
{
	CvHistogram *rHist;
	CvHistogram *gHist;
	CvHistogram *bHist;
};

class EASYIV_API CSuspectDetect
{
public:
	CSuspectDetect(int, int, int, int, int, int, int, int);
	~CSuspectDetect();

public:
	BOOL32		Init(TEZIVINFO_SUSPICIOUS &tParam);
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);

private:
	TEZIVINFO_SUSPICIOUS	m_tConfig;

public:
	/********************/
	/*	记录执行时间	*/
	/********************/
	//ofstream o_EachFrame;//("0l_EachFrame.txt");
	//ofstream o_SetImage;//("02_SetImage.txt");
	//ofstream o_UnderInitializing;//("03_UnderInitializing.txt");
	//ofstream o_Alarm;//("04_Alarm.txt");
	//ofstream o_MainProcess;//("05_MainProcess.txt");
	//ofstream o_TrainCodebook;//("06_TrainCodebook.txt");
	//ofstream o_ReorderCodebook;//("07_ReorderCodebook.txt");
	//ofstream o_BGsubtract;//("08_BGsubtract.txt");
	//ofstream o_Find_Object;//("09_Find_Object.txt");
	//ofstream o_Add_People_Blob;//("10_Add_People_Blob.txt");
	//ofstream o_Tracking;//("11_Tracking.txt");

	//==============
	// - Parameters
	//==============
	int peoples;													//记录有多少个人被抓到，最多不能超过Max_peoples
	int peoples_bef;												//记录"前一次"全部人数
	double gamma;													//用来预测移动向量的权重
	CvConnectedComp *temp_comp;										//记录每一个抓取出来Person的资讯
	int position[Max_peoples][4];									//记录前一次所有people的中心座标位置用来判别是否同一人
	//- [0,1]:(x,y). [2]:记录该位置是否使用过. [3]:记录该位置前一张Frame的[2]值.
	//pos[2] -> 0:未使用
	//          1:可能第一次加入or前一次加入但目前Frame未找到相同人员
	//			2:目前Frame有找到相同人员
	int position1[Max_peoples][8];									//-	[0,1]:(x0,y0). [2,3]:(x1,y1). [4,5]:(PMVx0,PMVy0). [6,7]:(PMVx1,PMVy1).
	Color *human_data, *human_data_tmp;								//储存前景人物的颜色资讯
	int count;														//计算现在的frame数,用於判断建立background的frame数

	//int aaa;														//测试停止点用参数
	IplImage *frame_copy, *frame_gray, *frame_edge, *frame_msg/*, frame_rotate */ ;
	IplImage *for_bHist, *for_gHist, *for_rHist;
	IplImage *Foreground, *Object, *Background, *Edge;

    //=============
	// - Functions
	//=============
	void SetImage(IplImage *);										// - Pre-Process Input Video/Image
	BOOL32 UnderInitializing();										// - Build the Code Book
	void MainProcess(IplImage *);									// - Main Detection Function
	BOOL32 Alarm();													// - Warning Detection Function
	void TrainCodebook(IplImage *, int, int, int);					// - Function1. Codebook建立
	void ReorderCodebook(int, int, int);							// - Function2. Reorder 
	void BGsubtract(int, int, u8 *,	u8 *, u8 *, u8 *, u8 *,IplImage *, int, int, int, int);							// - Function3. Background Subtraction 
	void Find_Object(IplImage *, int, int, int);					// - Function4. 找出前景人物位置资讯& 阴影去除法
	void Add_People_Blob(IplImage *, int, int);						// - Function5. 框出现在人物位置
	void Tracking(IplImage *);										// - Function6. Track人物资讯记录及清除等

private:
	int i, j, k, p, q, r, x, y, m, n;								//所有function使用变数
	int xxx, yyy, www, hhh;											//Set Image ROI
	int **cl;														//预设sequence resoluation
	int hist_size;													//使用於histogram计算
	Code ***codebook;

	//============================================
	//The parameters of the function, cvFloodFill
	//They are used in the function, Find_object
	//============================================
	CvPoint seed_point;
	CvScalar new_val;												//填充的前景颜色
	CvScalar lo_diff, up_diff;
	CvConnectedComp comp;
	CvFont font, font1;


	//==================================
	//原始用於Main Function之变数等宣告
	//==================================
	int dist;													//同一人移动范围上限
	int ctrl_time;												//定义判别为"滞留"的时间
	int people_area_min;										//define the min size of people region
	int people_area_max;										//define the max size of people region
	int x_I, y_I, width_I, height_I;							//rod coordinates
	int ThresholdV;												//阴影去除使用之限制值(目前不使用阴影去除法)

	int	StepF, StepR, StepG, StepB;

	//Background Subtraction Parameters
	u8 *dataSource, *dataForeground, *datarHistogram, *datagHistogram, *databHistogram;	

	int wait;													//cvWaitKey等待时间
	int ii, jj, kk, mm, pp;
	int count_fp;												//count_fp - 记录在前一Frame中有找到相同人员的总人数
	int tmp_dist[Max_peoples][3];								//暂存现在位置和Table中其他人之距离值			
	//[0, 1] : [x, y]的距离值.	[2] : 记录未排序前的原始Table位置编号
	int check;													//判别是否第一次建立人员资讯
	time_t nowTime, endTime, times[Max_peoples];				//计算时间用. times: 记录当下开始track时间
	int times1[Max_peoples];									//times1: 记录(曾经进入ROD)被侦测到滞留之次数
	int ROD_counter;											//ROD内现在的人数记录
	int people_rod[Max_peoples];
	char message[25];											//Message Image用显示
	CvPoint pt1, pt2;											//画线用
	float theta, sine, cosine;									//计算旋转角度用
	int new_width, new_height;									//Rotate图片大小
	CvRect h_rect;
	int count_sp[Max_peoples], count_cb[Max_peoples];			//Occlusion侦测判别为分裂或合并人数
};
