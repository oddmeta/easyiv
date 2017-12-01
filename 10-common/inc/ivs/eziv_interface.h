#ifndef __RG4_COMMON_INTELLIGENT_VIDEO_INCLUDE
#define __RG4_COMMON_INTELLIGENT_VIDEO_INCLUDE

#pragma warning(disable:4996)

#include "rsmacro.h"
#include "eziv_commondef.h"
#include "eziv_opencvdef.h"

#include <iostream>
using namespace std;

class EASYIV_API CCommonIV
{
public:
	CCommonIV();
	virtual ~CCommonIV();

public:
	virtual BOOL32		Init(u8* pbyParameters, s32 nBytes) = 0;
	virtual BOOL32		UpdateParam(int nEventType, u8* pbyParameters, s32 nBytes) = 0;
	virtual BOOL32		SetROI(int nEventType, CvRect &tRect, int nTime);
	virtual BOOL32		SetExtraParam(u8* pbyParameter, u32 dwBytes);
	virtual	BOOL32		ProcesIntelligents(int nEventType, u8* pbyBuffer, int nWidth, int nHeight, IplImage** pOutImage, BOOL32* bInitialize, TEZIV_DETECT_RESULT& tResult);

	//virtual BOOL32	ParseROIInfo(string strROIInfo);

public:
	static BOOL32		LoadCVModule(int nEventType, CCommonIV** pIntelligent, EZIV_EVENT_CB fpIVEventCB, void* lpIVEventCB);
	BOOL32				CreateBmp(char *filename, u8 *pRGBBuffer, int width, int height, int bpp);
	void				SetLogReport(void* lpParam, LOGGER fpLogReport);
	void				SetOutputImgType(EM_EZIV_IPLIMGTYPE emDetectOutputImgType) {m_emDetectOutputImgType = emDetectOutputImgType;}
	void				SetROIReport(void* lpParam, EZIV_ROI_CB fpROIStatusCB);

public:
	BOOL32				m_bInitParam;

	void*				m_lpParent;
	EM_EZIV_RULE_TYPE	m_emRuleType;
	EZIV_EVENT_CB		m_fpIVEventCB;

	//Log outputs
	LOGGER				m_fpLogReport;
	void*				m_lpLogReport;

	//ROI related definitions
	EZIV_ROI_CB			m_fpROIStatusCB;
	void*				m_lpROIStatusCB;
	TEZIVRoi			m_vROIInfo;
	BOOL32				m_bROISetted;
	s8					m_achROIInfo[8192];

	int					m_nInputCount;
	IplImage*			m_pRGBFrame;
	IplImage*			m_pBGRFrame;
	BOOL32				m_bExit;

	EM_EZIV_IPLIMGTYPE	m_emDetectOutputImgType;
};

#endif // __COMMON_INTELLIGENT_VIDEO_INCLUDE
