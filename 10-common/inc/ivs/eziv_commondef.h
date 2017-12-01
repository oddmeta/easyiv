#ifndef __COMMON_DEFINITION_INCLUDE
#define __COMMON_DEFINITION_INCLUDE

#include <rstype.h>
#include <iostream>

#include "opencv2/core/types_c.h"

#include "ivs/eziv_log.h"
#include "ivs/eziv_config.h"

#define EZIV_MAX_STREAM_SOURCE			8
#define EZIV_MAX_RULE_PER_SOURCE		4
#define EZIV_MAX_DETECT_RESULT_AREA		12

#define EZIV_MAX_SEARCH_OBJ				12
#define EZIV_MAX_INTRUDE_DETECT_OBJS	10

#define EZIV_STREAMING_START_PORT		50000

#define EZIV_CONF_SYS				"conf\\eziv_system.xml"
#define EZIV_CONF_VIDLIST			"conf\\eziv_vidlist.xml"
#define	EZIV_CONF_CMSLIST			"conf\\eziv_cms.xml"

#define	STR_MALICIOUS				"CCD Mask"
#define	STR_LOST					"LOST DETECT"
#define	STR_ABNORMAL				"Abnormal Object Detection"
#define	STR_ATM_ABNORMAL			"ATM Abnormal Detection"
#define	STR_SUSPECT					"Suspect Detection"
#define STR_FACE					"Face Detection"
#define STR_FACE_MASK				"Face Mask Detection"
#define STR_FALL					"Fall Detection"
#define STR_INTRUDER				"Intruder Detection"
#define STR_HUMAN_COUNT				"Human Count Detection"
#define STR_SMART_SEARCH			"Smart Search Detection"
#define STR_BLUR					"Blur Detect"


/*
#ifndef TYPE_CAMINFO
typedef struct T_CAMINFO
{
	u16			m_wConnectionType;
	u16			m_wProtocalType;
	s8			m_szProductLib[32];
	u16			m_wCameraType;
	u16			m_wDomeType;
	u16			m_wNTSCPAL;
	s8			m_szDispServerIP[64];
	u16			m_wDispServerPort;
	s8			m_szServerIP[64];
	u16			m_wServerPort;
	s8			m_szCameraIP[64];
	u16			m_wCameraPort;
	u32			m_wDeviceID;
	u16			m_wChannelNumber;
	u16			m_wCompressionType;
	union
	{
		u16			m_wImageSize;			//used to store enum STREAM_SOURCE_TYPE
	};
	u16			m_wRotation;
	s8			m_szLocation[32];
	s8			m_szNodeKey[32];
	s8			m_szAccount[16];
	s8			m_szPassword[16];
	s8			m_szAuthUser[16];
	s8			m_szAuthPass[16];
	s8			m_szConnParam[256];
} CAMINFO;
#define TYPE_CAMINFO
#endif
*/

#ifndef SYSTEMTIME__
	typedef struct T_SYSTEMTIME {
		u16 	wYear;
		u16 	wMonth;
		u16 	wDayOfWeek;
		u16 	wDay;
		u16 	wHour;
		u16 	wMinute;
		u16 	wSecond;
		u16 	wMilliseconds;
	} SYSTEMTIME_;

	typedef struct T_FILETIME_
	{
		u32 dwLowDateTime;
		u32 dwHighDateTime;
	}FILETIME_;

	#define SYSTEMTIME__
#endif

#ifndef TYPE_CAMINFO
	typedef struct T_CAMINFO
	{
		u16			m_wConnectionType;
		u16			m_wProtocalType;
		s8			m_szProductLib[32];
		u16			m_wCameraType;
		union
		{
			u16		m_emStreamSourceType;	//enum STREAM_SOURCE_TYPE
			u16		m_wDomeType;
		};
		u16			m_wNTSCPAL;
		s8			m_szDispServerIP[64];
		u16			m_wDispServerPort;
		s8			m_szServerIP[64];
		u16			m_wServerPort;
		s8			m_szCameraIP[64];
		u16			m_wCameraPort;
		u32			m_wDeviceID;
		u16			m_wChannelNumber;
		u16			m_wCompressionType;
		union
		{
			u16		m_wImageSize;				//enum STREAM_TYPE
			u16		m_emStreamMainDual;			//enum STREAM_TYPE
		};
		u16			m_wRotation;
		s8			m_szLocation[32];
		s8			m_szNodeKey[32];
		s8			m_szAccount[16];
		s8			m_szPassword[16];
		s8			m_szAuthUser[16];
		s8			m_szAuthPass[16];
		union
		{
			s8		m_szConnParam[MAX_PATH];
			s8		m_szMediaFile[MAX_PATH];
		};
	} CAMINFO;
	#define TYPE_CAMINFO
