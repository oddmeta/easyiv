/*** ��ΪAbnormal Detect ��header file ***/

#include <stdio.h>
//#include <WinBase.h>
//#include <WTypes.h>

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#define  objectNumber  5 //��������������

#ifndef TYPE_OBJECTINFO
class EASYIV_API ObjectInformation
{
	//�������ĸ�����Ѷ
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
/**** ������˵��****//*
 (01) X:�����X����(���½�)
 (02) Y:�����Y����(���½�)
 (03) State:���Ŀǰ��״̬; 0����ʼ״̬
	 		 				 1���ҵ����
							 2����������
						    -1��ȡ�����
 (04) Time:��¼�ҵ����ʱ��ʱ���
 (05) objWidth:����Ŀ��
 (06) objHeight:����ĸ߶�
 (07) cancel_object_flag:�ж��Ƿ�ִ��ȡ���������Ĵ���; 0����ִ��
														  1��ִ��
 (08) object_selection_flag:�ж��Ƿ�ѡ�񵽸����; 0����ִ��
												   1��ִ��
 (09) Updata_CancelObject_count:ִ��ȡ�����ʱ���������updata�ɱ����Ĵ���
 (10) objImage:�������ͼ
 (11) AlarmTime:��¼��������ʱ��ʱ���
*/

#ifndef TYPE_ABNORMALDETECT
class EASYIV_API CAbnormalDetect
{
	//�쳣���������Ա�뺯ʽ
private:
    int nWidth, nHeight; //frame�Ŀ����
	int nSeconds1; //��ֹ������������������Ԥ�����
	int nSeconds2; //��ֹ�������������������쳣���
	int nSeconds3; //�쳣�����ѡ��������ȡ���ÿ�ѡ

	int mFrameCount;              //���㽨����(��ʼ��)ʱ��������frame����
	//int totalFrameCount;        //��Alarm function ������������frame����(�ݲ���)
	int UpdateCount;              //����frame��������ǰ������(����������)
	int BG_UpdateCount;           //����frame����������������
	int FrameIntervalCount;       //���������frameִ��һ��Optical flow
	int FrameIntervalCountValue;  //���ݶ�����frameִ��һ��Optical flow��threshold
	int WindowSize;               //�������ֵʱ��ߢȡ�������С
	int BoundarySpace;            //�趨�߽緶Χ


	int nFrameRate; 	         //Frame rate
	int Train_BgModel_number;    //���㽨���������frame����
	double weight;               //�����ʼʱ����ģ�͵�Ȩ��ֵ

	//IplImage *CurFrame; //090813 add
	//IplImage* maxObjProb;

	CvSize nScale; //ֻ�������ֵ������


public:
	IplImage* CurrentImage;     //��ǰframe
	IplImage* BackgroundImage;  //����
	IplImage* ForegroundImage;  //ǰ��
	IplImage* LastFrame;        //������ǰframe��Ѷ  �������������ж���
	IplImage* ObjcetProb;       //���ͣ�Ͳ���ͳ�Ƶ�ֵ
	IplImage* pImage1;          //���resize image (320x240)
	CAbnormalDetect(int, int, int, int, int); //������	
	~CAbnormalDetect(); //�⹹��

	ObjectInformation ObjInf[objectNumber]; //����������

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
	


	//void SetObjectSize(CvSize);  //�趨�����С
	void SetObjectSize(int,int); //�趨�����С  (����Spatial TrackBar�����������С)
	void SetTimeInterval(int); //�趨�趨��������ʱ������С(����Temporal TrackBar���������ʱ���С  ����AlarmTime �����)
	void SetImage(IplImage *); //�趨Ӱ������
	IplImage* GetImage(int);   //ȡ��Ӱ������

	BOOL32 UnderInitializing(); //��ʼ��(��������Ӱ��)
	BOOL32 Alarm(); //�ж��Ƿ��о���

	void UpdateBackgroundImage(IplImage *, IplImage *, IplImage*, int, int, double); //������Ӱ�������
	void GetForeground(IplImage*, IplImage*, IplImage*, int); //ȡ��ǰ��Ӱ��
    void FindObject(IplImage*, IplImage*); //���쳣�������¼�������Ѷ
	void ShowAlarm(IplImage*, IplImage*, IplImage*, int); //�ж��Ƿ���ʾ����
	void HandCancelObject(IplImage* , IplImage* , int , int); //�����ֶ�ȡ�����ʱ�ĳ���
	void AutomaticCancelObject(IplImage*, IplImage*, int, int, int, int); //�����Զ�ȡ�����ʱ�ĳ���
	void ClearObjectInf(int); //����������
	void Re_rankingObject(); //���������������������

	BOOL32 ObjectCase(IplImage*, IplImage*, IplImage*, int, int); //���Ǹò��÷�����

};
#define TYPE_ABNORMALDETECT
#endif

