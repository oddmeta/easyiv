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
	EM_EZIV_IMAGE_ENHANCE_GAMA,				//1 gama 校正  
	EM_EZIV_IMAGE_ENHANCE_HISTOGRAM,		//2 histogram equaliration 
	EM_EZIV_IMAGE_ENHANCE_GAMA_AND_HIST,	//3 gama 校正+ histogram equaliration
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
	//有人离开
	ObjectNum=Search.ReturnObjectNum();
	cout<<"out object num:"<<ObjectNum<<'\n';
	for(i=0;i<ObjectNum;i++)
	{
		//输出资料
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
	double			scale;							//是否缩小画面进去找（scale=1.3）
	TEZIVArea		tMinArea;						//启动人脸侦测的前景区块大小, 启动侦测的前景区块最小面积为400（20*20为最小能侦测到的人脸大小）
	int				m_nBackgroundSamples;			//Jacky, 初始化背景所需帧数 default value 20 

	int				m_nObject;				// for face detect

	//光线调整参数start
	EM_EZIV_IMAGE_ENHANCE	m_emEnhanceMode;
	TEZIVHistEqualize	m_tHistEqualize;
	TEZIVGamaTrans		m_tGamaTrans;
	//double			parameter1;
	//double			parameter2;
	double			m_dSearchPecent;
	//光线调整参数end

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
	double		scale;							//是否缩小画面进去找（scale=1.3）
	double		face_area;						//启动人脸侦测的前景区块大小, 启动侦测的前景区块最小面积为400（20*20为最小能侦测到的人脸大小）
	int			m_nBackgroundSamples;			//Jacky, 初始化背景所需帧数 default value 20 

	//光线调整参数start
	EM_EZIV_IMAGE_ENHANCE	Mode;
	double		parameter1;
	double		parameter2;
	double		search_range;
	//光线调整参数end

	//Debug output
	//EZIVDbgVideo m_tDbgVideo;
}TEZIVINFO_FACEFEATURE;

typedef struct tagEZIVINFO_HUMANCOUNT
{
	int			m_nFrameWidth;
	int			m_nFrameHeight;
	s8			m_achConfPath[256];

	TEZIVRoi	m_tROI;
	double		scale;							//是否缩小画面进去找（scale=1.3）
	double		face_area;						//启动人脸侦测的前景区块大小, 启动侦测的前景区块最小面积为400（20*20为最小能侦测到的人脸大小）
	int			m_nBackgroundSamples;			//Jacky, 初始化背景所需帧数 default value 20 

	//光线调整参数start
	EM_EZIV_IMAGE_ENHANCE	Mode;
	double		parameter1;
	double		parameter2;
	double		search_range;
	//光线调整参数end

	//Debug output
	//EZIVDbgVideo m_tDbgVideo;
}TEZIVINFO_HUMANCOUNT;

enum EM_EZIV_INTELLISEARCH_MODE
{
	EM_EZIV_INTELLISEARCH_MODE_DEFAULT = 0,
	EM_EZIV_INTELLISEARCH_MODE_FACETRACKONLY,		//只搜寻人脸有出现动线
};

typedef struct tagEZIVINFO_INTELLISEARCH
{
	int					m_nFrameWidth;
	int					m_nFrameHeight;
	s8					m_achConfPath[MAX_PATH];
	int					m_nMaxObjectInFrame;				//最多能在画面中被找到的物件数目
	BOOL32				m_bFaceOnly;						//是否只回传人脸出现事件
	EM_EZIV_INTELLISEARCH_MODE m_emSearchMode;					//切换操作模式

	//ROI
	CvPoint				m_tROI_ltpt;
	CvPoint				m_tROI_rbpt;						//ROI左上点与右下点
	CvRect				m_tROIRect;							//设定ROI大小

	//光线调整参数start
	EM_EZIV_IMAGE_ENHANCE	m_emEnhanceMode;
	TEZIVHistEqualize	m_tHistEqualize;
	TEZIVGamaTrans		m_tGamaTrans;
	double				m_dSearchPecent;
	//光线调整参数end
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

		//界线的座标
		s32 x1;
		s32 y1;
		s32 x2;
		s32 y2;

		//y=nA*x+nB
		double nA;
		double nB;
	} m_tROADInfo;

	double		scale;							//是否缩小画面进去找（scale=1.3）
	double		face_area;						//启动人脸侦测的前景区块大小, 启动侦测的前景区块最小面积为400（20*20为最小能侦测到的人脸大小）
	int			m_nBackgroundSamples;			//Jacky, 初始化背景所需帧数 default value 20
}TEZIVINFO_INTRUDER;