#endif


enum EM_ROI_STATUS
{
	emRoiUpdate = 0,
	emRoiInitializing,
	emRoiDetecting,
	emRoiAlarmReport,
};

typedef struct tagEZIVDbgVideo
{
	BOOL32	m_bShowBackground;
	BOOL32	m_bShowForeground;
	BOOL32	m_bShowProceeding;
	BOOL32	m_bOthers;
}EZIVDbgVideo;

typedef struct tagEZIVDbgOutput
{
	BOOL32	m_bShowEncFrame;
	BOOL32	m_bShowDecFrame;
	BOOL32	m_bShowFrameProcInfo;
	EZIVDbgVideo m_tDbgVideo;
}EZIVDbgOutput;

enum EM_EZIV_RULE_TYPE
{
	EM_EZIV_RULE_INVALID				= 0,
	EM_EZIV_RULE_ABNORMAL_DETECT_ATM	= 1024,	//1024
	EM_EZIV_RULE_ABNORMAL_DETECT,
	EM_EZIV_RULE_FACE_DETECT,
	EM_EZIV_RULE_INTELLI_SEARCH,
	EM_EZIV_RULE_INTRUDER_DETECT,
	EM_EZIV_RULE_HUMAN_COUNT,
	EM_EZIV_RULE_FACE_MASK,
	EM_EZIV_RULE_FALL_DETECT,
	EM_EZIV_RULE_LOST_DETECT,
	EM_EZIV_RULE_MALICIOUS,
	EM_EZIV_RULE_CCDMASK = EM_EZIV_RULE_MALICIOUS,
	EM_EZIV_RULE_SUSPECT_DETECT,
	EM_EZIV_RULE_BLUR_DETECT,
	EM_EZIV_RULE_FACE_FEATURE,

	//Customized New Modules
	EM_EZIV_RULE_ABNORMAL			= 0x1000,
	EM_EZIV_RULE_SUSPECT,
};

typedef struct tagEventType
{
	EM_EZIV_RULE_TYPE		m_emRuleType;
	s8		m_szEventType[3];
	s8		m_szIVEventDesc[33];
}TEVENTTYPE;

enum EM_EZIV_IPLIMGTYPE
{
	EM_EZIV_IPLIMG_SHOW = 1,
	EM_EZIV_IPLIMG_BACKGROUD,
	EM_EZIV_IPLIMG_FOREGROUD,
	EM_EZIV_IPLIMG_OPTICALFLOW,
};

enum EM_EZIV_IMAGE_ENHANCE
{
	EM_EZIV_IMAGE_ENHANCE_MODE_NONE = 0,	//0 NONE
	EM_EZIV_IMAGE_ENHANCE_GAMA,				//1 gama У��  
	EM_EZIV_IMAGE_ENHANCE_HISTOGRAM,		//2 histogram equaliration 
	EM_EZIV_IMAGE_ENHANCE_GAMA_AND_HIST,	//3 gama У��+ histogram equaliration
};

enum EM_DETECT_AREA_TYPE
{
	EM_DETECT_AREA_TYPE_ROI	= 0,
	EM_DETECT_AREA_TYPE_ROD,
	EM_DETECT_AREA_TYPE_ROAD,
	EM_DETECT_AREA_TYPE_ALARM,
};

typedef struct tagEZIVRoi
{
	EM_DETECT_AREA_TYPE m_emAreaType;
	CvRect		m_tRect;
}TEZIVRoi;

typedef struct tagEZIVArea
{
	f64			area;
	s32			width;
	s32			height;
}TEZIVArea;

typedef struct tagROIINFO
{
	CvRect		m_tROIRect;
	TEZIVRoi	m_vROISize;
	int			m_nInterval;
	int			m_nIntervalCancel;
	int			m_nPattern;
	int			m_nThreshold;
	int			m_nFrequency;
	int			m_nMod;
	f32			m_fScale;
	TEZIVRoi	m_vRoadInfo;			// temp for Intruder Detect Only, should we move to another method to set road info???
	BOOL32		m_nBSensitive;
	BOOL32		m_nFSensitive;
}TEZIVRoiInfo;

