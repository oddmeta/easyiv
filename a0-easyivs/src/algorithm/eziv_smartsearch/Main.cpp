#include "ivs/algorithm/eziv_smartsearch.h"

#pragma comment(lib, "eziv_smartsearch.lib")
#pragma comment(lib, "ezframework.lib")

//���񱱨��ܼ�
bool stop=false;
bool fram=false;
bool GoOn=false;

int main(int argc, char* argv[])
{
	/************************************/
	/*		0. ����ShowImage��flag		*/
	/*		   "1"ΪON, "0"ΪOFF		*/
	/************************************/
	BOOL bEnhanceImg	= TRUE;
	CvCapture*	pCapture= 0;				//ȡ��video
	IplImage*	pFrame	= 0;				//��video��ȡ��ͼƬ

	int i,total	= 1,pause;
	int wait	= 50;													          // cvWaitKey��
	//ߢȡ�������Ѷ	
	int ObjectNum=0;									//���������� 
	int Out=0,In=0;										
	BOOL32 FaceDetect;

	const char* video_name = "D:\\ROSOO\\rslab\\a0-easyivs\\clips\\7.FaceDetect_TestVideo.avi";
	if (argc == 2)
		video_name = argv[1];

	/****************************/
	/*		2. ��ȡVideo		*/
	/****************************/
	pCapture = cvCaptureFromAVI(video_name);
	if(!pCapture)
		pCapture = cvCaptureFromCAM(0); 

	/****************************/
	/*		3. ��������			*/
	/****************************/
	if(cvGrabFrame(pCapture))//��ȡһ��frame���趨
		pFrame = cvRetrieveFrame(pCapture);
	else
	{
		printf("No frame in\n");//û��frame ����
		exit(0);//������ʽ  �ɸĳ�return 
	}

	//CIntelliSearch Search(frame->width,frame->height, 0,0,frame->width,frame->height,false);
	//Search(frame��,frame��,ROI��ʼ��X,ROI��ʼ��Y,ROI��,ROI��,
	//ֻ��Ѱ�����г��ֶ���(true))
	CIntelliSearch Search;

	TEZIVINFO_INTELLISEARCH tConfig = {0};
	strcpy(tConfig.m_achConfPath, "D:\\ROSOO\\rslab\\a0-easyivs\\conf\\smartsearch");
	tConfig.m_nMaxObjectInFrame	= 5;
	tConfig.m_emSearchMode		= EM_EZIV_INTELLISEARCH_MODE_DEFAULT;
	tConfig.m_nFrameWidth		= pFrame->width;
	tConfig.m_nFrameHeight		= pFrame->height;

	//*************************�趨 ROI����****************************************************************
	tConfig.m_tROI_ltpt.x=0;					//ROI���µ�x
	tConfig.m_tROI_ltpt.y=0;					//ROI���µ�y
    tConfig.m_tROI_rbpt.x=pFrame->width;				//ROI���ϵ�x
	tConfig.m_tROI_rbpt.y=pFrame->height;			//ROI���ϵ�y
	//�趨ROI����λ��
	tConfig.m_tROIRect.x=0;					//ROI���µ�x����
	tConfig.m_tROIRect.y=0;					//ROI���µ�y����
	tConfig.m_tROIRect.width=pFrame->width;			//ROI��		
	tConfig.m_tROIRect.height=pFrame->height;			//ROI��

	//*********************���ߵ�������****************************************
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
		// mode:0 Ϊ���������� mode:1 gama���� mode:2 hist���� mode:3 gama+hist����
		int Parameter_one_InitValue=6,Parameter_one_StopValue=10;
		// mode:0 ������Ч mode:1 ����= ���߱�׼ֵ�趨 mode:2 ����= histogram �����趨 mode:3 ����= ���߱�׼ֵ�趨
		int Parameter_two_InitValue=0,Parameter_two_StopValue=10;
		// mode:0 ������Ч mode:1 ����= gamaֵ�趨 mode:2 ����= histogram �����趨 mode:3 ����= gamaֵ�趨
		int Parameter_three_InitValue=0,Parameter_three_StopValue=10;
		// mode:0 ������Ч mode:1 2 3 ����=�����ѯ����Χ

		cvCreateTrackbar("Mode","result",&Mode_InitValue,Mode_StopValue,NULL);
		cvCreateTrackbar("Parameter 1","result",&Parameter_one_InitValue,Parameter_one_StopValue,NULL);
		cvCreateTrackbar("Parameter 2","result",&Parameter_two_InitValue,Parameter_two_StopValue,NULL);
	}	
#endif

	/****************************/
	/*		4. ��Ҫ�����Ȧ		*/
	/****************************/
	// ��ȡcapture 
	//-----
	if(pCapture)
	{
		for(;;)
		{
			//��capture�����ȡframe
			if( !cvGrabFrame( pCapture))
			{
				break;    // no load frame ,then break
			}
			pFrame = cvRetrieveFrame( pCapture );

#ifdef _ENABLE_ENHANCEMENT_
			//���ߵ���function
			if(bEnhanceImg)//ʹ��
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
			else//��ʹ�û�����ʹ��Ԥ��Ĺ���ģʽ�����
			{
				Search.SetAlignmentMode(EM_EZIV_IMAGE_ENHANCE_MODE_NONE, tConfig.m_tGamaTrans, tConfig.m_tHistEqualize);  //����У������ʹ�� 
			}
#endif
			//�������
			Search.Detect(pFrame);

			if( Search.Alarm())
			{
				//�����뿪
				ObjectNum=Search.ReturnObjectNum();
				cout<<"out object num:"<<ObjectNum<<'\n';
				for(i=0;i<ObjectNum;i++)
				{
					//�������
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
			/*		6. ��������: �ӳ� & ����ִ��ʱ��	*/
			/********************************************/
			if((pause = cvWaitKey(wait)) >= 0)
			{
				if(pause == 'p' || pause == 'P')	cvWaitKey(0);	// ��ͣ 
				else	break;										// ��ֹ��ʽ
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
      