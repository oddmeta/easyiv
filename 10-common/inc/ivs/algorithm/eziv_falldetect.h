
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
	int x ; //��Բ���ĵ�x����
	int y ; //��Բ���ĵ�y����
	int x_t_1 ;//��Բ���ĵ���һʱ��x����
	int y_t_1 ;//��Բ���ĵ���һʱ��y����
	int x_t_2 ;  //��Բ���ĵ�ǰ��ʱ��x����
	int y_t_2 ;  //��Բ���ĵ�ǰ��ʱ��y����
	double RegionSize;  //��¼��ǰ�������pixel��
	double a ; //��Բ���᳤
	double a_1;  //��һʱ����Բ���᳤
	double b ; //��Բ���᳤
	double b_1;  //��һʱ����Բ���᳤
	double theta ; //��Բ��x�᷽��ļн�
	double theta_t_1 ;  //��һʱ�̵ļн�
	double theta_t_2 ;  //ǰʱ�̵ļн�
	//======================================

	//The foreground image and the motion history image=======
	IplImage* Img_MHI ; //Motion History Image
	IplImage* Img_peoples ;//The Foregrounde Image
	IplImage* Img_peoples_t_1 ;//The Foregrounde Image
	//========================================================

	//The conuter=====================================
	double C_motion ; //coefficient of motion
	int Fall_cof_count ; //�ۻ�����������frame��
	int Usual_cof_count ; //�ۻ����������frame��
	int cof_t_1 ;//��һʱ�̵�״̬,1-unusual, -1-usual
	long int fal_time ;  //������ά���ڵذ��ϵ�ʱ�䣬��������ʱ����δվ������Ϊ�����¼�
	//================================================

	//operators=======================================
	BOOL32 check_updata ;//�ж���Щ�����Ƿ��б����¹�, false = δ���¹�, true = ���¹�
	BOOL32 Fall_detected;//���е����¼�����Ϊtrue
	COEF_ELLIPSE* next_node;//���link list��������һ���ڵ���
	//================================================
};

// This class is exported from the eziv_falldetect.dll
class EASYIV_API CFallDetect
{
private:
	struct COEF_ELLIPSE* DataObj ;//������������ϴ���
	int nWidth  ;//the size of the frame
	int nHeight ;
	double pi   ;//����Բ����
	int count   ;//�������ڵ�frame��,����жϽ���background��frame��
	int UpdateCount ;//updata background frequence
	int people_area ;//define the size of people region

	int Fill_val ;//The parameters of the function, cvFloodFill
	CvPoint seed_point;//They are used in the function, Find_object
	CvScalar new_val  ;//���ǰ��ʱ����ֵ�������Ѱǰ��λ��
	CvScalar lo_diff  ;//�����ɫ����Ҫ�Ĳ���
	CvScalar up_diff  ;//�����ɫ����Ҫ�Ĳ���
	CvConnectedComp comp;//�����ǰ��������¼����Ѷ���ڴ�

	int peoples_detect_last_time ;//��¼��һʱ����⵽����Ա��
	int peoples_detect_this_time ;//��¼��ʱ����⵽����Ա��
	int coef_peoples_detect ;//��¼ǰ��detect��״̬

	double a ;//��Բ�Ķ��᳤
	double b ;//��Բ�ĳ��᳤
	double theta ;//��Բ��ˮƽ����ļн�

	int C_motionthr ;//�������ż�ֵ
	double Ratiothr ;//�������ȵ��ż�ֵ
	double Thetathr ;//��Բ��ˮƽ����нǵı仯���ż�ֵ
	double Center_Positionthr ;//���ĵ�仯���ż�ֵ

	int After_Falltim ;//������ά�ֵ�motion��ʱ�䳤���ż�,����жϵ����¼��ķ���
	int Fall_detect ;//���е����¼�����Ϊ1
	int Fall_num ;//��¼�����Ĵ���

public:
	IplImage *Foreground ;//ǰ��Ӱ��
	IplImage *Object ;//�ҳ����˵�Ӱ��
	IplImage *Background ;//����ģ��
	IplImage *frame_copy ;//�������Ӱ��
	CFallDetect(int ,int ,int ,int ) ;        //������������
	virtual ~CFallDetect() ;                       //�⹹��

public:
	void DoDetect(BOOL32, int, int, double);
	void UpdateBackgroundImage(int, int, double) ;// ������Ӱ�������
	void GetForeground(char*, int ) ;// ȡ��ǰ��Ӱ��
	void RGB_TO_YUV(u8 *,double* ,double* ,double *) ;//change the color space(RGB2YUV)
	void Find_Object() ;//find the position of the object,using opencv function, cvFloodFill
	void Ellipse_Parameter(IplImage*,int* ,int* ) ;//compute the parameters of the ellipse
	void Set_Node(IplImage* ,int ,int ,double) ;//set new node
	void Find_Match(struct COEF_ELLIPSE** ,int ,int ) ;//�������ж���ʱ���жϴ�ʱ������һʱ��match������
	void Updata_Node(struct COEF_ELLIPSE** ,IplImage* ,int ,int ,double );
	void Motion_History_Image(IplImage* ,IplImage* ,struct COEF_ELLIPSE* ) ;//compute the motion history image
	int Fall_Detect(struct COEF_ELLIPSE *) ;//Detect Falls
	int Alarm();//���е����¼�������Ѷ

	///////Add by Tsao ץ�������Թ�����/////////////////////
	double GetC_motionthr();
	void SetC_motionthr(int);

	double GetThetathr();
	void SetThetathr(int);

	double GetCt_Posthr();
	void SetCt_Posthr(int);

	///////Add by Tsao ��counterҲ�Ž�Ԫ������������޸�/////////////////////

	BOOL32 Ini_and_Det();
};