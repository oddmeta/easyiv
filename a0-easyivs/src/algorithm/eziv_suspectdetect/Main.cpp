#include "SuspectDetect.h"



int main(){
	
	/************************************/
	/*		0. �}��ShowImage��flag		*/
	/*		   "1"��ON, "0"��OFF		*/
	/************************************/

	int flag = 1;
	
	
	

	/****************************/
	/*		1. �i�հѼƳ]�w		*/
	/****************************/
	// ---------------------------------------------------------------------------------------------
	const char *input_name = "ATM�r�ި���1(X2).avi";			// �v����
	int dist = 25;												// �P�@�H���ʽd��W��
	int ctrl_time = 3;											// �w�q�P�O��"���d"���ɶ�
	int people_area_min = 350;									// define the min size of people region
	int people_area_max = 4500;									// define the max size of people region
	//int x_I = 110, y_I = 40, width_I = 100, height_I = 150;		// ROD Coordinates
	int x_I = 0, y_I = 0, width_I = 320, height_I = 240;		// ���e������
	// ---------------------------------------------------------------------------------------------

	
	

	/****************************/
	/*		2. Ū��Video		*/
	/****************************/
	
	CvCapture *capture = cvCaptureFromCAM(0);				// From Camera
	if(!capture)
		capture = cvCaptureFromAVI(input_name);				// From Video
	
	
	
	
	/****************************/
	/*		3. �Ѽƫŧi			*/
	/****************************/
	
	IplImage *frame = 0;
	int pause;												// �Ȱ�('p' or 'P')�ε����{����
	int wait = 1;											// cvWaitKey��
	//clock_t start;											// ���{������ɶ���
	//double systime_s=0, systime_f=0, systime=0;							// ���{������ɶ���
	
	CSuspectDetect obj(dist, ctrl_time, people_area_min, people_area_max
		, x_I, y_I, width_I, height_I);						// �ŧi�@��Object 




	cvNamedWindow("Result", 1);
	if(flag){
		cvNamedWindow("Object", 1);
		cvNamedWindow("Foreground", 1);
		cvNamedWindow("Message", 1);
	}
	
	

	
	/****************************/
	/*		4. �D�n�B�z�j��		*/
	/****************************/
	
	if(capture){
		while(1){
			//systime_s = (double)cvGetTickCount();				// �{������_�l�ɶ�(���;msec)
			//start = clock();								// �{������_�l�ɶ�
			
			if(!cvGrabFrame(capture))	break;				// ++++++++++++++
			frame = cvRetrieveFrame(capture);				// ����C�iFrame
			if(!frame)	break;								// ++++++++++++++

			obj.SetImage(frame);							// �N�v���]�����स��
									
			if( obj.UnderInitializing() ){					// �P�_�O�_��l�Ƥ�
				printf("Initializing...\n");				
			}												
			
			else{											// �i�J�������A
								
				if( obj.Alarm() ){							// �P�_�O�_Ĳ�oĵ��
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
			cvShowImage("Result",			obj.frame_copy);
			if(flag){
				cvShowImage("Object",		obj.Object);
				cvShowImage("Foreground",	obj.Foreground);
				cvShowImage("Message",		obj.frame_msg);
			}

			/*		���եΰ����I		*/
			//if(obj.aaa==1){obj.aaa=0;cvWaitKey(0);}					// ���յ{�������I��
			
			
			
			
			/********************************************/
			/*		6. ��L�B�z: ���� & ���հ���ɶ�	*/
			/********************************************/
			
			if((pause = cvWaitKey(wait)) >= 0){						// +++++++++
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// �Ȱ� 
				else	break;										// �פ�{��
			}														// +++++++++
			
			//printf("%f\n",CLOCKS_PER_SEC/(double)(clock()-start));	// ���հ���ɶ�: fps
			//systime_f = (double)cvGetTickCount();					
			//obj.o_EachFrame<<systime_f<<" "<<(systime=systime_f-systime_s)<<" "<<systime/cvGetTickFrequency()/1000.0<<" ";	// �̧��x�s"��������ɶ�", "�`�@����ɶ�", "�����W�v"
			//obj.o_EachFrame<<CLOCKS_PER_SEC/(double)(clock()-start)<<endl;
			
		}															//END: while 													
	}																//END: if(capture)	

	
	
	
	/****************************/
	/*		7. Release Memory	*/
	/****************************/
	
	cvReleaseCapture(&capture);
	cvDestroyWindow("Result");  
	if(flag){
		cvDestroyWindow("Object"); 
		cvDestroyWindow("Foreground");
		cvDestroyWindow("Message");
	}
	
	//obj.o_EachFrame.close();
	//obj.o_SetImage.close();
	//obj.o_UnderInitializing.close();
	//obj.o_Alarm.close();
	//obj.o_MainProcess.close();
	//obj.o_TrainCodebook.close();
	//obj.o_ReorderCodebook.close();
	//obj.o_BGsubtract.close();
	//obj.o_Find_Object.close();
	//obj.o_Add_People_Blob.close();
	//obj.o_Tracking.close();

	
	return 0;
}																	//END: main()
