#include "ivs/algorithm/eziv_smartsearch.h"

#pragma comment(lib, "eziv_smartsearch.lib")
#pragma comment(lib, "ezframework.lib")

//冀北跑计
bool stop=false;
bool fram=false;
bool GoOn=false;

int main(int argc, char* argv[])
{
	/************************************/
	/*		0. 开关ShowImage用flag		*/
	/*		   "1"为ON, "0"为OFF		*/
	/************************************/
	BOOL bEnhanceImg	= TRUE;
	CvCapture*	pCapture= 0;				//取得video
	IplImage*	pFrame	= 0;				//从video中取得图片

	int i,total	= 1,pause;
	int wait	= 50;													          // cvWaitKey用
	//撷取输出的资讯	
	int ObjectNum=0;									//输出物件个数 
	int Out=0,In=0;										
	BOOL32 FaceDetect;

	const char* video_name = "D:\\ROSOO\\rslab\\a0-easyivs\\clips\\7.FaceDetect_TestVideo.avi";
	if (argc == 2)
		video_name = argv[1];

	/****************************/
	/*		2. 读取Video		*/
	/****************************/
	pCapture = cvCaptureFromAVI(video_name);
	if(!pCapture)
		pCapture = cvCaptureFromCAM(0); 

	/****************************/
	/*		3. 参数宣告			*/
	/****************************/
	if(cvGrabFrame(pCapture))//先取一张frame做设定
		pFrame = cvRetrieveFrame(pCapture);
	else
	{
		printf("No frame in\n");//没有frame 跳出
		exit(0);//结束程式  可改成return 
	}

	//CIntelliSearch Search(frame->width,frame->height, 0,0,frame->width,frame->height,false);
	//Search(frame宽,frame高,ROI起始点X,ROI起始点Y,ROI宽,ROI高,
	//只搜寻人脸有出现动线(true))
	CIntelliSearch Search;

	TEZIVINFO_INTELLISEARCH tConfig = {0};
	strcpy(tConfig.m_achConfPath, "D:\\ROSOO\\rslab\\a0-easyivs\\conf\\smartsearch");
	tConfig.m_nMaxObjectInFrame	= 5;
	tConfig.m_emSearchMode		= EM_EZIV_INTELLISEARCH_MODE_DEFAULT;
	tConfig.m_nFrameWidth		= pFrame->width;
	tConfig.m_nFrameHeight		= pFrame->height;

	//*************************设定 ROI座标****************************************************************
	tConfig.m_tROI_ltpt.x=0;					//ROI左下点x
	tConfig.m_tROI_ltpt.y=0;					//ROI左下点y
    tConfig.m_tROI_rbpt.x=pFrame->width;				//ROI右上点x
	tConfig.m_tROI_rbpt.y=pFrame->height;			//ROI右上点y
	//设定ROI矩形位置
	tConfig.m_tROIRect.x=0;					//ROI左下点x座标
	tConfig.m_tROIRect.y=0;					//ROI左下点y座标
	tConfig.m_tROIRect.width=pFrame->width;			//ROI宽		
	tConfig.m_tROIRect.height=pFrame->height;			//ROI高

	//*********************光线调整参数****************************************
	tConfig.m_emEnhanceMode = EM_EZIV_IMAGE_ENHANCE_MODE_NONE;
	tConfig.m_dSearchPecent = 1;
	//tConfig.parameter1=0,m_tConfig.parameter2=0;

	Search.Init(tConfig);

	cvNamedWindow( "result", 1 );

	cvNamedWindow( "origin", 1 );
	cvNamedWindow( "background", 1 );
	cvNamedWindow( "foreground", 3 );

#ifdef _ENABLE_ENHANCEMENT_
	if(bEnhanceImg==1)
	{
		int Mode_InitValue=0,Mode_StopValue=3;
		// mode:0 为不调整光线 mode:1 gama调整 mode:2 hist调整 mode:3 gama+hist调整
		int Parameter_one_InitValue=6,Parameter_one_StopValue=10;
		// mode:0 参数无效 mode:1 参数= 光线标准值设定 mode:2 参数= histogram 下限设定 mode:3 参数= 光线标准值设定
		int Parameter_two_InitValue=0,Parameter_two_StopValue=10;
		// mode:0 参数无效 mode:1 参数= gama值设定 mode:2 参数= histogram 上限设定 mode:3 参数= gama值设定
		int Parameter_three_InitValue=0,Parameter_three_StopValue=10;
		// mode:0 参数无效 mode:1 2 3 参数=物件收询扩大范围

		cvCreateTrackbar("Mode","result",&Mode_InitValue,Mode_StopValue,NULL);
		cvCreateTrackbar("Parameter 1","result",&Parameter_one_InitValue,Parameter_one_StopValue,NULL);
		cvCreateTrackbar("Parameter 2","result",&Parameter_two_InitValue,Parameter_two_StopValue,NULL);
	}	
#endif

	/****************************/
	/*		4. 主要处理回圈		*/
	/****************************/
	// 读取capture 
	//-----
	if(pCapture)
	{
		for(;;)
		{
			//从capture里面读取frame
			if( !cvGrabFrame( pCapture))
			{
				break;    // no load frame ,then break
			}
			pFrame = cvRetrieveFrame( pCapture );

#ifdef _ENABLE_ENHANCEMENT_
			//光线调整function
			if(bEnhanceImg)//使用
			{
				cvShowImage( "origin", pFrame);
				EZIV_IMAGE_ENHANCE emMode = (EZIV_IMAGE_ENHANCE)cvGetTrackbarPos("Mode","result");
				if (EZIV_IMAGE_ENHANCE_GAMA == emMode)
				{
					tConfig.m_tGamaTrans.m_nGamaShift = (double)cvGetTrackbarPos("Parameter 1","result");
					tConfig.m_tGamaTrans.m_nGamaValue = (double)cvGetTrackbarPos("Parameter 2","result");
				}
				else if (EZIV_IMAGE_ENHANCE_GAMA_AND_HIST == emMode)
				{
					tConfig.m_tGamaTrans.m_nGamaShift = (double)cvGetTrackbarPos("Parameter 1","result");
					tConfig.m_tGamaTrans.m_nGamaValue = (double)cvGetTrackbarPos("Parameter 2","result");
				}
				else if (EZIV_IMAGE_ENHANCE_HISTOGRAM)
				{
					tConfig.m_tHistEqualize.m_nHistoMinPixel = (double)cvGetTrackbarPos("Parameter 1","result");
					tConfig.m_tHistEqualize.m_nHistoMaxPixel = (double)cvGetTrackbarPos("Parameter 2","result");
				}
				else
				{

				}
				Search.SetAlignmentMode(emMode, tConfig.m_tGamaTrans, tConfig.m_tHistEqualize);
			}
			else//不使用或者是使用预设的光线模式与参数
			{
				Search.SetAlignmentMode(EM_EZIV_IMAGE_ENHANCE_MODE_NONE, tConfig.m_tGamaTrans, tConfig.m_tHistEqualize);  //光线校正函数使用 
			}
#endif
			//背景相减
			Search.Detect(pFrame);

			if( Search.Alarm())
			{
				//有人离开
				ObjectNum=Search.ReturnObjectNum();
				cout<<"out object num:"<<ObjectNum<<'\n';
				for(i=0;i<ObjectNum;i++)
				{
					//输出资料
					cout<<"Total:"<<total<<'\n';
					cout<<"object Num:"<<ObjectNum<<	'\n';
					Search.ReturnData(& In,& Out, &FaceDetect);
					cout<<"InFrameNum:"<<In<<'\n';
					if(FaceDetect)
						cout<<"FaceDetect!!"<<'\n';
					cout<<"OutFrameNum:"<<Out<<'\n';
					total++;
				}
			}

			/****************************/
			/*		5. Show Image		*/
			/****************************/
			if (Search.GetImage(EM_EZIV_IPLIMG_SHOW))
				cvShowImage( "result", Search.GetImage(EM_EZIV_IPLIMG_SHOW));

			/********************************************/
			/*		6. 其他处理: 延迟 & 测试执行时间	*/
			/********************************************/
			if((pause = cvWaitKey(wait)) >= 0)
			{
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// 暂停 
				else	break;										// 终止程式
			}														// +++++++++
		}
	}
	else
	{
		cout<<"no video";
	}

	cvReleaseCapture(&pCapture);	// release capture
	cvDestroyWindow("result");

#ifdef _ENABLE_ENHANCEMENT_
	if(bEnhanceImg)
	{
		cvDestroyWindow("origin");
		cvDestroyWindow("background");
		cvDestroyWindow("foreground");
	}
#endif
	printf("Press any key to exit.\n");
	getchar();
	return 0; 
} 
      