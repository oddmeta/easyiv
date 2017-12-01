
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
	CvFont font;//���͸�ʽ
	char *PrintfString;//�ִ��ݴ�Buffer

	CvMat *maskV;//��ֱaverage filter
	CvMat *maskH;//ˮƽaverage filter

	BOOL32 *Focus_DuringTime;//��¼Focus_Blur��ʱ��
	BOOL32 *Image_DuringTime;//��¼Image_Blur��ʱ��

	float Focus_Blur_Parameter; //����ʧ������
	float Image_Blur_Parameter; //����ģ������

	double time_count;    //ʱ������ۼ�ֵ
	time_t pre_time; //��ʱʹ��
	BOOL32 Time_Difference;//ʱ���в���ʱ�����
	double initial_time;//��ʹʱ���趨
	int Alarm_time;//Alarm�ڶ���ʱ����ȥ����
	float Alarm_time_percent;//ʱ����ģ������������ٰٷֱȼ�������Ѷ
	BOOL32 initial_flag;//��ʹ���,�Ƿ�initialʹ��

	int Average_filter_size;//ƽ��filter��size
	int edge_threshold;//����edge��ʹ�õ�threshold
	int sobel_filter_size;//����edge ��ʹ�õ�sobel filter size
	float Min_Focus_Parameter;//��¼��СFocus_Parameter normalize��
	float Min_Image_Parameter;//��¼��СImage_Parameter normalize��
	float Focus_maximum;//Focus_Parameter���ֵԤ��Ϊ
	float Image_maximum;//Image_Parameter���ֵΪ����Ԥ��Ϊ
	float Focus_percent_threshold;//Focus_Parameter ��threshold
	float Image_percent_threshold;//Image_Parameter ��threshold

	int Focus_NowNum;//��¼ĿǰFocus_DuringTime����λ��
	float Focus_frame_number;//ĿǰFocus������Frame Number
	float Focus_Blur_number;//ĿǰFocus����ģ����Frame Number
	float Focus_time_count;//Ŀǰ�ж���ʱ������Focus Blur

	int Image_NowNum;//��¼ĿǰImage_DuringTime����λ��
	float Image_frame_number;//ĿǰImage������Frame Number
	float Image_Blur_number;//ĿǰImage����ģ����Frame Number
	float Image_time_count;//Ŀǰ�ж���ʱ������Image Blur


	BOOL32 Focus_Blur_Alarm;//����ʧ���������
	BOOL32 Image_Blur_Alarm;//����ģ���������
	BOOL32 Blur_Alarm;//��ģ��״̬���

public:

	IplImage *gray;    //�������Frameת�ɻҽ�
	IplImage *edge;    //����ҽ�Ӱ��ı߽�ֵ
	IplImage *blurV;   //��ֱģ��Ӱ��
	IplImage *blurH;   //ˮƽģ��Ӱ��
	IplImage *frame_msg;   //�����Ѷͼ��

	CBlurDetect(int frame_Width,int frame_Height,int edge_thr,int sobel_size,int filter_size,double initial_second,float Focus_max_multiple,float Image_max,
		float Focus_threshold,float Image_threshold,int Alarm_second,float Alarm_percent);//��ʼ��
	void SetImage(IplImage *frame,time_t now_time);//frame input ���ܹ�����
	float FocusBlurFeature(IplImage *gray);//���㻭��ʧ������
	float ImageBlurFeature(IplImage *gray);//���㻭��ģ������
	BOOL32 UnderInitializing();//�Ƿ�Ϊ��ʹ�ĺ���
	BOOL32 Focus_Blur_time(BOOL32 Focus_Result);//Focus Blurʱ���Ƿ񳬹�threshold
	BOOL32 Image_Blur_time(BOOL32 Image_Result);//Image Blurʱ���Ƿ񳬹�threshold
	void SetParameter(int edge_thr,int sobel_size,int filter_size,double initial_second,float Focus_max_multiple,float Image_max,
		float Focus_threshold,float Image_threshold,int Alarm_second,float Alarm_percent);//�趨��������
	void ReInitializing();//���³�ʼ������
	BOOL32 Alarm();//�Ƿ���blur
	~CBlurDetect();//�⹹���ͷż�����
};
