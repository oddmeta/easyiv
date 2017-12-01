#include "FaceFeatureDetect.h"

int main(int argc, char* argv[])
{
	/************************************/
	/*		0. 開關ShowImage用flag		*/
	/*		   "1"為ON, "0"為OFF		*/
	/************************************/

	int flag = 1;
	
	/****************************/
	/*		1. 可調參數設定		*/
	/****************************/
/* 640 x 480 */
	// ---------------------------------------------------------------------------------------------
	char input_name[] = "FaceMaskDetect.avi";				// 影片檔
	//const char *input_name = "SuspectDetect.avi";	
	
	/****************************/
	/*		2. 讀取Video		*/
	/****************************/
	
	CvCapture *capture = cvCaptureFromCAM(0);						// From Camera
	if(!capture)
		capture = cvCaptureFromAVI(input_name);						// From Video	
	
	/****************************/
	/*		3. 參數宣告			*/
	/****************************/
	
	IplImage *frame = 0;												
	int pause;														// 暫停('p' or 'P')或結束程式用
	int wait = 1;													// cvWaitKey用
	
			

	CFacefeatureDect* obj;// 宣告一個Object 
	obj = new CFacefeatureDect();

// 	if (obj)
// 	{
// 		delete obj;
// 		obj = NULL;
// 	}

	
//	cvNamedWindow("Result", 1);
	if(flag){
		cvNamedWindow("Input", 1);
		cvNamedWindow("Output", 1);
//		cvNamedWindow("Message", 1);
	}
	
	

	int nFrame = 0;
	/****************************/
	/*		4. 主要處理迴圈		*/
	/****************************/
	
	if(capture){
		while(1){

			nFrame ++;
			if(!cvGrabFrame(capture))	break;						// ++++++++++++++
			frame = cvRetrieveFrame(capture);						// 抓取每張Frame
			if(!frame)	break;										// ++++++++++++++
	
 			obj->SetImage(frame);									// 將影像設給智能元件
			if(obj->exactFace())
				cvShowImage("Output",obj->BGRface);
			char tmpStr[1024];
			sprintf(tmpStr,"Conf:%f",obj->confidence);
			CvFont tFnt=cvFont(1,1);
			cvPutText(obj->inputImg,tmpStr,cvPoint(1,20),&tFnt,CV_RGB(255,0,0));

			/****************************/
			/*		5. Show Image		*/
			/****************************/
//			cvShowImage("Result",			obj.frame_copy);
			if(flag){
				cvShowImage("Input",		obj->inputImg);
//				cvShowImage("Foreground",	obj.Foreground);
//				cvShowImage("Message",		obj.frame_msg);
			}

			
			

			/********************************************/
			/*		6. 其他處理: 延遲 & 測試執行時間	*/
			/********************************************/
			
			if((pause = cvWaitKey(wait)) >= 0){						// +++++++++
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// 暫停 
				else	break;										// 終止程式
			}														// +++++++++
			
		}															//END: while 													
	}																//END: if(capture)	

	
	
	
	/****************************/
	/*		7. Release Memory	*/
	/****************************/
	if (obj)
	{
		delete obj;
		obj = NULL;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Result");  
	if(flag){
		cvDestroyWindow("Input"); 
		cvDestroyWindow("Output");
	}

	return 0;
}