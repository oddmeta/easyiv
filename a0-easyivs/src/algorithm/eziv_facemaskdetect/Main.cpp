#include <stdio.h>
#include "FaceMaskDetect.h"
//#pragma comment(lib, "FaceMaskDetect.lib") 
/*   
 *		usage : the program will work after video opening.  
 *				when you press the "space buttom", the program will select the 3 highest score image and show them.
 *				( these images just keep in memory )
 *				And the memory of the 3 highest images will be released when you press the "c buttom. 
 *		Note  : the folder "XML" has to be in the same folder of the execute file.
 *		
 *
 * 
 */
int main()
{
	CvCapture* capture = 0 ;													//³]©wcamera
	IplImage *Frame=NULL   ;													//image
	IplImage *Simg0=NULL   ; 
	IplImage *Simg1=NULL   ; 
	IplImage *Simg2=NULL   ;  
	//---ADD---
	IplImage *cpFrame=NULL;
	//---------
		 
	cvNamedWindow( "result", 1 );
	cvNamedWindow( "T-result", 1 );
	cvNamedWindow( "Snap-0", 1 );
	cvNamedWindow( "Snap-1", 1 );
	cvNamedWindow( "Snap-2", 1 );
	cvNamedWindow( "Feature Map", 1 );
	bool flagCheck;

	//webcam input video
  	capture = cvCaptureFromCAM(0);
 	if( capture == NULL ){
  		capture = cvCaptureFromFile("4.FaceMaskDetect.avi");
  	}
	while (Frame == NULL){
		Frame = cvQueryFrame( capture );
		cpFrame = cvCreateImage( cvSize(Frame->width,Frame->height), 8, 3 );
	}
	
	CFaceMaskDetect Detect( Frame->width,Frame->height,100, 1.5 );
    
	//---ADD---
	float weight = 0.0;
	//---------
	int key;
	int stat = 0;
	for( ;Frame; Frame = cvQueryFrame( capture ) )
	{
		//---ADD(and Change)---
		cvCopy( Frame, cpFrame, 0 );

		Detect.SetImage( Frame, cpFrame, &weight );
		printf("MAX : %5.2f\n", weight);
		cvShowImage("Feature Map", cpFrame);
		//---------------------

		key = cvWaitKey( 2000 );
		if(  key == 32 )
		{
			flagCheck = Detect.CheckPass( 60, 5 );
							
			Simg0 = Detect.GetSnap(0);
 			cvShowImage("Snap-0", Simg0);
			Simg1 = Detect.GetSnap(1);
			cvShowImage("Snap-1", Simg1);
			Simg2 = Detect.GetSnap(2);
			cvShowImage("Snap-2", Simg2);
			if (flagCheck == false){
				printf("!!! -- WARNING -- !!!\n");
			}
		}	  
		if ( key == 27 ){
			break; 
		}
		if( key != 32 && key > 0 ){
			printf("XXXXXXXXx\n");
			Detect.ReleaseSnap();
			Simg0 = Detect.GetSnap(0);
			cvShowImage("Snap-0", Simg0);
		}
		
		cvShowImage( "result", Frame );

	}
    
    cvDestroyAllWindows();//¾P·´µøµ¡
    cvReleaseCapture( &capture);
	cvReleaseImage( &Simg0 );
	cvReleaseImage( &Simg1 );
	cvReleaseImage( &Simg2 );
	cvReleaseImage( &cpFrame );
	return 0;
}