typedef struct T_IV_TYPE
{
	EM_EZIV_RULE_TYPE	m_emIVType;
	s8			m_szIVType[32];
}IV_TYPE;


//////////////////////////////////////////////////////////////////////////
//z Detect results
//////////////////////////////////////////////////////////////////////////
enum EM_EZIV_INTELLISEARCH_TYPE
{
	EM_EZIV_INTELLISEARCH_HUMAN		= 1,
	EM_EZIV_INTELLISEARCH_VEICHLE	= 2,
};
typedef struct tagEZIV_HUMAN_SEARCH
{
	s32			m_nIn;
	s32			m_nOut;
	BOOL32		m_bFaceDetected;
}TEZIV_HUMAN_SEARCH;

typedef struct tagEZIV_SEARCH_RESULT
{
	s32 m_nObjectNum;

	TEZIV_HUMAN_SEARCH m_atHumanSearch[EZIV_MAX_SEARCH_OBJ];
	/*
	//�����뿪
	ObjectNum=Search.ReturnObjectNum();
	cout<<"out object num:"<<ObjectNum<<'\n';
	for(i=0;i<ObjectNum;i++)
	{
		//�������
		cout<<"Total:"<<total<<'\n';
		cout<<"object Num:"<<ObjectNum<<	'\n';
		Search.ReturnData(& In,& Out, &FaceDetect);
		cout<<"InFrameNum:"<<In<<'\n';
		if(FaceDetect)
			cout<<"FaceDetect!!"<<'\n';
		cout<<"OutFrameNum:"<<Out<<'\n';
		total++;
	}s
	*/
}TEZIV_SEARCH_RESULT;
typedef struct tagEZIV_HUMAN_COUNT_RESULT
{
	int			m_nInNumber;
	int			m_nOutNumber;
}TEZIV_HUMAN_COUNT_RESULT;

#define IDENTIFIER_MAGIC_EZIV_DETECT_RESULT 0x44332211

typedef struct tagEZIV_DETECT_RESULT
{
	tagEZIV_DETECT_RESULT()
	{
		memset(this, 0, sizeof(*this));
		m_dwMagic = IDENTIFIER_MAGIC_EZIV_DETECT_RESULT;
		m_dwSize = sizeof(*this);
	}
	u32			m_dwMagic;
	u32			m_dwSize;
	EM_EZIV_RULE_TYPE	m_emEZIVType;
	s32			m_nDetectResults;
	s32			m_nDetectWidth;		//Input image width, maybe resized before input IV modules
	s32			m_nDetectHeight;	//Input image height, maybe resized before input IV modules
	TEZIVRoi	m_atAlarmArea[EZIV_MAX_DETECT_RESULT_AREA];
	SYSTEMTIME_	m_tAlarmStartTime;
	SYSTEMTIME_	m_tAlarmEndTime;
	union
	{
		TEZIV_HUMAN_COUNT_RESULT	m_tRetHumanCount;
		TEZIV_SEARCH_RESULT			m_tRetIntelliSearch;
	};
}TEZIV_DETECT_RESULT;
//End of Detect Results
//////////////////////////////////////////////////////////////////////////

typedef struct tagEZIVGAMATrans
{
	int m_nGamaStandRand;
	int m_nGamaValue;		//Value range 1, 10
	int m_nGamaShift;
}TEZIVGamaTrans;

typedef struct tagEZIVHistEqualize
{
	int				m_nPixelType;
	int				m_nHistoMinPixel;	//Value range 1, 255
	int				m_nHistoMaxPixel;	//Value range 1, 255
}TEZIVHistEqualize;

//////////////////////////////////////////////////////////////////////////
// Setting related structure definitions
//////////////////////////////////////////////////////////////////////////
typedef struct tagEZIVINFO_FACEDETECT
{
	int				m_nFrameWidth;
	int				m_nFrameHeight;
	s8				m_achConfPath[256];

	TEZIVRoi		m_tROI;
	double			scale;							//�Ƿ���С�����ȥ�ң�scale=1.3��
	TEZIVArea		tMinArea;						//������������ǰ�������С, ��������ǰ��������С���Ϊ400��20*20Ϊ��С����⵽��������С��
	int				m_nBackgroundSamples;			//Jacky, ��ʼ����������֡�� default value 20 

	int				m_nObject;				// for face detect

	//���ߵ�������start
	EM_EZIV_IMAGE_ENHANCE	m_emEnhanceMode;
	TEZIVHistEqualize	m_tHistEqualize;
	TEZIVGamaTrans		m_tGamaTrans;
	//double			parameter1;
	//double			parameter2;
	double			m_dSearchPecent;
	//���ߵ�������end

	//Debug output
	//EZIVDbgVideo m_tDbgVideo;
}TEZIVINFO_FACEDETECT;

