/*** 此为主程式 ***/

#include <stdio.h>
#include "ivs/algorithm/eziv_abnormaldetectatm.h"

#ifdef WIN32
#pragma comment(lib, "eziv_abnormalatm.lib")
#pragma comment(lib, "ezframework.lib")
#endif

int main() 
{	
	/************************************/
	/*		0. 开关ShowImage用flag		*/
	/*		   "1"为ON, "0"为OFF		*/
	/************************************/

	int flag = 1;

	//*********************************************************************************************************//
	//**********   参数宣告   *********************************************************************************//
	//*********************************************************************************************************//
	CvCapture* videofile = NULL;
	IplImage* pImage = NULL;      //原始影片资讯
	IplImage* pImage1 = NULL;     //resize後影片资讯 (320x240)

	IplImage* im1 = NULL;
	IplImage* im2 = NULL;
	IplImage* im3 = NULL;
	//IplImage* im4 = NULL;

	int loop=0;
	int totalframe,pause,FPS;
	int DetectFlag=0;
	int FPSFlag=0;;
	int sensitivity=1;        //senseitivity (time) 相关参数
	int size=1;              //threshold (size) 相关参数
	//char resize_type[]=CV_INTER_LINEAR;

	videofile=cvCaptureFromFile("libabnormaldetect_sample.avi"); //读取video
	if(videofile==NULL){
		videofile=cvCaptureFromCAM(0); //预设为camera
		if(!videofile){ //是否读档成功
			printf( "Input Source Error!!! \n" ); 
			exit(0);
		}
		totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video的frame总数 
	}else{
		totalframe = -1; //当camera为讯号来源时，设为-1
	}

	totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video的frame总数
	FPS = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FPS  ); //video的frame rate  

	if(!videofile){ //是否读档成功
		printf( "Input Source Error!!! \n" ); 
		exit(0);
	}

	cvCreateTrackbar( "Spatial", "Original",&size,10 ,NULL);        //  "Object Size"  "Spatial domain"
	cvCreateTrackbar( "Temporal", "Original",&sensitivity,10 ,NULL);    //  "Sensitivity"  "Temporal domain"


	//*********************************************************************************************************//
	//**********   宣告视窗   *********************************************************************************//
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
	//**********   讯号来源   *********************************************************************************//
	//*********************************************************************************************************//
    //   说明: 讯号来源预设为camera，当未接camera时		//
	//         则自动改读取video						//
	//         第一段有考虑camera，第二段只考虑测试影片 //

	//   第一段
	/*
    */

    //   第二段
	 //videofile=cvCaptureFromFile("20100526_155603-1.avi"); //读取video   30
	 //videofile=cvCaptureFromFile("ATM_add_704x576.AVI"); //读取video
	 //videofile=cvCaptureFromFile("ATM_add_1_704x576.AVI"); //读取video
	 //videofile=cvCaptureFromFile("abnormal.avi"); //读取video
     //videofile=cvCaptureFromFile("abnormal-1.avi"); //读取video
	//videofile=cvCaptureFromFile("AbnormalDetect_TestVideo.AVI"); //读取video
	//videofile=cvCaptureFromFile("ATM_add_1_704x576.AVI"); //读取video   30


		

	//*********************************************************************************************************//
	//**********   宣告记忆体与物件   *************************************************************************//
	//*********************************************************************************************************//
	pImage = cvQueryFrame( videofile );

	int alarmtime = 60;  //扦隅藩等弇alarm time大小
	//宣告物件
	CAbnormalATM *vDetect;
	vDetect=new CAbnormalATM();			     //异常物件框选多少秒後取消该框选

    TEZIVINFO_ABNORMAL tConfig;
    tConfig.m_nFrameWidth = pImage->width;
    tConfig.m_nFrameHeight = pImage->height;
    tConfig.nObjectNumber = 5;
    tConfig.nSeconds1 = 5;
    tConfig.nSeconds2 = alarmtime;
    tConfig.nSeconds3 = alarmtime*2;

    vDetect->Init(tConfig);

	//设定物件大小
	//vDetect.SetObjectSize( cvSize(30,30) );

    //*********************************************************************************************************//
	//**********   主要处理回圈   *****************************************************************************//
	//*********************************************************************************************************//
	for(;;)
    {
		vDetect->SetObjectSize(cvGetTrackbarPos("Spatial","Original") , cvGetTrackbarPos("Spatial","Original"));
		vDetect->SetTimeInterval(cvGetTrackbarPos("Temporal","Original")*alarmtime ); 
		//   实际静止物体持续多少才算是异常物件的秒数设定   与Temporal TrackBar的数字与参数alarmtime有关

		pImage=cvQueryFrame(videofile); //取得frame
		//for(int i=0; i<4;i++){
		
		vDetect->InputImage(pImage); //设定影像资料	

		if( vDetect->UnderInitializing() )
        {
            //正在初始化(建背景)
			printf("Initializing...\n");
		}
        else
        {
            //已完成初始化，正式进入运算
			DetectFlag++;
			//判断是否有警告
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

		im1=vDetect->GetImage(EM_EZIV_IPLIMG_SHOW); //读取当前frame
		im2=vDetect->GetImage(EM_EZIV_IPLIMG_BACKGROUD); //读取背景
		im3=vDetect->GetImage(EM_EZIV_IPLIMG_FOREGROUD); //读取前景
		//im4=vDetect.GetImage(4); //读取optical flow

		//在视窗显示影像
		cvShowImage( "Original" , im1 );
		if (flag==1)
        {
			cvShowImage( "BackgroundModel" , im2 ); 
			cvShowImage( "ForegroundModel" , im3 );	
		}
		//cvShowImage( "maxObjProb" , im4 );	

		//延迟
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

		//结束video的读取
		if(loop==(totalframe-2)) 
			break;
		if(loop==2147483600)
			loop=1;
	}

    //*********************************************************************************************************//
	//**********   释放参数与记忆体   *************************************************************************//
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
