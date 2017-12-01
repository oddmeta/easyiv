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

#define MAX_ObjectNumber	10				//�����������������

#ifndef TYPE_OBJECTINFO
class EASYIV_API ObjectInformation
{
//�������ĸ�����Ϣ
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
/**** ������˵�� ****//*
				 (01) X:�����X���� (���½�)
				 (02) Y:�����Y���� (���½�)
				 (03) State:����Ŀǰ��״̬ ; 0����ʼ״̬
				 1���ҵ�����
				 2����������
				 -1��ȡ������
				 (04) Time:��¼�ҵ�����ʱ��ʱ���
				 (05) objWidth:����Ŀ��
				 (06) objHeight:����ĸ߶�
				 (07) cancel_object_flag:�ж��Ƿ�ִ��ȡ�����徯��Ĵ��� ; 0����ִ��
				 1��ִ��
				 (08) object_selection_flag:�ж��Ƿ�ѡ�񵽸����� ; 0����ִ��
				 1��ִ��
				 (09) Updata_CancelObject_count:ִ��ȡ������ʱ�������屻updata�ɱ����Ĵ���
				 (10) objImage:�������ͼ
				 (11) AlarmTime:��¼��������ʱ��ʱ���
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
	FUNCTION			:��ʼ��(����������Ƶ)
	*/
	BOOL32				UnderInitializing();

	/*
	FUNCTION			:�趨�����С   ֻ����size_width��size_height�������
	INPUT
	- size				:�����������Сsize
	- size_width		:�����������С���򳤶�
	- size_height		:�����������С���򳤶�
	*/
	void SetObjectSize(int size_width, int size_height);

	/*
	FUNCTION			:�趨��������ʱ������С   ע����Temporal TrackBar�����������alarmtime�й�
	INPUT
	- time_index		:��������ʱ������С
	*/
	void SetTimeInterval(int time_index);

	/*
	FUNCTION			:���Ǹò��÷�����
	INPUT
		CurFrame		:������Դ��Ƶ
		ptFrameBG		:���뱳����Ƶ
		ptFrameFG		:����ǰ����Ƶ
		x				:����X
		y				:����Y
	OUTPUT
		TRUE			��alarm
		FALSE			����alarm
	*/
	BOOL32 ObjectCase(IplImage* CurFrame, IplImage* ptFrameBG, IplImage* ptFrameFG, int x, int y);

	/*
	FUNCTION			:���������������������
	*/
	void Re_rankingObject();

	/*
	FUNCTION			:����������
	INPUT
	- index:��N������
	*/
	void ClearObjectInf(int index);

	/*
	FUNCTION			:�ж��Ƿ���ʾ����
	INPUT
	- CurFrame			:������Դ��Ƶ
	- m_ptFrameBG		:���뱳����Ƶ   
	- m_ptFrameFG		:����ǰ����Ƶ
	- IntervalTime		:���N���ʱ��
	*/
	void ShowAlarm(IplImage* CurFrame, IplImage* ptFrameBG, IplImage* ptFrameFG, int IntervalTime);

	/*
	FUNCTION			:ȡ��ǰ����Ƶ
	INPUT
	- sourceImg			:������Դ��Ƶ
	- backgroundImgModel:���뱳����Ƶ 
	- foregroundImg		:��Ŵ������ǰ����Ƶ 
	- background_thresh	:�ж�Ϊǰ����threshold
	*/
	void GetForeground(IplImage* sourceImg, IplImage* backgroundImgModel, IplImage* foregroundImg, int background_thresh);

	/*
	FUNCTION			:��������Ƶ�����
	INPUT
	- SourceImage		:������Դ��Ƶ
	- foregroundImg		:����ǰ����Ƶ
	- m_ptFrameBG		:��Ŵ�����ı�����Ƶ
	- IntervalFrame		:���������frameִ��һ��
	- select			:ģʽѡ��
		select=0 �� ʹ���ڳ�ʼ��������ģ��
		select=1 �� ʹ���ں��������һ��ʱ����±��� (ǰ��������)
	- alpha				:������ģ��ʱ��Ȩ��
	*/
	void UpdateBackgroundImage(IplImage *SourceImage, IplImage *foregroundImg, IplImage* ptFrameBG, int IntervalFrame, int select, double alpha);

	/*
	FUNCTION			:���쳣���岢��¼�������Ϣ
	INPUT
	- CurFrame			:������Դ��Ƶ
	- foregroundImg		:����ǰ����Ƶ   
	*/
	void FindObject(IplImage* CurFrame, IplImage* foregroundImg);

	/*
	FUNCTION			:�ֶ�ȡ������
	INPUT:
	- CurFrame			:������Դ��Ƶ
	- m_ptFrameBG		:���뱳����Ƶ
	- ObjNumber			:��N������
	- UpdataInterval	:����ͼ����µĴ���
	*/
	void HandCancelObject(IplImage* CurFrame, IplImage* ptFrameBG, int ObjNumber, int UpdataInterval);

	/*
	FUNCTION			:�Զ�ȡ������
	INPUT				:
	- CurFrame			:������Դ��Ƶ
	- m_ptFrameBG		:���뱳����Ƶ   
	- index				:��N������
	- m_Minute			:���N��
	- m_Second			:���N��
	- UpdataInterval	:����ͼ����µĴ���
	*/
	void AutomaticCancelObject(IplImage* CurFrame, IplImage* ptFrameBG, int index, int m_Minute, int m_Second, int UpdataInterval);

private:
	IplImage*	m_ptFrameCopy;					//��ǰframe
	IplImage*	m_ptFrameBG;					//����
	IplImage*	m_ptFrameFG;					//ǰ��
	IplImage*	m_ipmaxObjProb;					//Optical flow
	//IplImage*	m_ptFrameObjProbMax;			//����

	IplImage*	m_ptFrameLast;					//������ǰframe��Ϣ   �������������ж���
	IplImage*	m_ptFrameObjProb;				//���ͣ�Ͳ���ͳ�Ƶ�ֵ
#ifdef _ABNORMAL_DETECT_ENABLE_RESIZE_
	IplImage*	m_ptFrameResize;				//���resize image (320x240)
#endif

	// used for Find object
	IplImage*   m_ipdeNoiseForeground, *m_ipEditImage, *m_ipgCurFrame, *m_ipgLastFrame, *m_vex, *m_vey;
	//IplImage* m_ipldeNoiseForeground2,*m_ipleditImage, * m_iplgCurFrame,* m_iplgLastFrame,* vex,* vey,* maxObjProb;

	//used to UpdateBackground Image
	IplImage* m_ipTemp1, *m_ipTemp2;

	//used to Get Foreground Image
	IplImage* m_ipdeNoiseForeground1;		

	ObjectInformation ObjInf[MAX_ObjectNumber]; //�����������

private:
	TEZIVINFO_ABNORMAL	m_tAbnormalParam;

	int			m_nFrameCount;					//���㽨����(��ʼ��)ʱ��������frame����
	//int		m_nFrameCountTotal;				//��Alarm function ������������frame���� (�ݲ���)
	int			m_nUpdateCount;					//����frame��������ǰ������ (����������)
	int			m_nUpdateCountBG;				//����frame����������������
	int			m_nFrameRate; 					//Frame rate

	//IplImage *m_ptFrameCur;					//090813 added by Jacky Wei
};

#endif //_EZIV_ABNORMAL_DETECT_ATM_H_
