#include "BlurDetect.h"


int main()
{
	/************************************/
	/*		0. 开关ShowImage用flag		*/
	/*		   "1"为ON, "0"为OFF		*/
	/************************************/
	int flag = 1;

	/****************************/
	/*		1. 可调参数设定		*/
	/****************************/

	// ---------------------------------------------------------------------------------------------
	
	//const char *input_name = "00252_640.avi";			// 影片档
	//const char *input_name = "00253_640.avi";			// 影片档
	//const char *input_name = "00247_640.AVI";		// 影片档
	
	//const char *input_name = "前景变化.AVI";			// 影片档
	const char *input_name = "frontmotion.AVI";			// 影片档
	
	CvCapture *capture = 0;                             //影片指标
	bool resize_frame_flag=1;							//进去的frame是否要统一大小为320*240(可使用预设参数

	int edge_thr=20;									//Edge的threshold 预设为20 合理范围(>1) 建议范围(20~255)
	int sobel_size=3;									//Edge的filter size 预设为3 合理范围(3,5,7) 正比於影像大小
	int filter_size=20;									//image filter的size 可以调整 预设为20 合理范围(>1) 正比於影像大小
	double initial_second=5;                            //设定初使要多少时间  预设为五秒
	float Focus_max=6;                                  //Focus最大值为多少 预设为6 合理范围(>1) 建议范围(4~20) 正比於影像大小
	float Image_max=(float)1;                                //Image_Parameter最大值为多少 预设为1 合理范围(0~1)  建议范围(0.7~1)
	float Focus_threshold=40;								//Focus_Parameter 的threshold 合理范围(0~100)  建议范围(40~100)
	float Image_threshold=40;								//Image_Parameter 的threshold 合理范围(0~100)  建议范围(40~100)
	int Alarm_second=3;	                                //Alarm在多少时间间格内去计算 预设为5 合理范围(>0)								
	float Alarm_percent=50;								//时间内模糊情况超过多少百分比即发出警讯 合理范围(0~100) 建议范围(50~100)
	int Height,Width;                                   //抓取影像长宽
	int Frame_Skip =1;                                 //skip frame设定，1表示不跳frame，2表跳一张frame(frame%2==0时才做)，依此类推

	/****************************/
	/*		2. 读取Video		*/
	/****************************/
	
	capture = cvCaptureFromAVI(input_name);						// From Video
	if(!capture)
		capture = cvCaptureFromCAM(0);						// From Camera
	
	/****************************/
	/*		3. 参数宣告			*/
	/****************************/
	
	IplImage *frame = 0;													  //读取video input frame
	IplImage *frame_resize = 0;
	int pause;															      // 暂停('p' or 'P')或结束程式用
	int wait =1;													          // cvWaitKey用
	time_t now_time;                                                //目前时间参数
	time_t now_time2;
	int frame_count=0;

	if(cvGrabFrame(capture))//先取一张frame做设定
        frame=cvRetrieveFrame(capture);
	else
	{
		printf("No frame in\n");//没有frame 跳出
		exit(0);//结束程式  可改成return 
	}

	if(resize_frame_flag==1)
	{
		frame_resize=cvCreateImage(cvSize(320,240), IPL_DEPTH_8U, 3); //resize frame到320*240
		frame_resize->origin=frame->origin;
		cvResize(frame,frame_resize,CV_INTER_LINEAR);
	}
	else
	{
		frame_resize=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); //纯粹复制frame
		frame_resize->origin=frame->origin;
		cvCopy(frame,frame_resize);
	}

	CBlurDetect obj(frame_resize->width,frame_resize->height,edge_thr,sobel_size,filter_size,initial_second,Focus_max,
			Image_max,Focus_threshold,Image_threshold,Alarm_second,Alarm_percent);			// 宣告一个Object
	

	
	



	int edge_InitValue=20,edge_StopValue=100;//edge threshold设定参数 初使为20 目前最大设为100 可调
	int sobel_InitValue=0,sobel_StopValue=3;//sobel设定参数 初使为0 目前最大设为3 可调 0=3 1=3 2=5 3=7;
	
	int filter_size_InitValue=8,filter_size_StopValue=100;//filter_size设定参数 初使为20 目前最大设为100 可调
	int initial_InitValue=5,initial_StopValue=100;//initial second设定参数 初使为5 目前最大设为100 可调
	int Focus_max_InitValue=4,Focus_max_StopValue=20;//Focus_max设定参数 初使为6 目前最大设为20 可调
	
	int Image_max_InitValue=8,Image_max_StopValue=10;//Image_max设定参数 初使为10 目前最大设为10 可调 1=0.1 2=0.2 ...10=1
	int Focus_threshold_InitValue=40,Focus_threshold_StopValue=100;//Focus_threshold设定参数 初使为40 目前最大设为100 可调
	int Image_threshold_InitValue=40,Image_threshold_StopValue=100;//Image_threshold设定参数 初使为40 目前最大设为100 可调
	int Alarm_second_InitValue=3,Alarm_second_StopValue=100;//Alarm_second设定参数 初使为3 目前最大设为100 可调
	int Alarm_percent_InitValue=50,Alarm_percent_StopValue=100;//Alarm_percent设定参数 初使为50 目前最大设为100 可调
	int Reinitial_InitValue=0,Reinitial_StopValue=1;//Reinitial设定参数 初使为0 目前最大设为1 0=off 1=on

	int Blur_Sensitive_Initvalue=10, Blur_Sensitive_StopValue=19;
	int Focus_Sensitive_Initvalue=14, Focus_Sensitive_StopValue=19;

	cvNamedWindow("Result", 1);//一般影片播放
	if(flag){
		
		cvNamedWindow("Edge Image", 1);//focus edge计算图
		cvNamedWindow("Blur Gray Image", 1);//image blur计算图

		//cvNamedWindow("Parameter", 1);//参数调整视窗
		//cvNamedWindow("threshold", 1);//threshold调整视窗
		cvNamedWindow("Message", 1);//讯息图 F=focus I=image N=目前计算参数数值 M=参数最小值 P=模糊程度百分比 T=Alarm时间百分比
		//cvResizeWindow("Parameter", 320, 320);
		//cvResizeWindow("threshold", 320, 320);

		cvNamedWindow("Sensitive", 1);//sensitive调整视窗
		cvResizeWindow("Sensitive", 480, 100);

		/*
		int edge_InitValue=20,edge_StopValue=100;//edge threshold设定参数 初使为20 目前最大设为100 可调
		int sobel_InitValue=0,sobel_StopValue=3;//sobel设定参数 初使为0 目前最大设为3 可调 0=3 1=3 2=5 3=7;
		
		int filter_size_InitValue=8,filter_size_StopValue=100;//filter_size设定参数 初使为20 目前最大设为100 可调
		int initial_InitValue=5,initial_StopValue=100;//initial second设定参数 初使为5 目前最大设为100 可调
		int Focus_max_InitValue=6,Focus_max_StopValue=20;//Focus_max设定参数 初使为6 目前最大设为20 可调
		
		int Image_max_InitValue=8,Image_max_StopValue=10;//Image_max设定参数 初使为10 目前最大设为10 可调 1=0.1 2=0.2 ...10=1
		int Focus_threshold_InitValue=40,Focus_threshold_StopValue=100;//Focus_threshold设定参数 初使为40 目前最大设为100 可调
		int Image_threshold_InitValue=40,Image_threshold_StopValue=100;//Image_threshold设定参数 初使为40 目前最大设为100 可调
		int Alarm_second_InitValue=3,Alarm_second_StopValue=100;//Alarm_second设定参数 初使为3 目前最大设为100 可调
		int Alarm_percent_InitValue=50,Alarm_percent_StopValue=100;//Alarm_percent设定参数 初使为50 目前最大设为100 可调
		int Reinitial_InitValue=0,Reinitial_StopValue=1;//Reinitial设定参数 初使为0 目前最大设为1 0=off 1=on

		int Sensitive_Initvalue=8, Sensitive_StopValue=15;*/


		//创造调整条
		//cvCreateTrackbar("edge_thr","Parameter",&edge_InitValue,edge_StopValue,NULL);
		//cvCreateTrackbar("sobel_filter","Parameter",&sobel_InitValue,sobel_StopValue,NULL);
		//cvCreateTrackbar("filter_size","Parameter",&filter_size_InitValue,filter_size_StopValue,NULL);
		//cvCreateTrackbar("initial_second","Parameter",&initial_InitValue,initial_StopValue,NULL);
		//cvCreateTrackbar("Focus_max","Parameter",&Focus_max_InitValue,Focus_max_StopValue,NULL);
		//cvCreateTrackbar("Image_max","Parameter",&Image_max_InitValue,Image_max_StopValue,NULL);
		//cvCreateTrackbar("Focus_threshold","threshold",&Focus_threshold_InitValue,Focus_threshold_StopValue,NULL);
		//cvCreateTrackbar("Image_threshold","threshold",&Image_threshold_InitValue,Image_threshold_StopValue,NULL);
		//cvCreateTrackbar("Alarm_second","threshold",&Alarm_second_InitValue,Alarm_second_StopValue,NULL);
		//cvCreateTrackbar("Alarm_percent","threshold",&Alarm_percent_InitValue,Alarm_percent_StopValue,NULL);
		//cvCreateTrackbar("Reinitial","threshold",&Reinitial_InitValue,Reinitial_StopValue,NULL);

		cvCreateTrackbar("B_Sensitive","Sensitive",&Blur_Sensitive_Initvalue,Blur_Sensitive_StopValue,NULL);
		cvCreateTrackbar("F_Sensitive","Sensitive",&Focus_Sensitive_Initvalue,Focus_Sensitive_StopValue,NULL);
		
	}

	
	
	

	
	/****************************/
	/*		4. 主要处理回圈		*/
	/****************************/
	
	if(capture){
		time(&now_time2);
		while(1){
			
			if(!cvGrabFrame(capture))	break;						// ++++++++++++++
			frame = cvRetrieveFrame(capture);						// 抓取每张Frame
			if(!frame)	break;										// ++++++++++++++
			Width = cvGetDimSize(frame,1);
			
			frame_count++;

			
			time(&now_time);										//取得时间
			if(frame_count%Frame_Skip==0)
			{
				if((int)(now_time - now_time2)>0)
				printf("fps = %f  ",(double)( (double)(frame_count/Frame_Skip) /(int)(now_time - now_time2)));   //计算fps
			

				if(resize_frame_flag==1)
					cvResize(frame,frame_resize,CV_INTER_LINEAR);       //resize frame
				else
					cvCopy(frame,frame_resize);                         //维持原影像

				obj.SetImage(frame_resize,now_time);					// 将影像设给智能元件
				
				if( obj.UnderInitializing() ){							// 判断是否初始化中
					printf("Initializing...\n");				
				}												
				
				else{													// 进入侦测状态
					
					if( obj.Alarm() ){									// 判断是否触发警报
						printf("Warning!!!\n");					
					}
					else
					{
						printf("Detecting...\n");				
					}
				}


				
				/****************************/
				/*		5. Show Image		*/
				/****************************/
				
				cvShowImage("Result",			frame_resize);

				if(flag){
					cvShowImage("Edge Image",		obj.edge);//edge影像
					cvShowImage("Blur Gray Image",	obj.blurV);//模糊影像
					cvShowImage("Message",		obj.frame_msg);//讯息影像

					//调参数
					
					/*--------------------------------------------------------------------------------------
					obj.SetParameter(cvGetTrackbarPos("edge_thr","Parameter"),
					(int)(cvGetTrackbarPos("sobel_filter","Parameter")*2+1),
					cvGetTrackbarPos("filter_size","Parameter"),
					(double)cvGetTrackbarPos("initial_second","Parameter"),
					(float)cvGetTrackbarPos("Focus_max","Parameter"),
					(float)(cvGetTrackbarPos("Image_max","Parameter")*0.1),
					(float)cvGetTrackbarPos("Focus_threshold","threshold"),
					(float)cvGetTrackbarPos("Image_threshold","threshold"),
					cvGetTrackbarPos("Alarm_second","threshold"),
					(float)cvGetTrackbarPos("Alarm_percent","threshold"));  //设定参数
					//重新initial
					if(cvGetTrackbarPos("Reinitial","threshold")!=0)
					obj.ReInitializing();
					//---------------------------------------------------------------------------------------*/

					obj.SetParameter(edge_InitValue,
						(int)(sobel_InitValue*2+1),
						(Width/320)*(20 - cvGetTrackbarPos("B_Sensitive","Sensitive")),
						(double)initial_InitValue,
						(float)(20 - cvGetTrackbarPos("F_Sensitive","Sensitive")),
						(float)(Image_max_InitValue*0.1),
						(float)Focus_threshold_InitValue,
						(float)Image_threshold_InitValue,
						Alarm_second_InitValue,
						(float)Alarm_percent_InitValue);  //设定参数

					//重新initial
					if(Reinitial_InitValue!=0)
						obj.ReInitializing();

				}

				/********************************************/
				/*		6. 其他处理: 延迟 & 测试执行时间	*/
				/********************************************/
				
				if((pause = cvWaitKey(wait)) >= 0){						// +++++++++
					if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// 暂停 
					else	break;										// 终止程式
				}														// +++++++++
				//cvWaitKey(0);
			}
			
		}															//END: while 													
	}																//END: if(capture)	

	
	
	
	/****************************/
	/*		7. Release Memory	*/
	/****************************/
	
	cvReleaseCapture(&capture);
	cvReleaseImage(&frame_resize);
	cvDestroyWindow("Result");  
	if(flag){
		cvDestroyWindow("Edge Image"); 
		cvDestroyWindow("Blur Gray Image");
		cvDestroyWindow("Message");
		cvDestroyWindow("Parameter");
		cvDestroyWindow("threshold");
	}



	
	return 0;
}																	//END: main()
