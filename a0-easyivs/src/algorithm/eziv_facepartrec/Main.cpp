#include "FaceFeatureDetect.h"

int main(int argc, char* argv[])
{
	/************************************/
	/*		0. �}��ShowImage��flag		*/
	/*		   "1"��ON, "0"��OFF		*/
	/************************************/

	int flag = 1;
	
	/****************************/
	/*		1. �i�հѼƳ]�w		*/
	/****************************/
/* 640 x 480 */
	// ---------------------------------------------------------------------------------------------
	char input_name[] = "FaceMaskDetect.avi";				// �v����
	//const char *input_name = "SuspectDetect.avi";	
	
	/****************************/
	/*		2. Ū��Video		*/
	/****************************/
	
	CvCapture *capture = cvCaptureFromCAM(0);						// From Camera
	if(!capture)
		capture = cvCaptureFromAVI(input_name);						// From Video	
	
	/****************************/
	/*		3. �Ѽƫŧi			*/
	/****************************/
	
	IplImage *frame = 0;												
	int pause;														// �Ȱ�('p' or 'P')�ε����{����
	int wait = 1;													// cvWaitKey��
	
			

	CFacefeatureDect* obj;// �ŧi�@��Object 
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
	/*		4. �D�n�B�z�j��		*/
	/****************************/
	
	if(capture){
		while(1){

			nFrame ++;
			if(!cvGrabFrame(capture))	break;						// ++++++++++++++
			frame = cvRetrieveFrame(capture);						// ����C�iFrame
			if(!frame)	break;										// ++++++++++++++
	
 			obj->SetImage(frame);									// �N�v���]�����स��
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
			/*		6. ��L�B�z: ���� & ���հ���ɶ�	*/
			/********************************************/
			
			if((pause = cvWaitKey(wait)) >= 0){						// +++++++++
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// �Ȱ� 
				else	break;										// �פ�{��
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