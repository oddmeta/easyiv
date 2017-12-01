
#include <conio.h>
#include <stdio.h>

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

typedef u8 u8 ;

class EASYIV_API CLostDetect
{
public:
	CLostDetect();
	~CLostDetect();

public:
	BOOL32		Init(TEZIVINFO_LOSTDETECT &tParam);								//Initialize configurations
    void		InputImage(IplImage *);
	BOOL32		DetectAndDraw(TEZIV_DETECT_RESULT& tResult);								//Find faces and draw them in the image
	BOOL32		Alarm();														//Trigger when detected 
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);					//Get image array
	BOOL32		SetROI(IplImage *pImage, int x, int y, int width, int height);

private:
	BOOL32		Reset();
	int			GetObjectState();

private:
	TEZIVINFO_LOSTDETECT	m_tConfig;

	int						m_nAlarmFlag;
	BOOL32					m_bROIObjSetted;

	BOOL32					m_bEgdeDen;
	float					*m_pfHistRanges;		//A temporary pointer points to m_tConfig.m_afHistRangesArr

	CvHistogram *m_pHistogramR, *m_pHistogramG, *m_pHistogramB;
	//Jacky: 20090813 upgrade
	CvHistogram *m_pHistogramCurR, *m_pHistogramCurG, *m_pHistogramCurB;

	IplImage *m_pImgGray, *m_pImgObjectModel, *m_pImgObjectModel1, *m_pImgObjectModelMinus1, *m_pImgEdge;
	IplImage *m_ptFrameCopy;
};