#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

class EASYIV_API CMaliciousDetect
{
private:
	BOOL32	bAlarmFlag;
	int 	nAlarmTimer;
	int		nWidth, nHeight;

	BOOL32 IsSetted;

    IplImage *Frame;		//input image
    IplImage *Gray;			//color to gray
    IplImage *C_edge; 		//computation the edge value
    IplImage *BG_edge;
    IplImage *_small; 		//resize C_edge to _small
    IplImage *BG_small;
    
    CvHistogram *BG_hist,*C_hist;
    int hist_size ;
    CvScalar  num1 , num2 ;	//computation BG_small & _small	
  	
  	int WCount; 
  	int OCount;
  	int ECount;
  	int HCount;
  	int H_HT  ;
  	int H_LT  ;
    
    int line_type;
    double	HistDiff;


public:
	CMaliciousDetect();
	CMaliciousDetect(int nImageWidth, int nImageHeight, int nSeconds);
	~CMaliciousDetect();


	BOOL32 SetImage(u8 *pImageArray);
	BOOL32 SetImage(IplImage *pImage);
	BOOL32 Alarm() {return bAlarmFlag;};
	void ResetSystem() {IsSetted = false;};


};

