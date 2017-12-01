
#include <stdio.h>
#define SNAPTHD 95
#define TH		(0.2)

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

class EASYIV_API CFaceMaskDetect
{
private:
	BOOL32	bAlarmFlag      ;
	int 	nAlarmFlag		;			//警告之flag
	int 	nWidth, nHeight	;			//Frame之宽高
	int 	nAlarmTimer		;			//警告时间
	int 	nBufLen			;
	int		queueHead		;
	int     nCheckThd		;			//For checkpass

	char *XmlName[4]		;
	IplImage *iFrame 		;
	IplImage *iFrameGray  	;

	struct SnapShot
	{
		int 	 SnapWeighting	;
		IplImage *SnapImg 		;	
		double	 SnapTime		;
	} shot[3], tmpShot; 	

	int *weightingBuf	;	
	int QueueCont		;		//For Queue
	int SnapCont		;		//For snap	
	int snapTable[3]	;

	int nCheckNum       ;

	int nGetNum			;


	CvSeq 					*iFeature[4]	;	
	CvMemStorage 			*storage[4] 	;
	CvHaarClassifierCascade *cascade[4] 	;

	CvSeq *FeaturePosition( IplImage* img , int j );
	CvRect	*rt0 , *rt1 , *rt2 ,*rt3;
	CvPoint pt0 , pt1 , pt2 ,pt3 	;

	double WeightDecision( CvPoint pPt0, CvPoint pPt1, CvPoint pPt2, CvPoint pPt3);
	double GaussianProb( int pType , int norType , CvPoint ,  CvPoint , CvPoint targeProb );
	static int	  pTable[4][6]   ;
	double scale  ;
	float  nScale ;

public:
	CFaceMaskDetect( int nImageWidth, int nImageHeight , int buflen = 100, float wscale =1.0) ;
	~CFaceMaskDetect( );

	//---ADD (change)---
	//BOOL32 SetImage(BYTE *pImageArray )	;
	BOOL32 SetImage(IplImage *pImage, IplImage *p_cpImage, float *weight )	;
	//------------------

	IplImage *GetSnap( int nGetNum )	;
	BOOL32 CheckPass(int nCheckThd , int nCheckNum)		;
	void ReleaseSnap( )					;
};