typedef struct tagEZIVINFO_FALL
{
	int				m_nMotionthr;			// fall detect
	int				m_nThetathr;			// fall detect
	int				m_nCenterPositionthr;	// fall detect
}TEZIVINFO_FALLDETECT;

typedef struct tagEZIVINFO_FACEFEATURE
{
	int			m_nFrameWidth;
	int			m_nFrameHeight;
	s8			m_achConfPath[256];

	TEZIVRoi	m_tROI;
	double		scale;							//�Ƿ���С�����ȥ�ң�scale=1.3��
	double		face_area;						//������������ǰ�������С, ��������ǰ��������С���Ϊ400��20*20Ϊ��С����⵽��������С��
	int			m_nBackgroundSamples;			//Jacky, ��ʼ����������֡�� default value 20 

	//���ߵ�������start
	EM_EZIV_IMAGE_ENHANCE	Mode;
	double		parameter1;
	double		parameter2;
	double		search_range;
	//���ߵ�������end

	//Debug output
	//EZIVDbgVideo m_tDbgVideo;
}TEZIVINFO_FACEFEATURE;

typedef struct tagEZIVINFO_HUMANCOUNT
{
	int			m_nFrameWidth;
	int			m_nFrameHeight;
	s8			m_achConfPath[256];

	TEZIVRoi	m_tROI;
	double		scale;							//�Ƿ���С�����ȥ�ң�scale=1.3��
	double		face_area;						//������������ǰ�������С, ��������ǰ��������С���Ϊ400��20*20Ϊ��С����⵽��������С��
	int			m_nBackgroundSamples;			//Jacky, ��ʼ����������֡�� default value 20 

	//���ߵ�������start
	EM_EZIV_IMAGE_ENHANCE	Mode;
	double		parameter1;
	double		parameter2;
	double		search_range;
	//���ߵ�������end

	//Debug output
	//EZIVDbgVideo m_tDbgVideo;
}TEZIVINFO_HUMANCOUNT;

enum EM_EZIV_INTELLISEARCH_MODE
{
	EM_EZIV_INTELLISEARCH_MODE_DEFAULT = 0,
	EM_EZIV_INTELLISEARCH_MODE_FACETRACKONLY,		//ֻ��Ѱ�����г��ֶ���
};

typedef struct tagEZIVINFO_INTELLISEARCH
{
	int					m_nFrameWidth;
	int					m_nFrameHeight;
	s8					m_achConfPath[MAX_PATH];
	int					m_nMaxObjectInFrame;				//������ڻ����б��ҵ��������Ŀ
	BOOL32				m_bFaceOnly;						//�Ƿ�ֻ�ش����������¼�
	EM_EZIV_INTELLISEARCH_MODE m_emSearchMode;					//�л�����ģʽ

	//ROI
	CvPoint				m_tROI_ltpt;
	CvPoint				m_tROI_rbpt;						//ROI���ϵ������µ�
	CvRect				m_tROIRect;							//�趨ROI��С

	//���ߵ�������start
	EM_EZIV_IMAGE_ENHANCE	m_emEnhanceMode;
	TEZIVHistEqualize	m_tHistEqualize;
	TEZIVGamaTrans		m_tGamaTrans;
	double				m_dSearchPecent;
	//���ߵ�������end
}TEZIVINFO_INTELLISEARCH;

typedef struct tagEZIVINFO_SUSPICIOUS
{
	int			m_nFrameWidth;
	int			m_nFrameHeight;
	s8			achConfPath[256];
}TEZIVINFO_SUSPICIOUS;

enum EM_EZIV_INTRUDER_DIRECTION
{
	EM_EZIV_INTRUDER_IN = 1,
	EM_EZIV_INTRUDER_OUT,
};

