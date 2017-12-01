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
	/*		0. 开关ShowImage用flag		*/
	/*		   "1"为ON, "0"为OFF		*/
	/************************************/

	int flag = 1;

	/****************************/
	/*		1. 可调参数设定		*/
	/****************************/

	// ---------------------------------------------------------------------------------------------

	CvCapture* capture = 0;								//取得video
	IplImage *frame=0;									//从video中取得图片

	const char *video_name="libfacedetect_sample.avi"; // 影片档
	if (args == 2)
		video_name = argv[1];

	// ---------------------------------------------------------------------------------------------

	/****************************/
	/*		2. 读取Video		*/
	/****************************/

	capture = cvCaptureFromAVI( video_name );
	if(!capture)
		capture=cvCaptureFromCAM( 0 ); 

	/****************************/
	/*		3. 参数宣告			*/
	/****************************/

	if(cvGrabFrame(capture))//先取一张frame做设定
		frame=cvRetrieveFrame(capture);
	else
	{
		printf("No frame in\n");//没有frame 跳出
		exit(0);//结束程式  可改成return 
	}

	int pause;												// 暂停('p' or 'P')或结束程式用
	int wait =1;													          // cvWaitKey用

	//物件宣告
	//CFaceDetect face(frame->width,frame->height,110,60,80,100);					//缩小版ROI 
	CFaceDetect face;					//缩小版ROI 
	// 	 CFaceDetect face(20,0,240,200);						//一般ROI
	//CFaceDetect face(frame->width,frame->height,0,0,frame->width,frame->height);	                    //全画面ROI
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
	//初始化背景所需帧数
	tFaceParam.m_nBackgroundSamples		= 20;
	//启动侦测的前景区块最小面积（20*20为最小能侦测到的人脸大小）
	tFaceParam.tMinArea.area			= 400;
	tFaceParam.tMinArea.width			= 20;
	tFaceParam.tMinArea.height			= 20;
	tFaceParam.scale					= 1;

	//图像增强处理相关参数
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
		// mode:0 为不调整光线 mode:1 gama调整 mode:2 hist调整 mode:3 gama+hist调整
		int Parameter_one_InitValue=6,Parameter_one_StopValue=10;
		// mode:0 参数无效 mode:1 参数= 光线标准值设定 mode:2 参数= histogram 下限设定 mode:3 参数= 光线标准值设定
		int Parameter_two_InitValue=0,Parameter_two_StopValue=10;
		// mode:0 参数无效 mode:1 参数= gama值设定 mode:2 参数= histogram 上限设定 mode:3 参数= gama值设定
		int Parameter_three_InitValue=0,Parameter_three_StopValue=10;
		// mode:0 参数无效 mode:1 2 3 参数=物件收询扩大范围

		cvNamedWindow( "origin", 1 );
		cvNamedWindow( "background", 1 );
		cvNamedWindow( "foreground", 3 );

		cvCreateTrackbar("Mode","result",&Mode_InitValue,Mode_StopValue,NULL);
		cvCreateTrackbar("Parameter 1","result",&Parameter_one_InitValue,Parameter_one_StopValue,NULL);
		cvCreateTrackbar("Parameter 2","result",&Parameter_two_InitValue,Parameter_two_StopValue,NULL);
		cvCreateTrackbar("Object","result",&Parameter_three_InitValue,Parameter_three_StopValue,NULL);

	}

	/****************************/
	/*		4. 主要处理回圈		*/
	/****************************/

	// 读取capture 
	//-----
	if(capture)
	{

		for(;;)
		{

			//从capture里面读取frame
			if( !cvGrabFrame( capture ))
			{
				break;    // no load frame ,then break
			}
			frame = cvRetrieveFrame( capture );

			//光线调整function
			if(flag)//使用
			{
				cvShowImage( "origin", frame);
				int nMod, nP1, nP2, nObj;
				nMod = cvGetTrackbarPos("Mode","result");
				nP1  = cvGetTrackbarPos("Parameter 1","result");
				nP2	 = cvGetTrackbarPos("Parameter 2","result");
				nObj = cvGetTrackbarPos("Object","result");
				face.SetAlignmentMode((EZIV_IMAGE_ENHANCE)cvGetTrackbarPos("Mode","result"),(double)cvGetTrackbarPos("Parameter 1","result"),
					(double)cvGetTrackbarPos("Parameter 2","result"));  //光线校正函数使用
				face.SetSearchRange((double)cvGetTrackbarPos("Object","result"));   //设定收寻范围
			}
			else//使用但不调整,或者是使用固定参数,只要将下两函数改成固定数字
			{
				face.SetAlignmentMode((EZIV_IMAGE_ENHANCE)0,0,0);  //光线校正函数使用 
				//(模式,参数一,参数二)
				face.SetSearchRange(0);   //设定收寻范围
				//(参数三)
			}


			//背景相减
			if(!face.BackgroundSubtraction(frame))
			{
				printf("Initialing\n");
			}
			else
			{
				if(face.BeginDetectFace())  
				{//前景大於人脸大小开始侦测
					face.DetectAndDraw();	 	
				}

				if( face.Alarm()) 
				{//侦测到人脸
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
			/*		6. 其他处理: 延迟 & 测试执行时间	*/
			/********************************************/

			if((pause = cvWaitKey(2)) >= 0){						// +++++++++
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// 暂停 
				else	break;										// 终止程式
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