typedef struct tagEZIVINFO_ABNORMAL
{
	s32			m_nFrameWidth;
	s32			m_nFrameHeight;
	s8			m_achConfPath[256];

	BOOL32		m_bResizeImg;
	int			nSeconds1;					//静止物体持续多少秒才算是预警物体
	int			nSeconds2;					//静止物体持续多少秒才算是异常物体
	int			nSeconds3;					//异常物体框选多少秒后取消该框选
	int			FrameIntervalCount;			//间隔多少张frame执行一次Optical flow
	int			FrameIntervalCountValue;	//根据多少张frame执行一次Optical flow的threshold
	int			WindowSize;					//计算光流值时，撷取的区块大小
	int			BoundarySpace;				//设定边界范围
	int			Train_BgModel_number;		//计算建背景所需的frame张数
	double		weight;						//计算初始时背景模型的权重值
	s32			nObjectNumber;				//可允许的物体数量
	CvSize		nScale;						//只侦测比这个值大的物体

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
//事件搜索条件
typedef struct tagNET_DVR_SEARCH_EVENT_PARAM
{
	u16 wMajorType;					//0-移动侦测，1-报警输入, 2-智能事件
	u16 wMinorType;					//搜索次类型- 根据主类型变化，0xffff表示全部
	SYSTEMTIME_ struStartTime;		//搜索的开始时间，停止时间: 同时为(0, 0) 表示从最早的时间开始，到最后，最前面的4000个事件
	SYSTEMTIME_ struEndTime;		//
	u8			byLockType;			// 0xff-全部，0-未锁，1-锁定
	u8 			byValue;			//0-按位表示，1-按值表示
	u8 			byRes[130];			// 保留
	union	
	{
		u8 byLen[256];				//SEARCH_EVENT_INFO_LEN
		struct//报警输入
		{
			u8 byAlarmInNo[32];		//MAX_ALARMIN_V30 报警输入号，byAlarmInNo[0]若置1则表示查找由报警输入1触发的事件
			u8 byRes[64-32];		//SEARCH_EVENT_INFO_LEN - MAX_ALARMIN_V30
		}struAlarmParam;

		struct//报警输入--按值
		{
			u16 wAlarmInNo[128];	// 报警输入号，按值表示,采用紧凑型排列
			u8 byRes[44];			// Reserved 
		}struAlarmParamByValue;

		struct//移动侦测
		{
			u8 byMotDetChanNo[32];	//MAX_CHANNUM_V30 移动侦测通道，byMotDetChanNo[0]若置1则表示查找由通道1发生移动侦测触发的事件
			u8 byRes[64 - 32];		//SEARCH_EVENT_INFO_LEN - MAX_CHANNUM_V30
		}struMotionParam;

		struct//移动侦测--按值
		{
			u16 wMotDetChanNo[64];	// 移动侦测通道，按值表示 ,采用紧凑型排列
			u8 byRes[172];			// Reserved
		}struMotionParamByValue;

		struct//行为分析
		{
			u8 byChanNo[32];		//MAX_CHANNUM_V30 触发事件的通道
			u8 byRuleID;			//规则ID，0xff表示全部
			u8 byRes1[43];			//保留
		}struVcaParam;

		struct //行为分析--按值方式查找 
		{
			u16 wChanNo[64];		//行为分析对应的通道，按值表示,采用紧凑型排列
			u8 byRuleID;			//行为分析类型，规则0xff表示全部，从0开始
			u8 byRes[171];			//保留
		}struVcaParamByValue;

		struct//审讯事件搜索条件
		{
			u8 byRoomIndex;    //审讯室编号,从1开始
			u8 byRes[299];     //保留
		} struInquestParam;
		struct  //智能侦测查找条件
		{
			u8 byChan [256];//触发智能侦测的通道号，按数组下标表示，byChan[0]若置1则表示查找由通道1发生移动侦测触发的事件 
		}struVCADetectByBit;

		struct//智能侦测查找条件 ，通道号按值表示
		{
			u32 dwChanNo[32-1];//MAX_CHANNUM_V30 触发通道号,按值表示，0xffffffff无效，且后续数据也表示无效值
			u8 byAll;//0-表示不是全部，1-表示全部。
			u8 byres[3];
		}struVCADetectByValue;
		/*
		struct
		{
			NET_DVR_STREAM_INFO struIDInfo; // 流id信息，72字节长
			u32				dwCmdType;  // 外部触发类型，NVR接入云存储使用
			u8				byBackupVolumeNum; //存档卷号，CVR使用
			u8				byRes[223];
		}struStreamIDParam;
		*/
	}uSeniorParam;
}NET_DVR_SEARCH_EVENT_PARAM, *LPNET_DVR_SEARCH_EVENT_PARAM;

//查找返回结果
typedef struct tagEZIV_SEARCH_EVENT_RET
{
	u16 wMajorType;					//主类型
	u16 wMinorType;					//次类型
	SYSTEMTIME_	struStartTime;		//事件开始的时间
	SYSTEMTIME_ struEndTime;		//事件停止的时间，脉冲事件时和开始时间一样
	u8 byChan[32];					//MAX_CHANNUM_V30
	u8 byRes[36];
	union
	{
		struct//报警输入结果
		{
			u32 dwAlarmInNo;		//报警输入号
			u8 byRes[32];			//SEARCH_EVENT_INFO_LEN
		}struAlarmRet;
		struct//移动侦测结果
		{
			u32 dwMotDetNo;			//移动侦测通道
			u8 byRes[32];			//SEARCH_EVENT_INFO_LEN
		}struMotionRet;
		struct//行为分析结果  
		{
			u32 dwChanNo;					//触发事件的通道号
			u8 byRuleID;					//规则ID
			u8 byRes1[3];					//保留
			u8 byRuleName[MAX_PATH];		//规则名称
			EM_EZIV_RULE_TYPE uEvent;				//NET_VCA_EVENT_UNION uEvent;     //行为事件参数
		}struVcaRet;
		struct//审讯事件
		{
			u8  byRoomIndex;     //审讯室编号,从1开始
			u8  byDriveIndex;    //刻录机编号,从1开始
			u8  byRes1[6];       //保留
			u32 dwSegmentNo;     //本片断在本次审讯中的序号,从1开始 
			u16  wSegmetSize;     //本片断的大小, 单位M 
			u16  wSegmentState;   //本片断状态 0 刻录正常，1 刻录异常，2 不刻录审讯
			u8  byRes2[288];     //保留
		}struInquestRet;
		struct //流id录像查询结果
		{
			u32 dwRecordType;	//录像类型 0-定时录像 1-移动侦测 2-报警录像 3-报警|移动侦测 4-报警&移动侦测 5-命令触发 6-手动录像 7-震动报警 8-环境触发 9-智能报警 10-回传录像
			u32 dwRecordLength;	//录像大小
			u8  byLockFlag;    // 锁定标志 0：没锁定 1：锁定
			u8  byDrawFrameType;    // 0：非抽帧录像 1：抽帧录像
			u8  byRes1[2];
			u8  byFileName[MAX_PATH]; 	//文件名
			u32 dwFileIndex;    		// 存档卷上的文件索引
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