typedef struct tagEZIVINFO_LOSTDETECT
{
	int			m_nFrameWidth;
	int			m_nFrameHeight;
	s8			m_achConfPath[MAX_PATH];

	CvRect		m_tROI;
	int			m_nHistDimSize;			//Histogram dims size, default 16
	float		m_afHistRangesArr[2];	//Histogram ranges array, default {0, 180}
	int			m_nAlarmTimer;			//Alarm triggered after object lost for N seconds, default 5 seconds
	int			m_nThreshold;
    bool        m_bResizeImg;
}TEZIVINFO_LOSTDETECT;

typedef struct tagEZIVINFO_INTRUDER
{
	int			m_nFrameWidth;
	int			m_nFrameHeight;
	s8			m_achConfPath[MAX_PATH];

	//TEZIVRoi	m_tROI;
	CvRect		m_tROI;
	int			m_nThreshold;				//threshold of background substraction
	float		m_fPatternScale;			//pattern scale value

	BOOL32		m_bSetRoadOn;
	BOOL32		m_bSetThresholdOn;
	BOOL32		m_bSetRoiOn;

	struct tagIntrudeROAD 
	{
		CvPoint m_tWallPoint0;
		CvPoint m_tWallPoint1;

		//���ߵ�����
		s32 x1;
		s32 y1;
		s32 x2;
		s32 y2;

		//y=nA*x+nB
		double nA;
		double nB;
	} m_tROADInfo;

	double		scale;							//�Ƿ���С�����ȥ�ң�scale=1.3��
	double		face_area;						//������������ǰ�������С, ��������ǰ��������С���Ϊ400��20*20Ϊ��С����⵽��������С��
	int			m_nBackgroundSamples;			//Jacky, ��ʼ����������֡�� default value 20
}TEZIVINFO_INTRUDER;

typedef struct tagEZIVINFO_ABNORMAL
{
	s32			m_nFrameWidth;
	s32			m_nFrameHeight;
	s8			m_achConfPath[256];

	BOOL32		m_bResizeImg;
	int			nSeconds1;					//��ֹ������������������Ԥ������
	int			nSeconds2;					//��ֹ�������������������쳣����
	int			nSeconds3;					//�쳣�����ѡ�������ȡ���ÿ�ѡ
	int			FrameIntervalCount;			//���������frameִ��һ��Optical flow
	int			FrameIntervalCountValue;	//���ݶ�����frameִ��һ��Optical flow��threshold
	int			WindowSize;					//�������ֵʱ��ߢȡ�������С
	int			BoundarySpace;				//�趨�߽緶Χ
	int			Train_BgModel_number;		//���㽨���������frame����
	double		weight;						//�����ʼʱ����ģ�͵�Ȩ��ֵ
	s32			nObjectNumber;				//���������������
	CvSize		nScale;						//ֻ�������ֵ�������

	//Debug output
	//EZIVDbgVideo m_tDbgVideo;
}TEZIVINFO_ABNORMAL;

//////////////////////////////////////////////////////////////////////////
//EZIV_RULE_CONFIG
//////////////////////////////////////////////////////////////////////////
/*
typedef struct tagEVIZRuleConf
{
	s32			m_nFrameWidth;
	s32			m_nFrameHeight;
	s8			m_achConfPath[256];

	union
	{
		TEZIVINFO_ABNORMAL		m_tAbnomal;
		TEZIVINFO_FACEDETECT		m_tFace;
		TEZIVINFO_LOSTDETECT	m_tLost;
		TEZIVINFO_HUMANCOUNT		m_tHumanCount;
		TEZIVINFO_INTELLISEARCH	m_tIntelliSearch;
		TEZIVINFO_INTRUDER		m_tIntruder;
		TEZIVINFO_FACEFEATURE	m_tFaceFeature;
		TEZIVINFO_SUSPICIOUS		m_tSuspicious;
	};
}TEZIVRULECONF;
*/
typedef struct tagEZIVRuleCfg
{
	s8					m_achDeviceGUID[32];

	u16					m_wWidth;			//Real image width
	u16					m_wHeight;			//Real image height
	EM_EZIV_RULE_TYPE	m_emRuleType;		//Rule Type ID
	//s8				m_achRuleType[32];	//Rule Type Str
	BOOL32				m_bDefaultSetting;	//Are we using default rule setting?
	BOOL32				m_bNewRule;			//Indicator of a new rule(an updated rule?)
	int					m_nActivate;

	int					m_nFrequency;

	//TEZIVRoiInfo		m_tRoiInfo;
	//TEZIVRoi			m_atROI[4];

	s32					m_nConfLen;		//Config buf length
	union
	{
		u8							m_abyConf[32<<10];	//TEZIVINFO_HUMANCOUNT
		TEZIVINFO_LOSTDETECT		m_tConfLostDetect;
		TEZIVINFO_FACEDETECT		m_tConfFaceDetect;
		TEZIVINFO_INTELLISEARCH		m_tConfIntelliSearch;
		TEZIVINFO_HUMANCOUNT		m_tConfHumanCount;
		TEZIVINFO_FACEFEATURE		m_tConfFaceFeature;
		TEZIVINFO_ABNORMAL			m_tConfAbnomal;
		TEZIVINFO_INTRUDER			m_tConfIntruder;
		TEZIVINFO_SUSPICIOUS		m_tConfSuspicious;
	};
}TEZIVRuleCfg;

