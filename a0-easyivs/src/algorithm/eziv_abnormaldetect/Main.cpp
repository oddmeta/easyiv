/*** �����D�{�� ***/

#include <stdio.h>

#include "AbnormalDetect.h"



int main(int args, char* argv[]) 
{
	/************************************/
	/*		0. �}��ShowImage��flag		*/
	/*		   "1"��ON, "0"��OFF		*/
	/************************************/

	int flag = 1;

	//*********************************************************************************************************//
	//**********   �Ѽƫŧi   *********************************************************************************//
	//*********************************************************************************************************//
	CvCapture* videofile = NULL;
	IplImage* pImage = NULL;      //��l�v����T
	IplImage* pImage1 = NULL;     //resize��v����T (320x240)

	IplImage* im1 = NULL;
	IplImage* im2 = NULL;
	IplImage* im3 = NULL;
	//IplImage* im4 = NULL;

	int loop=0;
	int totalframe,pause,FPS;
	int DetectFlag=0;
	int FPSFlag=0;;
	int sensitivity=1;        //senseitivity (time) �����Ѽ�
	int size=2;              //threshold (size) �����Ѽ�
	//char resize_type[]=CV_INTER_LINEAR;



	//*********************************************************************************************************//
	//**********   �ŧi����   *********************************************************************************//
	//*********************************************************************************************************//
	cvNamedWindow( "Original" , 1 ); 
//	cvResizeWindow("Original",320,240);
//	cvMoveWindow("Original",350,100);

	if(flag==1){
		cvNamedWindow( "BackgroundModel" , 0 ); 
		cvResizeWindow("BackgroundModel",320,240);
		cvMoveWindow("BackgroundModel",700,100);
		
		cvNamedWindow( "ForegroundModel" , 0 );
		cvResizeWindow("ForegroundModel",320,240);
		cvMoveWindow("ForegroundModel",700,450);
	}

	//cvNamedWindow( "maxObjProb" , 0 ); 
	//cvResizeWindow("maxObjProb",320,240);
	//cvMoveWindow("maxObjProb",350,100);

	//*********************************************************************************************************//
	//**********   �T���ӷ�   *********************************************************************************//
	//*********************************************************************************************************//
    //   ����: �T���ӷ��w�]��camera�A����camera��		//
	//         �h�۰ʧ�Ū��video						//
	//         �Ĥ@�q���Ҽ{camera�A�ĤG�q�u�Ҽ{���ռv�� //

	//   �Ĥ@�q
	const char* filename = "libabnormaldetect_sample.avi";
	if (args == 2)
		filename = argv[1];

	videofile=cvCaptureFromFile(filename); //Ū��video
	if(videofile==NULL){
		videofile=cvCaptureFromCAM(0); //�w�]��camera
		if(!videofile){ //�O�_Ū�ɦ��\
			printf( "Input Source Error!!! \n" ); 
			exit(0);
		}
		totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video��frame�`�� 
	}else{
		totalframe = -1; //��camera���T���ӷ��ɡA�]��-1
	}

    //   �ĤG�q
	//videofile=cvCaptureFromFile("20100526_155603.avi"); //Ū��video
	 //videofile=cvCaptureFromFile("testvideo_out01_320x240_FPS10.avi"); //Ū��video   4   bluebag orangebox whitebox  orangebox  FP1(leg)  EE7
	//videofile=cvCaptureFromFile("testvideo_out02_320x240_FPS10.avi"); //Ū��video
	//videofile=cvCaptureFromFile("testvideo_out03_320x240_FPS10.avi"); //Ū��video 
	//videofile=cvCaptureFromFile("testvideo_out04_320x240_FPS10.avi"); //Ū��video 
	//videofile=cvCaptureFromFile("testvideo_out05_320x240_FPS10.avi"); //Ū��video   3   bluebag    EE1   many people
	//videofile=cvCaptureFromFile("testvideo_out06_320x240_FPS10.avi"); //Ū��video

	//videofile=cvCaptureFromFile("AbnormalDetect_TestVideo_320X240_FPS10.AVI"); //Ū��video
	//videofile=cvCaptureFromFile("AbnormalDetect_TestVideo.AVI"); //Ū��video


	//videofile=cvCaptureFromFile("0305_1.AVI"); //Ū��video           3   bluebag yellowbox whitebox(miss)              EE1
	//videofile=cvCaptureFromFile("0305_2.AVI"); //Ū��video           5   bluebag*2 yellowbox*1 whitebox*2 (miss 1)   square nearby library   
	//videofile=cvCaptureFromFile("0305_5.AVI"); //Ū��video           6   bluebag*2 yellowbox*2 whitebox*2 (miss 1)    EE1

	//videofile=cvCaptureFromFile("0307_1.AVI"); //Ū��video           2   bluebag yellowbox  library
	//videofile=cvCaptureFromFile("0307_3.AVI"); //Ū��video           2   bluebag yellowbox  library
	//videofile=cvCaptureFromFile("0307_4.AVI"); //Ū��video           2   bluebag yellowbox  library
	//videofile=cvCaptureFromFile("0307_5.AVI"); //Ū��video           2   bluebag yellowbox  EE7
	//videofile=cvCaptureFromFile("0307_6.AVI"); //Ū��video           1   bluebag yellowbox  EE7

	//videofile=cvCaptureFromFile("0310_1.AVI"); //Ū��video           1   bluebag    EE1
	//videofile=cvCaptureFromFile("0310_3.AVI"); //Ū��video           1   bluebag    EE1
	//videofile=cvCaptureFromFile("0310_4.AVI"); //Ū��video           2   bluebag    EE1

	// ----     framerate=30  -----
	//videofile=cvCaptureFromFile("0518_1.AVI"); //Ū��video           2   blackbag moneybag(miss)    �u�{�@�]�Ǽs��
	//videofile=cvCaptureFromFile("0518_2-1.AVI"); //Ū��video         4   blackbag moneybag*2 umberlla            FP 1
	//videofile=cvCaptureFromFile("0518_3.AVI"); //Ū��video           1   moneybag                    iceshop
	//videofile=cvCaptureFromFile("0518_4.AVI"); //Ū��video           4   safehat*2 blackbag bluebag  ������Ǽs��

	//videofile=cvCaptureFromFile("0520_6.AVI"); //Ū��video           2   two moneybag                lab708
	//videofile=cvCaptureFromFile("0520_7.AVI"); //Ū��video           2   two moneybag                lab708
	//videofile=cvCaptureFromFile("0520_8.AVI"); //Ū��video           4   bread card phone moneybag   my site
	//videofile=cvCaptureFromFile("0520_9.AVI"); //Ū��video           2   moneybag tea                my site
	//videofile=cvCaptureFromFile("0520_10.AVI"); //Ū��video          4   tea bluebox phone moneybag  my site   FP1 (�����Ȥl)


		totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video��frame�`��
		FPS = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FPS  ); //video��frame rate  

		if(!videofile){ //�O�_Ū�ɦ��\
			printf( "Input Source Error!!! \n" ); 
			exit(0);
		}

		cvCreateTrackbar( "Spatial", "Original",&size,10 ,NULL);        //  "Object Size"  "Spatial domain"
        cvCreateTrackbar( "Temporal", "Original",&sensitivity,10 ,NULL);    //  "Sensitivity"  "Temporal domain"
		

	//*********************************************************************************************************//
	//**********   �ŧi�O����P����   *************************************************************************//
	//*********************************************************************************************************//
	pImage = cvQueryFrame( videofile );

	int alarmtime = 5;  //�]�w�C���alarm time���j�p
	//�ŧi����
	CAbnormalDetect *vDetect;
	vDetect=new CAbnormalDetect(320,             //��Jframe��width
								240,             //��Jframe��height
								2,				 //�R������h�֬�~��O�wĵ����
								alarmtime,       //�R������h�֬�~��O���`����
								0);				 //���`����ؿ�h�֬������Ӯؿ�
								          


	//�]�w����j�p
	//vDetect.SetObjectSize( cvSize(30,30) );

	

    //*********************************************************************************************************//
	//**********   �D�n�B�z�j��   *****************************************************************************//
	//*********************************************************************************************************//
	for(;;){

		vDetect->SetObjectSize(cvGetTrackbarPos("Spatial","Original") , cvGetTrackbarPos("Spatial","Original"));  
		vDetect->SetTimeInterval(cvGetTrackbarPos("Temporal","Original")*alarmtime );  
		//   ����R������h�֤~��O���`���󪺬�Ƴ]�w   �PTemporal TrackBar���Ʀr�P�Ѽ�alarmtime����

		pImage=cvQueryFrame(videofile); //���oframe
		//for(int i=0; i<4;i++){
		
		vDetect->SetImage(pImage); //�]�w�v�����	

		if( vDetect->UnderInitializing() ){ //���b��l��(�حI��)

			printf("Initializing...\n");

		}else{ //�w������l�ơA�����i�J�B��
			if(DetectFlag<10){
				printf("Detection Start !!!\n");
				DetectFlag++;
			}
			
			//�P�_�O�_��ĵ�i
			if( vDetect->Alarm() ){

				printf("Warning!!!\n");

			}		
		//}
		}
        
		loop++;

		im1=vDetect->GetImage(1); //Ū����eframe
		im2=vDetect->GetImage(2); //Ū���I��
		im3=vDetect->GetImage(3); //Ū���e��
		//im4=vDetect.GetImage(4); //Ū��optical flow

		//�b������ܼv��
		cvShowImage( "Original" , im1 );
		if(flag==1){
			cvShowImage( "BackgroundModel" , im2 ); 
			cvShowImage( "ForegroundModel" , im3 );	
		}
		//cvShowImage( "maxObjProb" , im4 );	

		//����
		//if( cvWaitKey(1) >= 0 ) 
		//	break;
            if((pause=cvWaitKey(1))>=0)
			{
				if(pause=='p')
				{
					cvWaitKey(0);
				}
				else
					break;
			
			}

		//����video��Ū��
		if(loop==(totalframe-2)) 
			break;
		if(loop==2147483600)
			loop=1;

		

	}



    //*********************************************************************************************************//
	//**********   ����ѼƻP�O����   *************************************************************************//
	//*********************************************************************************************************//
	cvReleaseCapture( &videofile ); 
//	cvDestroyWindow( "Original" );
//	if(flag==1){
//	cvDestroyWindow( "BackgroundModel" );
//	cvDestroyWindow( "ForegroundModel" );
//	}
//	cvReleaseImage(&vDetect.LastFrame);
//	printf("5\n");
//	cvReleaseImage(&vDetect.CurrentImage);
//	printf("6\n");
//	cvReleaseImage(&vDetect.BackgroundImage);
//	printf("7\n");
//	cvReleaseImage(&vDetect.ForegroundImage);
//	printf("8\n");
//	cvReleaseImage(&vDetect.ObjcetProb);
//	printf("9\n");
	delete vDetect;
	return 0;
}

