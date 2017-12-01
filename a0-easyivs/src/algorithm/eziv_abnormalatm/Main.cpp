/*** ��Ϊ����ʽ ***/

#include <stdio.h>
#include "ivs/algorithm/eziv_abnormaldetectatm.h"

#ifdef WIN32
#pragma comment(lib, "eziv_abnormalatm.lib")
#pragma comment(lib, "ezframework.lib")
#endif

int main() 
{	
	/************************************/
	/*		0. ����ShowImage��flag		*/
	/*		   "1"ΪON, "0"ΪOFF		*/
	/************************************/

	int flag = 1;

	//*********************************************************************************************************//
	//**********   ��������   *********************************************************************************//
	//*********************************************************************************************************//
	CvCapture* videofile = NULL;
	IplImage* pImage = NULL;      //ԭʼӰƬ��Ѷ
	IplImage* pImage1 = NULL;     //resize��ӰƬ��Ѷ (320x240)

	IplImage* im1 = NULL;
	IplImage* im2 = NULL;
	IplImage* im3 = NULL;
	//IplImage* im4 = NULL;

	int loop=0;
	int totalframe,pause,FPS;
	int DetectFlag=0;
	int FPSFlag=0;;
	int sensitivity=1;        //senseitivity (time) ��ز���
	int size=1;              //threshold (size) ��ز���
	//char resize_type[]=CV_INTER_LINEAR;

	videofile=cvCaptureFromFile("libabnormaldetect_sample.avi"); //��ȡvideo
	if(videofile==NULL){
		videofile=cvCaptureFromCAM(0); //Ԥ��Ϊcamera
		if(!videofile){ //�Ƿ�����ɹ�
			printf( "Input Source Error!!! \n" ); 
			exit(0);
		}
		totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video��frame���� 
	}else{
		totalframe = -1; //��cameraΪѶ����Դʱ����Ϊ-1
	}

	totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video��frame����
	FPS = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FPS  ); //video��frame rate  

	if(!videofile){ //�Ƿ�����ɹ�
		printf( "Input Source Error!!! \n" ); 
		exit(0);
	}

	cvCreateTrackbar( "Spatial", "Original",&size,10 ,NULL);        //  "Object Size"  "Spatial domain"
	cvCreateTrackbar( "Temporal", "Original",&sensitivity,10 ,NULL);    //  "Sensitivity"  "Temporal domain"


	//*********************************************************************************************************//
	//**********   �����Ӵ�   *********************************************************************************//
	//*********************************************************************************************************//
	cvNamedWindow( "Original" , 0 ); 
	cvResizeWindow("Original",320,240);
	cvMoveWindow("Original",350,100);

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
	//**********   Ѷ����Դ   *********************************************************************************//
	//*********************************************************************************************************//
    //   ˵��: Ѷ����ԴԤ��Ϊcamera����δ��cameraʱ		//
	//         ���Զ��Ķ�ȡvideo						//
	//         ��һ���п���camera���ڶ���ֻ���ǲ���ӰƬ //

	//   ��һ��
	/*
    */

    //   �ڶ���
	 //videofile=cvCaptureFromFile("20100526_155603-1.avi"); //��ȡvideo   30
	 //videofile=cvCaptureFromFile("ATM_add_704x576.AVI"); //��ȡvideo
	 //videofile=cvCaptureFromFile("ATM_add_1_704x576.AVI"); //��ȡvideo
	 //videofile=cvCaptureFromFile("abnormal.avi"); //��ȡvideo
     //videofile=cvCaptureFromFile("abnormal-1.avi"); //��ȡvideo
	//videofile=cvCaptureFromFile("AbnormalDetect_TestVideo.AVI"); //��ȡvideo
	//videofile=cvCaptureFromFile("ATM_add_1_704x576.AVI"); //��ȡvideo   30


		

	//*********************************************************************************************************//
	//**********   ��������������   *************************************************************************//
	//*********************************************************************************************************//
	pImage = cvQueryFrame( videofile );

	int alarmtime = 60;  //Ǥ�緪�ȏmalarm time��С
	//�������
	CAbnormalATM *vDetect;
	vDetect=new CAbnormalATM();			     //�쳣�����ѡ��������ȡ���ÿ�ѡ

    TEZIVINFO_ABNORMAL tConfig;
    tConfig.m_nFrameWidth = pImage->width;
    tConfig.m_nFrameHeight = pImage->height;
    tConfig.nObjectNumber = 5;
    tConfig.nSeconds1 = 5;
    tConfig.nSeconds2 = alarmtime;
    tConfig.nSeconds3 = alarmtime*2;

    vDetect->Init(tConfig);

	//�趨�����С
	//vDetect.SetObjectSize( cvSize(30,30) );

    //*********************************************************************************************************//
	//**********   ��Ҫ�����Ȧ   *****************************************************************************//
	//*********************************************************************************************************//
	for(;;)
    {
		vDetect->SetObjectSize(cvGetTrackbarPos("Spatial","Original") , cvGetTrackbarPos("Spatial","Original"));
		vDetect->SetTimeInterval(cvGetTrackbarPos("Temporal","Original")*alarmtime ); 
		//   ʵ�ʾ�ֹ����������ٲ������쳣����������趨   ��Temporal TrackBar�����������alarmtime�й�

		pImage=cvQueryFrame(videofile); //ȡ��frame
		//for(int i=0; i<4;i++){
		
		vDetect->InputImage(pImage); //�趨Ӱ������	

		if( vDetect->UnderInitializing() )
        {
            //���ڳ�ʼ��(������)
			printf("Initializing...\n");
		}
        else
        {
            //����ɳ�ʼ������ʽ��������
			DetectFlag++;
			//�ж��Ƿ��о���
            TEZIV_DETECT_RESULT tResult;
			if( vDetect->DetectAndDraw(tResult))
            {
				printf("Warning!!! %d\n", DetectFlag);
			}
			else
            {
				printf("Detecting!!! %d\n", DetectFlag);
			}
		//}
		}
        
		loop++;

        /*
        EM_EZIV_IPLIMG_SHOW = 1,
        EM_EZIV_IPLIMG_BACKGROUD,
        EM_EZIV_IPLIMG_FOREGROUD,
        EM_EZIV_IPLIMG_OPTICALFLOW,
        */

		im1=vDetect->GetImage(EM_EZIV_IPLIMG_SHOW); //��ȡ��ǰframe
		im2=vDetect->GetImage(EM_EZIV_IPLIMG_BACKGROUD); //��ȡ����
		im3=vDetect->GetImage(EM_EZIV_IPLIMG_FOREGROUD); //��ȡǰ��
		//im4=vDetect.GetImage(4); //��ȡoptical flow

		//���Ӵ���ʾӰ��
		cvShowImage( "Original" , im1 );
		if (flag==1)
        {
			cvShowImage( "BackgroundModel" , im2 ); 
			cvShowImage( "ForegroundModel" , im3 );	
		}
		//cvShowImage( "maxObjProb" , im4 );	

		//�ӳ�
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

		//����video�Ķ�ȡ
		if(loop==(totalframe-2)) 
			break;
		if(loop==2147483600)
			loop=1;
	}

    //*********************************************************************************************************//
	//**********   �ͷŲ����������   *************************************************************************//
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
