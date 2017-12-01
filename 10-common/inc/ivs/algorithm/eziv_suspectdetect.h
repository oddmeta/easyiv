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
#define Max_peoples		10							//����⵽����������ֵ
#define WWidth			320							//���������ӰƬ�ߴ�
#define HHeight			240  						//���������ӰƬ�ߴ�
#define W				20							//ROI��ROD��W���

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
	/*	��¼ִ��ʱ��	*/
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
	int peoples;													//��¼�ж��ٸ��˱�ץ������಻�ܳ���Max_peoples
	int peoples_bef;												//��¼"ǰһ��"ȫ������
	double gamma;													//����Ԥ���ƶ�������Ȩ��
	CvConnectedComp *temp_comp;										//��¼ÿһ��ץȡ����Person����Ѷ
	int position[Max_peoples][4];									//��¼ǰһ������people����������λ�������б��Ƿ�ͬһ��
	//- [0,1]:(x,y). [2]:��¼��λ���Ƿ�ʹ�ù�. [3]:��¼��λ��ǰһ��Frame��[2]ֵ.
	//pos[2] -> 0:δʹ��
	//          1:���ܵ�һ�μ���orǰһ�μ��뵫ĿǰFrameδ�ҵ���ͬ��Ա
	//			2:ĿǰFrame���ҵ���ͬ��Ա
	int position1[Max_peoples][8];									//-	[0,1]:(x0,y0). [2,3]:(x1,y1). [4,5]:(PMVx0,PMVy0). [6,7]:(PMVx1,PMVy1).
	Color *human_data, *human_data_tmp;								//����ǰ���������ɫ��Ѷ
	int count;														//�������ڵ�frame��,����жϽ���background��frame��

	//int aaa;														//����ֹͣ���ò���
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
	void TrainCodebook(IplImage *, int, int, int);					// - Function1. Codebook����
	void ReorderCodebook(int, int, int);							// - Function2. Reorder 
	void BGsubtract(int, int, u8 *,	u8 *, u8 *, u8 *, u8 *,IplImage *, int, int, int, int);							// - Function3. Background Subtraction 
	void Find_Object(IplImage *, int, int, int);					// - Function4. �ҳ�ǰ������λ����Ѷ& ��Ӱȥ����
	void Add_People_Blob(IplImage *, int, int);						// - Function5. �����������λ��
	void Tracking(IplImage *);										// - Function6. Track������Ѷ��¼�������

private:
	int i, j, k, p, q, r, x, y, m, n;								//����functionʹ�ñ���
	int xxx, yyy, www, hhh;											//Set Image ROI
	int **cl;														//Ԥ��sequence resoluation
	int hist_size;													//ʹ���histogram����
	Code ***codebook;

	//============================================
	//The parameters of the function, cvFloodFill
	//They are used in the function, Find_object
	//============================================
	CvPoint seed_point;
	CvScalar new_val;												//����ǰ����ɫ
	CvScalar lo_diff, up_diff;
	CvConnectedComp comp;
	CvFont font, font1;


	//==================================
	//ԭʼ���Main Function֮����������
	//==================================
	int dist;													//ͬһ���ƶ���Χ����
	int ctrl_time;												//�����б�Ϊ"����"��ʱ��
	int people_area_min;										//define the min size of people region
	int people_area_max;										//define the max size of people region
	int x_I, y_I, width_I, height_I;							//rod coordinates
	int ThresholdV;												//��Ӱȥ��ʹ��֮����ֵ(Ŀǰ��ʹ����Ӱȥ����)

	int	StepF, StepR, StepG, StepB;

	//Background Subtraction Parameters
	u8 *dataSource, *dataForeground, *datarHistogram, *datagHistogram, *databHistogram;	

	int wait;													//cvWaitKey�ȴ�ʱ��
	int ii, jj, kk, mm, pp;
	int count_fp;												//count_fp - ��¼��ǰһFrame�����ҵ���ͬ��Ա��������
	int tmp_dist[Max_peoples][3];								//�ݴ�����λ�ú�Table��������֮����ֵ			
	//[0, 1] : [x, y]�ľ���ֵ.	[2] : ��¼δ����ǰ��ԭʼTableλ�ñ��
	int check;													//�б��Ƿ��һ�ν�����Ա��Ѷ
	time_t nowTime, endTime, times[Max_peoples];				//����ʱ����. times: ��¼���¿�ʼtrackʱ��
	int times1[Max_peoples];									//times1: ��¼(��������ROD)����⵽����֮����
	int ROD_counter;											//ROD�����ڵ�������¼
	int people_rod[Max_peoples];
	char message[25];											//Message Image����ʾ
	CvPoint pt1, pt2;											//������
	float theta, sine, cosine;									//������ת�Ƕ���
	int new_width, new_height;									//RotateͼƬ��С
	CvRect h_rect;
	int count_sp[Max_peoples], count_cb[Max_peoples];			//Occlusion����б�Ϊ���ѻ�ϲ�����
};
