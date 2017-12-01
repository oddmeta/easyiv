#include "SuspectDetect.h"



int main(){
	
	/************************************/
	/*		0. 開關ShowImage用flag		*/
	/*		   "1"為ON, "0"為OFF		*/
	/************************************/

	int flag = 1;
	
	
	

	/****************************/
	/*		1. 可調參數設定		*/
	/****************************/
	// ---------------------------------------------------------------------------------------------
	const char *input_name = "ATM徘徊角度1(X2).avi";			// 影片檔
	int dist = 25;												// 同一人移動範圍上限
	int ctrl_time = 3;											// 定義判別為"滯留"的時間
	int people_area_min = 350;									// define the min size of people region
	int people_area_max = 4500;									// define the max size of people region
	//int x_I = 110, y_I = 40, width_I = 100, height_I = 150;		// ROD Coordinates
	int x_I = 0, y_I = 0, width_I = 320, height_I = 240;		// 全畫面偵測
	// ---------------------------------------------------------------------------------------------

	
	

	/****************************/
	/*		2. 讀取Video		*/
	/****************************/
	
	CvCapture *capture = cvCaptureFromCAM(0);				// From Camera
	if(!capture)
		capture = cvCaptureFromAVI(input_name);				// From Video
	
	
	
	
	/****************************/
	/*		3. 參數宣告			*/
	/****************************/
	
	IplImage *frame = 0;
	int pause;												// 暫停('p' or 'P')或結束程式用
	int wait = 1;											// cvWaitKey用
	//clock_t start;											// 測程式執行時間用
	//double systime_s=0, systime_f=0, systime=0;							// 測程式執行時間用
	
	CSuspectDetect obj(dist, ctrl_time, people_area_min, people_area_max
		, x_I, y_I, width_I, height_I);						// 宣告一個Object 




	cvNamedWindow("Result", 1);
	if(flag){
		cvNamedWindow("Object", 1);
		cvNamedWindow("Foreground", 1);
		cvNamedWindow("Message", 1);
	}
	
	

	
	/****************************/
	/*		4. 主要處理迴圈		*/
	/****************************/
	
	if(capture){
		while(1){
			//systime_s = (double)cvGetTickCount();				// 程式執行起始時間(單位;msec)
			//start = clock();								// 程式執行起始時間
			
			if(!cvGrabFrame(capture))	break;				// ++++++++++++++
			frame = cvRetrieveFrame(capture);				// 抓取每張Frame
			if(!frame)	break;								// ++++++++++++++

			obj.SetImage(frame);							// 將影像設給智能元件
									
			if( obj.UnderInitializing() ){					// 判斷是否初始化中
				printf("Initializing...\n");				
			}												
			
			else{											// 進入偵測狀態
								
				if( obj.Alarm() ){							// 判斷是否觸發警報
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

			/*		測試用停止點		*/
			//if(obj.aaa==1){obj.aaa=0;cvWaitKey(0);}					// 測試程式停止點用
			
			
			
			
			/********************************************/
			/*		6. 其他處理: 延遲 & 測試執行時間	*/
			/********************************************/
			
			if((pause = cvWaitKey(wait)) >= 0){						// +++++++++
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// 暫停 
				else	break;										// 終止程式
			}														// +++++++++
			
			//printf("%f\n",CLOCKS_PER_SEC/(double)(clock()-start));	// 測試執行時間: fps
			//systime_f = (double)cvGetTickCount();					
			//obj.o_EachFrame<<systime_f<<" "<<(systime=systime_f-systime_s)<<" "<<systime/cvGetTickFrequency()/1000.0<<" ";	// 依序儲存"結束執行時間", "總共執行時間", "執行頻率"
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
