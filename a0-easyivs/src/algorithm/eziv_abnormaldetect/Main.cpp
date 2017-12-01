/*** 此為主程式 ***/

#include <stdio.h>

#include "AbnormalDetect.h"



int main(int args, char* argv[]) 
{
	/************************************/
	/*		0. 開關ShowImage用flag		*/
	/*		   "1"為ON, "0"為OFF		*/
	/************************************/

	int flag = 1;

	//*********************************************************************************************************//
	//**********   參數宣告   *********************************************************************************//
	//*********************************************************************************************************//
	CvCapture* videofile = NULL;
	IplImage* pImage = NULL;      //原始影片資訊
	IplImage* pImage1 = NULL;     //resize後影片資訊 (320x240)

	IplImage* im1 = NULL;
	IplImage* im2 = NULL;
	IplImage* im3 = NULL;
	//IplImage* im4 = NULL;

	int loop=0;
	int totalframe,pause,FPS;
	int DetectFlag=0;
	int FPSFlag=0;;
	int sensitivity=1;        //senseitivity (time) 相關參數
	int size=2;              //threshold (size) 相關參數
	//char resize_type[]=CV_INTER_LINEAR;



	//*********************************************************************************************************//
	//**********   宣告視窗   *********************************************************************************//
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
	//**********   訊號來源   *********************************************************************************//
	//*********************************************************************************************************//
    //   說明: 訊號來源預設為camera，當未接camera時		//
	//         則自動改讀取video						//
	//         第一段有考慮camera，第二段只考慮測試影片 //

	//   第一段
	const char* filename = "libabnormaldetect_sample.avi";
	if (args == 2)
		filename = argv[1];

	videofile=cvCaptureFromFile(filename); //讀取video
	if(videofile==NULL){
		videofile=cvCaptureFromCAM(0); //預設為camera
		if(!videofile){ //是否讀檔成功
			printf( "Input Source Error!!! \n" ); 
			exit(0);
		}
		totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video的frame總數 
	}else{
		totalframe = -1; //當camera為訊號來源時，設為-1
	}

    //   第二段
	//videofile=cvCaptureFromFile("20100526_155603.avi"); //讀取video
	 //videofile=cvCaptureFromFile("testvideo_out01_320x240_FPS10.avi"); //讀取video   4   bluebag orangebox whitebox  orangebox  FP1(leg)  EE7
	//videofile=cvCaptureFromFile("testvideo_out02_320x240_FPS10.avi"); //讀取video
	//videofile=cvCaptureFromFile("testvideo_out03_320x240_FPS10.avi"); //讀取video 
	//videofile=cvCaptureFromFile("testvideo_out04_320x240_FPS10.avi"); //讀取video 
	//videofile=cvCaptureFromFile("testvideo_out05_320x240_FPS10.avi"); //讀取video   3   bluebag    EE1   many people
	//videofile=cvCaptureFromFile("testvideo_out06_320x240_FPS10.avi"); //讀取video

	//videofile=cvCaptureFromFile("AbnormalDetect_TestVideo_320X240_FPS10.AVI"); //讀取video
	//videofile=cvCaptureFromFile("AbnormalDetect_TestVideo.AVI"); //讀取video


	//videofile=cvCaptureFromFile("0305_1.AVI"); //讀取video           3   bluebag yellowbox whitebox(miss)              EE1
	//videofile=cvCaptureFromFile("0305_2.AVI"); //讀取video           5   bluebag*2 yellowbox*1 whitebox*2 (miss 1)   square nearby library   
	//videofile=cvCaptureFromFile("0305_5.AVI"); //讀取video           6   bluebag*2 yellowbox*2 whitebox*2 (miss 1)    EE1

	//videofile=cvCaptureFromFile("0307_1.AVI"); //讀取video           2   bluebag yellowbox  library
	//videofile=cvCaptureFromFile("0307_3.AVI"); //讀取video           2   bluebag yellowbox  library
	//videofile=cvCaptureFromFile("0307_4.AVI"); //讀取video           2   bluebag yellowbox  library
	//videofile=cvCaptureFromFile("0307_5.AVI"); //讀取video           2   bluebag yellowbox  EE7
	//videofile=cvCaptureFromFile("0307_6.AVI"); //讀取video           1   bluebag yellowbox  EE7

	//videofile=cvCaptureFromFile("0310_1.AVI"); //讀取video           1   bluebag    EE1
	//videofile=cvCaptureFromFile("0310_3.AVI"); //讀取video           1   bluebag    EE1
	//videofile=cvCaptureFromFile("0310_4.AVI"); //讀取video           2   bluebag    EE1

	// ----     framerate=30  -----
	//videofile=cvCaptureFromFile("0518_1.AVI"); //讀取video           2   blackbag moneybag(miss)    工程一館旁廣場
	//videofile=cvCaptureFromFile("0518_2-1.AVI"); //讀取video         4   blackbag moneybag*2 umberlla            FP 1
	//videofile=cvCaptureFromFile("0518_3.AVI"); //讀取video           1   moneybag                    iceshop
	//videofile=cvCaptureFromFile("0518_4.AVI"); //讀取video           4   safehat*2 blackbag bluebag  停車塔旁廣場

	//videofile=cvCaptureFromFile("0520_6.AVI"); //讀取video           2   two moneybag                lab708
	//videofile=cvCaptureFromFile("0520_7.AVI"); //讀取video           2   two moneybag                lab708
	//videofile=cvCaptureFromFile("0520_8.AVI"); //讀取video           4   bread card phone moneybag   my site
	//videofile=cvCaptureFromFile("0520_9.AVI"); //讀取video           2   moneybag tea                my site
	//videofile=cvCaptureFromFile("0520_10.AVI"); //讀取video          4   tea bluebox phone moneybag  my site   FP1 (場景椅子)


		totalframe = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FRAME_COUNT ); //video的frame總數
		FPS = (int)cvGetCaptureProperty( videofile, CV_CAP_PROP_FPS  ); //video的frame rate  

		if(!videofile){ //是否讀檔成功
			printf( "Input Source Error!!! \n" ); 
			exit(0);
		}

		cvCreateTrackbar( "Spatial", "Original",&size,10 ,NULL);        //  "Object Size"  "Spatial domain"
        cvCreateTrackbar( "Temporal", "Original",&sensitivity,10 ,NULL);    //  "Sensitivity"  "Temporal domain"
		

	//*********************************************************************************************************//
	//**********   宣告記憶體與物件   *************************************************************************//
	//*********************************************************************************************************//
	pImage = cvQueryFrame( videofile );

	int alarmtime = 5;  //設定每單位alarm time的大小
	//宣告物件
	CAbnormalDetect *vDetect;
	vDetect=new CAbnormalDetect(320,             //輸入frame的width
								240,             //輸入frame的height
								2,				 //靜止物體持續多少秒才算是預警物件
								alarmtime,       //靜止物體持續多少秒才算是異常物件
								0);				 //異常物件框選多少秒後取消該框選
								          


	//設定物件大小
	//vDetect.SetObjectSize( cvSize(30,30) );

	

    //*********************************************************************************************************//
	//**********   主要處理迴圈   *****************************************************************************//
	//*********************************************************************************************************//
	for(;;){

		vDetect->SetObjectSize(cvGetTrackbarPos("Spatial","Original") , cvGetTrackbarPos("Spatial","Original"));  
		vDetect->SetTimeInterval(cvGetTrackbarPos("Temporal","Original")*alarmtime );  
		//   實際靜止物體持續多少才算是異常物件的秒數設定   與Temporal TrackBar的數字與參數alarmtime有關

		pImage=cvQueryFrame(videofile); //取得frame
		//for(int i=0; i<4;i++){
		
		vDetect->SetImage(pImage); //設定影像資料	

		if( vDetect->UnderInitializing() ){ //正在初始化(建背景)

			printf("Initializing...\n");

		}else{ //已完成初始化，正式進入運算
			if(DetectFlag<10){
				printf("Detection Start !!!\n");
				DetectFlag++;
			}
			
			//判斷是否有警告
			if( vDetect->Alarm() ){

				printf("Warning!!!\n");

			}		
		//}
		}
        
		loop++;

		im1=vDetect->GetImage(1); //讀取當前frame
		im2=vDetect->GetImage(2); //讀取背景
		im3=vDetect->GetImage(3); //讀取前景
		//im4=vDetect.GetImage(4); //讀取optical flow

		//在視窗顯示影像
		cvShowImage( "Original" , im1 );
		if(flag==1){
			cvShowImage( "BackgroundModel" , im2 ); 
			cvShowImage( "ForegroundModel" , im3 );	
		}
		//cvShowImage( "maxObjProb" , im4 );	

		//延遲
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

		//結束video的讀取
		if(loop==(totalframe-2)) 
			break;
		if(loop==2147483600)
			loop=1;

		

	}



    //*********************************************************************************************************//
	//**********   釋放參數與記憶體   *************************************************************************//
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