typedef struct tagEZIVStreamCfg
{
	CAMINFO			m_vCamInfo;			// Source info
	s32				m_nTotalRules;		// Total rules for this source
	BOOL32			m_bEnabled;			// This source is enabled
	BOOL32			m_bFinished;		// This source has been finished processing
	TEZIVRuleCfg	m_atRule[EZIV_MAX_RULE_PER_SOURCE];
}TEZIVStreamCfg;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//�¼���������
typedef struct tagNET_DVR_SEARCH_EVENT_PARAM
{
	u16 wMajorType;					//0-�ƶ���⣬1-��������, 2-�����¼�
	u16 wMinorType;					//����������- ���������ͱ仯��0xffff��ʾȫ��
	SYSTEMTIME_ struStartTime;		//�����Ŀ�ʼʱ�䣬ֹͣʱ��: ͬʱΪ(0, 0) ��ʾ�������ʱ�俪ʼ���������ǰ���4000���¼�
	SYSTEMTIME_ struEndTime;		//
	u8			byLockType;			// 0xff-ȫ����0-δ����1-����
	u8 			byValue;			//0-��λ��ʾ��1-��ֵ��ʾ
	u8 			byRes[130];			// ����
	union	
	{
		u8 byLen[256];				//SEARCH_EVENT_INFO_LEN
		struct//��������
		{
			u8 byAlarmInNo[32];		//MAX_ALARMIN_V30 ��������ţ�byAlarmInNo[0]����1���ʾ�����ɱ�������1�������¼�
			u8 byRes[64-32];		//SEARCH_EVENT_INFO_LEN - MAX_ALARMIN_V30
		}struAlarmParam;

		struct//��������--��ֵ
		{
			u16 wAlarmInNo[128];	// ��������ţ���ֵ��ʾ,���ý���������
			u8 byRes[44];			// Reserved 
		}struAlarmParamByValue;

		struct//�ƶ����
		{
			u8 byMotDetChanNo[32];	//MAX_CHANNUM_V30 �ƶ����ͨ����byMotDetChanNo[0]����1���ʾ������ͨ��1�����ƶ���ⴥ�����¼�
			u8 byRes[64 - 32];		//SEARCH_EVENT_INFO_LEN - MAX_CHANNUM_V30
		}struMotionParam;

		struct//�ƶ����--��ֵ
		{
			u16 wMotDetChanNo[64];	// �ƶ����ͨ������ֵ��ʾ ,���ý���������
			u8 byRes[172];			// Reserved
		}struMotionParamByValue;

		struct//��Ϊ����
		{
			u8 byChanNo[32];		//MAX_CHANNUM_V30 �����¼���ͨ��
			u8 byRuleID;			//����ID��0xff��ʾȫ��
			u8 byRes1[43];			//����
		}struVcaParam;

		struct //��Ϊ����--��ֵ��ʽ���� 
		{
			u16 wChanNo[64];		//��Ϊ������Ӧ��ͨ������ֵ��ʾ,���ý���������
			u8 byRuleID;			//��Ϊ�������ͣ�����0xff��ʾȫ������0��ʼ
			u8 byRes[171];			//����
		}struVcaParamByValue;

		struct//��Ѷ�¼���������
		{
			u8 byRoomIndex;    //��Ѷ�ұ��,��1��ʼ
			u8 byRes[299];     //����
		} struInquestParam;
		struct  //��������������
		{
			u8 byChan [256];//������������ͨ���ţ��������±��ʾ��byChan[0]����1���ʾ������ͨ��1�����ƶ���ⴥ�����¼� 
		}struVCADetectByBit;

		struct//�������������� ��ͨ���Ű�ֵ��ʾ
		{
			u32 dwChanNo[32-1];//MAX_CHANNUM_V30 ����ͨ����,��ֵ��ʾ��0xffffffff��Ч���Һ�������Ҳ��ʾ��Чֵ
			u8 byAll;//0-��ʾ����ȫ����1-��ʾȫ����
			u8 byres[3];
		}struVCADetectByValue;
		/*
		struct
		{
			NET_DVR_STREAM_INFO struIDInfo; // ��id��Ϣ��72�ֽڳ�
			u32				dwCmdType;  // �ⲿ�������ͣ�NVR�����ƴ洢ʹ��
			u8				byBackupVolumeNum; //�浵��ţ�CVRʹ��
			u8				byRes[223];
		}struStreamIDParam;
		*/
	}uSeniorParam;
}NET_DVR_SEARCH_EVENT_PARAM, *LPNET_DVR_SEARCH_EVENT_PARAM;

