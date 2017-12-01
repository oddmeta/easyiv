#include "ivs/algorithm/eziv_facedetect.h"

#include "windows.h"

static char* EZIV_GetConfigPath(const char* pszProject)
{
	const int MAX_MODNAME_LEN = 256;
	static char path[MAX_MODNAME_LEN];

#ifdef _MSC_VER
	int ret = GetModuleFileName(NULL, path, MAX_MODNAME_LEN);			
	//if (NULL != getcwd(path, sizeof(path)))
	if(ret != 0)
	{
		path[MAX_MODNAME_LEN-1] = '\0';
		s8* temp = strrchr(path, '\\');
		path[temp - path] = '\0';
		//pchBaseModName = GetBaseModName(path);
	}

	strcat(path, "\\conf\\");
	strcat(path, pszProject);
	strcat(path, "\\");
#elif defined(_LINUX_)
	if (NULL != getcwd(path, sizeof(path)))
	{
		//pchBaseModName = basename(path);
	}
	strcat(path, "/conf/");
	strcat(path, pszProject);
	strcat(path, "/");
#endif

	return (char*)path;
}

int main(int args, char* argv[])
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

	CvCapture* capture = 0;								//ȡ��video
	IplImage *frame=0;									//��video��ȡ��ͼƬ

	const char *video_name="libfacedetect_sample.avi"; // ӰƬ��
	if (args == 2)
		video_name = argv[1];

	// ---------------------------------------------------------------------------------------------

	/****************************/
	/*		2. ��ȡVideo		*/
	/****************************/

	capture = cvCaptureFromAVI( video_name );
	if(!capture)
		capture=cvCaptureFromCAM( 0 ); 

	/****************************/
	/*		3. ��������			*/
	/****************************/

	if(cvGrabFrame(capture))//��ȡһ��frame���趨
		frame=cvRetrieveFrame(capture);
	else
	{
		printf("No frame in\n");//û��frame ����
		exit(0);//������ʽ  �ɸĳ�return 
	}

	int pause;												// ��ͣ('p' or 'P')�������ʽ��
	int wait =1;													          // cvWaitKey��

	//�������
	//CFaceDetect face(frame->width,frame->height,110,60,80,100);					//��С��ROI 
	CFaceDetect face;					//��С��ROI 
	// 	 CFaceDetect face(20,0,240,200);						//һ��ROI
	//CFaceDetect face(frame->width,frame->height,0,0,frame->width,frame->height);	                    //ȫ����ROI
	TEZIVINFO_FACEDETECT tFaceParam;
	BOOL32 m_bResizeImg = TRUE;
	int m_wWidth = 320;
	int m_wHeight = 240;
	memset(&tFaceParam, 0, sizeof(tFaceParam));
	if (m_bResizeImg)
	{
		tFaceParam.m_nFrameWidth        = m_wWidth;
		tFaceParam.m_nFrameHeight		= m_wHeight;
	}
	else
	{
		tFaceParam.m_nFrameWidth		= m_wWidth;
		tFaceParam.m_nFrameHeight		= m_wHeight;
	}
	//��ʼ����������֡��
	tFaceParam.m_nBackgroundSamples		= 20;
	//��������ǰ��������С�����20*20Ϊ��С����⵽��������С��
	tFaceParam.tMinArea.area			= 400;
	tFaceParam.tMinArea.width			= 20;
	tFaceParam.tMinArea.height			= 20;
	tFaceParam.scale					= 1;

	//ͼ����ǿ������ز���
	tFaceParam.m_emEnhanceMode						= EM_EZIV_IMAGE_ENHANCE_MODE_NONE;//EZIV_IMAGE_ENHANCE_GAMA;
	tFaceParam.parameter1				= 0;
	tFaceParam.parameter2				= 0;
	tFaceParam.search_range				= 3;//1;

	strncpy(tFaceParam.m_achConfPath, EZIV_GetConfigPath("face"), sizeof(tFaceParam.m_achConfPath) - 1);

	face.Init(tFaceParam);

	cvNamedWindow( "result", 1 );

	if(flag)
	{
		int Mode_InitValue=0,Mode_StopValue=3; 
		// mode:0 Ϊ���������� mode:1 gama���� mode:2 hist���� mode:3 gama+hist����
		int Parameter_one_InitValue=6,Parameter_one_StopValue=10;
		// mode:0 ������Ч mode:1 ����= ���߱�׼ֵ�趨 mode:2 ����= histogram �����趨 mode:3 ����= ���߱�׼ֵ�趨
		int Parameter_two_InitValue=0,Parameter_two_StopValue=10;
		// mode:0 ������Ч mode:1 ����= gamaֵ�趨 mode:2 ����= histogram �����趨 mode:3 ����= gamaֵ�趨
		int Parameter_three_InitValue=0,Parameter_three_StopValue=10;
		// mode:0 ������Ч mode:1 2 3 ����=�����ѯ����Χ

		cvNamedWindow( "origin", 1 );
		cvNamedWindow( "background", 1 );
		cvNamedWindow( "foreground", 3 );

		cvCreateTrackbar("Mode","result",&Mode_InitValue,Mode_StopValue,NULL);
		cvCreateTrackbar("Parameter 1","result",&Parameter_one_InitValue,Parameter_one_StopValue,NULL);
		cvCreateTrackbar("Parameter 2","result",&Parameter_two_InitValue,Parameter_two_StopValue,NULL);
		cvCreateTrackbar("Object","result",&Parameter_three_InitValue,Parameter_three_StopValue,NULL);

	}

	/****************************/
	/*		4. ��Ҫ�����Ȧ		*/
	/****************************/

	// ��ȡcapture 
	//-----
	if(capture)
	{

		for(;;)
		{

			//��capture�����ȡframe
			if( !cvGrabFrame( capture ))
			{
				break;    // no load frame ,then break
			}
			frame = cvRetrieveFrame( capture );

			//���ߵ���function
			if(flag)//ʹ��
			{
				cvShowImage( "origin", frame);
				int nMod, nP1, nP2, nObj;
				nMod = cvGetTrackbarPos("Mode","result");
				nP1  = cvGetTrackbarPos("Parameter 1","result");
				nP2	 = cvGetTrackbarPos("Parameter 2","result");
				nObj = cvGetTrackbarPos("Object","result");
				face.SetAlignmentMode((EZIV_IMAGE_ENHANCE)cvGetTrackbarPos("Mode","result"),(double)cvGetTrackbarPos("Parameter 1","result"),
					(double)cvGetTrackbarPos("Parameter 2","result"));  //����У������ʹ��
				face.SetSearchRange((double)cvGetTrackbarPos("Object","result"));   //�趨��Ѱ��Χ
			}
			else//ʹ�õ�������,������ʹ�ù̶�����,ֻҪ�����������ĳɹ̶�����
			{
				face.SetAlignmentMode((EZIV_IMAGE_ENHANCE)0,0,0);  //����У������ʹ�� 
				//(ģʽ,����һ,������)
				face.SetSearchRange(0);   //�趨��Ѱ��Χ
				//(������)
			}


			//�������
			if(!face.BackgroundSubtraction(frame))
			{
				printf("Initialing\n");
			}
			else
			{
				if(face.BeginDetectFace())  
				{//ǰ�����������С��ʼ���
					face.DetectAndDraw();	 	
				}

				if( face.Alarm()) 
				{//��⵽����
					printf("Face detect!!\n");; 
				}
				else
				{
					printf("Detecting\n");
				}
			}




			/****************************/
			/*		5. Show Image		*/
			/****************************/

			cvShowImage( "result", face.GetImage(EZIV_IPLIMG_SHOW));

			/********************************************/
			/*		6. ��������: �ӳ� & ����ִ��ʱ��	*/
			/********************************************/

			if((pause = cvWaitKey(2)) >= 0){						// +++++++++
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// ��ͣ 
				else	break;										// ��ֹ��ʽ
			}														// +++++++++
		}
	}
	else
	{
		cout<<"no video";
	} 

	/****************************/
	/*		7. Release Memory	*/
	/****************************/


	cvReleaseCapture( &capture );	// release capture
	cvDestroyWindow("result");

	if(flag)
	{
		cvDestroyWindow( "origin");
		cvDestroyWindow( "background");
		cvDestroyWindow( "foreground");
	}
	return 0; 
}