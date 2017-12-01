#include "BlurDetect.h"


int main()
{
	/************************************/
	/*		0. ����ShowImage��flag		*/
	/*		   "1"ΪON, "0"ΪOFF		*/
	/************************************/
	int flag = 1;

	/****************************/
	/*		1. �ɵ������趨		*/
	/****************************/

	// ---------------------------------------------------------------------------------------------
	
	//const char *input_name = "00252_640.avi";			// ӰƬ��
	//const char *input_name = "00253_640.avi";			// ӰƬ��
	//const char *input_name = "00247_640.AVI";		// ӰƬ��
	
	//const char *input_name = "ǰ���仯.AVI";			// ӰƬ��
	const char *input_name = "frontmotion.AVI";			// ӰƬ��
	
	CvCapture *capture = 0;                             //ӰƬָ��
	bool resize_frame_flag=1;							//��ȥ��frame�Ƿ�Ҫͳһ��СΪ320*240(��ʹ��Ԥ�����

	int edge_thr=20;									//Edge��threshold Ԥ��Ϊ20 ����Χ(>1) ���鷶Χ(20~255)
	int sobel_size=3;									//Edge��filter size Ԥ��Ϊ3 ����Χ(3,5,7) �����Ӱ���С
	int filter_size=20;									//image filter��size ���Ե��� Ԥ��Ϊ20 ����Χ(>1) �����Ӱ���С
	double initial_second=5;                            //�趨��ʹҪ����ʱ��  Ԥ��Ϊ����
	float Focus_max=6;                                  //Focus���ֵΪ���� Ԥ��Ϊ6 ����Χ(>1) ���鷶Χ(4~20) �����Ӱ���С
	float Image_max=(float)1;                                //Image_Parameter���ֵΪ���� Ԥ��Ϊ1 ����Χ(0~1)  ���鷶Χ(0.7~1)
	float Focus_threshold=40;								//Focus_Parameter ��threshold ����Χ(0~100)  ���鷶Χ(40~100)
	float Image_threshold=40;								//Image_Parameter ��threshold ����Χ(0~100)  ���鷶Χ(40~100)
	int Alarm_second=3;	                                //Alarm�ڶ���ʱ������ȥ���� Ԥ��Ϊ5 ����Χ(>0)								
	float Alarm_percent=50;								//ʱ����ģ������������ٰٷֱȼ�������Ѷ ����Χ(0~100) ���鷶Χ(50~100)
	int Height,Width;                                   //ץȡӰ�񳤿�
	int Frame_Skip =1;                                 //skip frame�趨��1��ʾ����frame��2����һ��frame(frame%2==0ʱ����)����������

	/****************************/
	/*		2. ��ȡVideo		*/
	/****************************/
	
	capture = cvCaptureFromAVI(input_name);						// From Video
	if(!capture)
		capture = cvCaptureFromCAM(0);						// From Camera
	
	/****************************/
	/*		3. ��������			*/
	/****************************/
	
	IplImage *frame = 0;													  //��ȡvideo input frame
	IplImage *frame_resize = 0;
	int pause;															      // ��ͣ('p' or 'P')�������ʽ��
	int wait =1;													          // cvWaitKey��
	time_t now_time;                                                //Ŀǰʱ�����
	time_t now_time2;
	int frame_count=0;

	if(cvGrabFrame(capture))//��ȡһ��frame���趨
        frame=cvRetrieveFrame(capture);
	else
	{
		printf("No frame in\n");//û��frame ����
		exit(0);//������ʽ  �ɸĳ�return 
	}

	if(resize_frame_flag==1)
	{
		frame_resize=cvCreateImage(cvSize(320,240), IPL_DEPTH_8U, 3); //resize frame��320*240
		frame_resize->origin=frame->origin;
		cvResize(frame,frame_resize,CV_INTER_LINEAR);
	}
	else
	{
		frame_resize=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); //���⸴��frame
		frame_resize->origin=frame->origin;
		cvCopy(frame,frame_resize);
	}

	CBlurDetect obj(frame_resize->width,frame_resize->height,edge_thr,sobel_size,filter_size,initial_second,Focus_max,
			Image_max,Focus_threshold,Image_threshold,Alarm_second,Alarm_percent);			// ����һ��Object
	

	
	



	int edge_InitValue=20,edge_StopValue=100;//edge threshold�趨���� ��ʹΪ20 Ŀǰ�����Ϊ100 �ɵ�
	int sobel_InitValue=0,sobel_StopValue=3;//sobel�趨���� ��ʹΪ0 Ŀǰ�����Ϊ3 �ɵ� 0=3 1=3 2=5 3=7;
	
	int filter_size_InitValue=8,filter_size_StopValue=100;//filter_size�趨���� ��ʹΪ20 Ŀǰ�����Ϊ100 �ɵ�
	int initial_InitValue=5,initial_StopValue=100;//initial second�趨���� ��ʹΪ5 Ŀǰ�����Ϊ100 �ɵ�
	int Focus_max_InitValue=4,Focus_max_StopValue=20;//Focus_max�趨���� ��ʹΪ6 Ŀǰ�����Ϊ20 �ɵ�
	
	int Image_max_InitValue=8,Image_max_StopValue=10;//Image_max�趨���� ��ʹΪ10 Ŀǰ�����Ϊ10 �ɵ� 1=0.1 2=0.2 ...10=1
	int Focus_threshold_InitValue=40,Focus_threshold_StopValue=100;//Focus_threshold�趨���� ��ʹΪ40 Ŀǰ�����Ϊ100 �ɵ�
	int Image_threshold_InitValue=40,Image_threshold_StopValue=100;//Image_threshold�趨���� ��ʹΪ40 Ŀǰ�����Ϊ100 �ɵ�
	int Alarm_second_InitValue=3,Alarm_second_StopValue=100;//Alarm_second�趨���� ��ʹΪ3 Ŀǰ�����Ϊ100 �ɵ�
	int Alarm_percent_InitValue=50,Alarm_percent_StopValue=100;//Alarm_percent�趨���� ��ʹΪ50 Ŀǰ�����Ϊ100 �ɵ�
	int Reinitial_InitValue=0,Reinitial_StopValue=1;//Reinitial�趨���� ��ʹΪ0 Ŀǰ�����Ϊ1 0=off 1=on

	int Blur_Sensitive_Initvalue=10, Blur_Sensitive_StopValue=19;
	int Focus_Sensitive_Initvalue=14, Focus_Sensitive_StopValue=19;

	cvNamedWindow("Result", 1);//һ��ӰƬ����
	if(flag){
		
		cvNamedWindow("Edge Image", 1);//focus edge����ͼ
		cvNamedWindow("Blur Gray Image", 1);//image blur����ͼ

		//cvNamedWindow("Parameter", 1);//���������Ӵ�
		//cvNamedWindow("threshold", 1);//threshold�����Ӵ�
		cvNamedWindow("Message", 1);//ѶϢͼ F=focus I=image N=Ŀǰ���������ֵ M=������Сֵ P=ģ���̶Ȱٷֱ� T=Alarmʱ��ٷֱ�
		//cvResizeWindow("Parameter", 320, 320);
		//cvResizeWindow("threshold", 320, 320);

		cvNamedWindow("Sensitive", 1);//sensitive�����Ӵ�
		cvResizeWindow("Sensitive", 480, 100);

		/*
		int edge_InitValue=20,edge_StopValue=100;//edge threshold�趨���� ��ʹΪ20 Ŀǰ�����Ϊ100 �ɵ�
		int sobel_InitValue=0,sobel_StopValue=3;//sobel�趨���� ��ʹΪ0 Ŀǰ�����Ϊ3 �ɵ� 0=3 1=3 2=5 3=7;
		
		int filter_size_InitValue=8,filter_size_StopValue=100;//filter_size�趨���� ��ʹΪ20 Ŀǰ�����Ϊ100 �ɵ�
		int initial_InitValue=5,initial_StopValue=100;//initial second�趨���� ��ʹΪ5 Ŀǰ�����Ϊ100 �ɵ�
		int Focus_max_InitValue=6,Focus_max_StopValue=20;//Focus_max�趨���� ��ʹΪ6 Ŀǰ�����Ϊ20 �ɵ�
		
		int Image_max_InitValue=8,Image_max_StopValue=10;//Image_max�趨���� ��ʹΪ10 Ŀǰ�����Ϊ10 �ɵ� 1=0.1 2=0.2 ...10=1
		int Focus_threshold_InitValue=40,Focus_threshold_StopValue=100;//Focus_threshold�趨���� ��ʹΪ40 Ŀǰ�����Ϊ100 �ɵ�
		int Image_threshold_InitValue=40,Image_threshold_StopValue=100;//Image_threshold�趨���� ��ʹΪ40 Ŀǰ�����Ϊ100 �ɵ�
		int Alarm_second_InitValue=3,Alarm_second_StopValue=100;//Alarm_second�趨���� ��ʹΪ3 Ŀǰ�����Ϊ100 �ɵ�
		int Alarm_percent_InitValue=50,Alarm_percent_StopValue=100;//Alarm_percent�趨���� ��ʹΪ50 Ŀǰ�����Ϊ100 �ɵ�
		int Reinitial_InitValue=0,Reinitial_StopValue=1;//Reinitial�趨���� ��ʹΪ0 Ŀǰ�����Ϊ1 0=off 1=on

		int Sensitive_Initvalue=8, Sensitive_StopValue=15;*/


		//���������
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
	/*		4. ��Ҫ�����Ȧ		*/
	/****************************/
	
	if(capture){
		time(&now_time2);
		while(1){
			
			if(!cvGrabFrame(capture))	break;						// ++++++++++++++
			frame = cvRetrieveFrame(capture);						// ץȡÿ��Frame
			if(!frame)	break;										// ++++++++++++++
			Width = cvGetDimSize(frame,1);
			
			frame_count++;

			
			time(&now_time);										//ȡ��ʱ��
			if(frame_count%Frame_Skip==0)
			{
				if((int)(now_time - now_time2)>0)
				printf("fps = %f  ",(double)( (double)(frame_count/Frame_Skip) /(int)(now_time - now_time2)));   //����fps
			

				if(resize_frame_flag==1)
					cvResize(frame,frame_resize,CV_INTER_LINEAR);       //resize frame
				else
					cvCopy(frame,frame_resize);                         //ά��ԭӰ��

				obj.SetImage(frame_resize,now_time);					// ��Ӱ���������Ԫ��
				
				if( obj.UnderInitializing() ){							// �ж��Ƿ��ʼ����
					printf("Initializing...\n");				
				}												
				
				else{													// �������״̬
					
					if( obj.Alarm() ){									// �ж��Ƿ񴥷�����
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
					cvShowImage("Edge Image",		obj.edge);//edgeӰ��
					cvShowImage("Blur Gray Image",	obj.blurV);//ģ��Ӱ��
					cvShowImage("Message",		obj.frame_msg);//ѶϢӰ��

					//������
					
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
					(float)cvGetTrackbarPos("Alarm_percent","threshold"));  //�趨����
					//����initial
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
						(float)Alarm_percent_InitValue);  //�趨����

					//����initial
					if(Reinitial_InitValue!=0)
						obj.ReInitializing();

				}

				/********************************************/
				/*		6. ��������: �ӳ� & ����ִ��ʱ��	*/
				/********************************************/
				
				if((pause = cvWaitKey(wait)) >= 0){						// +++++++++
					if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// ��ͣ 
					else	break;										// ��ֹ��ʽ
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