//���ҷ��ؽ��
typedef struct tagEZIV_SEARCH_EVENT_RET
{
	u16 wMajorType;					//������
	u16 wMinorType;					//������
	SYSTEMTIME_	struStartTime;		//�¼���ʼ��ʱ��
	SYSTEMTIME_ struEndTime;		//�¼�ֹͣ��ʱ�䣬�����¼�ʱ�Ϳ�ʼʱ��һ��
	u8 byChan[32];					//MAX_CHANNUM_V30
	u8 byRes[36];
	union
	{
		struct//����������
		{
			u32 dwAlarmInNo;		//���������
			u8 byRes[32];			//SEARCH_EVENT_INFO_LEN
		}struAlarmRet;
		struct//�ƶ������
		{
			u32 dwMotDetNo;			//�ƶ����ͨ��
			u8 byRes[32];			//SEARCH_EVENT_INFO_LEN
		}struMotionRet;
		struct//��Ϊ�������  
		{
			u32 dwChanNo;					//�����¼���ͨ����
			u8 byRuleID;					//����ID
			u8 byRes1[3];					//����
			u8 byRuleName[MAX_PATH];		//��������
			EM_EZIV_RULE_TYPE uEvent;				//NET_VCA_EVENT_UNION uEvent;     //��Ϊ�¼�����
		}struVcaRet;
		struct//��Ѷ�¼�
		{
			u8  byRoomIndex;     //��Ѷ�ұ��,��1��ʼ
			u8  byDriveIndex;    //��¼�����,��1��ʼ
			u8  byRes1[6];       //����
			u32 dwSegmentNo;     //��Ƭ���ڱ�����Ѷ�е����,��1��ʼ 
			u16  wSegmetSize;     //��Ƭ�ϵĴ�С, ��λM 
			u16  wSegmentState;   //��Ƭ��״̬ 0 ��¼������1 ��¼�쳣��2 ����¼��Ѷ
			u8  byRes2[288];     //����
		}struInquestRet;
		struct //��id¼���ѯ���
		{
			u32 dwRecordType;	//¼������ 0-��ʱ¼�� 1-�ƶ���� 2-����¼�� 3-����|�ƶ���� 4-����&�ƶ���� 5-����� 6-�ֶ�¼�� 7-�𶯱��� 8-�������� 9-���ܱ��� 10-�ش�¼��
			u32 dwRecordLength;	//¼���С
			u8  byLockFlag;    // ������־ 0��û���� 1������
			u8  byDrawFrameType;    // 0���ǳ�֡¼�� 1����֡¼��
			u8  byRes1[2];
			u8  byFileName[MAX_PATH]; 	//�ļ���
			u32 dwFileIndex;    		// �浵���ϵ��ļ�����
			u8  byRes[256];
		}struStreamIDRet;
	}uSeniorRet;
}TEZIV_SEARCH_EVENT_RET, *LPTEZIV_SEARCH_EVENT_RET;

//////////////////////////////////////////////////////////////////////////
//Callbacks
//////////////////////////////////////////////////////////////////////////
typedef BOOL32 (*LOGGER)(int nLevel, const char* pszType, const char* pszModule, const char* pszFormat, ...);
typedef void (*EZIV_EVENT_CB)(void* pParam, int nAlarmType, void* lpReserved, void* lpReserved2);
typedef void (*EZIV_ROI_CB)(void* pParam, int nAlarmType, EM_ROI_STATUS nROIStatus, TEZIVRoi* pROIInfo, void* lpReserved, void* lpReserved2);

#endif
