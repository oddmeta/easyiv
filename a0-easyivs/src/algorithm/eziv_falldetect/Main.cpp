#include "FallDetect.h"

///////Add by Tsao////////////////////////////////////////
int test1, test2, test3;
int C_motionthr,Center_Positionthr;
double Thetathr;
void onTrack1(int h)
{
	C_motionthr = test1;
}
void onTrack2(int h)
{
	Thetathr = test2;
}
void onTrack3(int h)
{
	Center_Positionthr = test3;
}
///////Add by Tsao////////////////////////////////////////

void main(int args, char* argv[])
{
	int control;
	int count = 0;
	   
	//load test video--------------------------------------
	const char* input_name = "libfalldetect_sample.avi";
	if (args == 2)
	    input_name = argv[1];

	CvCapture *capture = 0;
    capture = cvCreateFileCapture( input_name );
	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
	}
	//-----------------------------------------------------

	//define the windows to show result--------------------
    cvNamedWindow( "Video", 1 );
	cvNamedWindow( "Foreground", 1 );
	cvNamedWindow( "Background", 1 );
	//-----------------------------------------------------
    
	if( capture ){
		IplImage *frame = 0;
        frame = cvQueryFrame(capture);
        
		//initial-------------------------------------------------------------------
		CFallDetect Run_Test(frame->width,frame->height,3,1100);
        //--------------------------------------------------------------------------
        int value = 7;
		int fr_ra_test = value;
		int fr_ra_count = 0;
		int frameratecount = 0;
		time_t systemTime1; //time_t �O ctime �̩w�q���@�ث��A (long int)
        time_t systemTime2;

		///////Add by Tsao////////////////////////////////////////
		test1 = Run_Test.GetC_motionthr();
		test2 = Run_Test.GetThetathr();
		test3 = Run_Test.GetCt_Posthr();

		///////Add by Tsao////////////////////////////////////////


        for(;;){

			if(frameratecount == 0) time( &systemTime1);
            if( !cvGrabFrame( capture )) break;
            frame = cvRetrieveFrame( capture );//load the video frame
			if( !frame )                 break;
			if( frame->origin == IPL_ORIGIN_TL)	cvCopy(frame, Run_Test.frame_copy , 0);
			else                                cvFlip(frame, Run_Test.frame_copy , 0);
		    
			fr_ra_count +=1;

		  if(fr_ra_count == fr_ra_test){			
			//updata the background and compute the foreground---------------------------------------- 
			
			  bool IsInitializing = Run_Test.Ini_and_Det();
			  if(IsInitializing)
			  {
				  printf("Initializing...\n");
			  }
			  else
			  {
				  printf("Detecting...\n");
			  }
			//----------------------------------------------------------------------------------------

		    fr_ra_test = (fr_ra_test == 1) ? value:fr_ra_test-1;
		    fr_ra_count=0;
			cvShowImage( "Video", frame );		
			cvShowImage( "Foreground", Run_Test.Foreground );
	        cvShowImage( "Background", Run_Test.Background );




			///////Add by Tsao //////////////////////////////////////////
			//�N�T�ӰѼƩԥX�Ӱ���bar���վ�
			//C_motionthr�G�ʶq�j�p�����e�ȡA�j�󦹥N���i��|���^�˨ƥ�o��
	        //Thetathr�G���P������V�������ܤƶq���e�ȡA�j�󦹥N���i��|���^�˨ƥ�o��
	        //Center_Positionthr�G��ꤤ���I�첾�q�����e�ȡA�j�󦹥N���i��|���^�˨ƥ�o��
						
			cvCreateTrackbar( "C_motion", "Video", &test1, 100, onTrack1);
			onTrack1(0);
			Run_Test.SetC_motionthr(C_motionthr);
			
			cvCreateTrackbar( "Theta", "Video", &test2, 30, onTrack2);
			onTrack2(0);
			Run_Test.SetThetathr(Thetathr);

			cvCreateTrackbar( "Ct_Pos", "Video", &test3, 100, onTrack3);
			onTrack3(0);
			Run_Test.SetCt_Posthr(Center_Positionthr);

			///////Add by Tsao///////////////////////////////////////////////////

			if(Run_Test.Alarm()){
				printf("Fall detect !!\n");
			} 
		  }

			control = cvWaitKey(50);
			if( control == 'p' ){for(;;){if( cvWaitKey( 0 ) >= 0 ) break;}}
			else if( control >= 0 )break;

            
			count++;
			frameratecount++;
			if(frameratecount==15){
				for(;;){
					time( &systemTime2);
					if(systemTime2-systemTime1>0){
						frameratecount=0;
						break;	
					}
				}
			}
        }

		
		cvReleaseCapture( &capture );

    }

        
    cvDestroyWindow( "Videio");  //��l�Ϲ�
	cvDestroyWindow( "Foreground");
	cvDestroyWindow( "Background");

}