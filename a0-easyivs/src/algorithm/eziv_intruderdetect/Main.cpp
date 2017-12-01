#include "ivs/algorithm/eziv_intruderdetect.h"

#pragma comment(lib, "ezframework.lib")

float scale=1;
int threshold=1;
int count_mouse=0;    //�㻬��Ĵ���
int temp;

//��ֵ
int test=7,test2=12;
int g_nMousePointX0,g_nMousePointY0,g_nMousePointX1,g_nMousePointY1,p_x2,p_y2,p_x3,p_y3;  // ������Χǽ4����Ѷ

CvRect g_tROI;    //set ROI

IplImage*    im1                ; 
IplImage*    im2                ;
IplImage*    im3                ;

void on_mouse( int event, int x, int y, int flags, void* param )
{
	if( event == CV_EVENT_LBUTTONDOWN )
	{
		if(count_mouse==0)
		{
			g_nMousePointX0=x;
			g_nMousePointY0=y;
		}
		if(count_mouse==1)
		{
			g_nMousePointX1=x;
			g_nMousePointY1=y;
		}
		count_mouse++;
	}
}

//-------------------------------------------------------------------------
void on_change(int h)
{
	if(count_mouse>1)
	{
		scale=(float)(test*0.1+1);    
		if(((20*scale>g_tROI.width)||(60*scale>g_tROI.height))&&(scale!=1))
		{
			if((g_tROI.width/20-1)>(g_tROI.height/60-1))
			{    
				scale=(float)((g_tROI.height-1)/60);test=(int)((scale-1)*10);
			}
			else 
			{    
				scale=(float)((g_tROI.width-1)/20);test=(int)((scale-1)*10);
			}
			printf("error scale\n");
		}
		// UniSVR:  �����error scale, ��������Щtest & scale��������������? �ǲ���Ӧ��Ҫ����ȷ�Ĳſ���?
		// NTHU:Ҫ��ȷ�ſ���
	}
}
//-----------------------------------------------------------------------
void change(int h)
{
	threshold=test2*5+1;
}

//***********************************************************************
// ʹ���߲�����
// 1.���Ӵ���ѡʵ��Χǽ���������ǣ�˳������ң�
// 2.����Threshold�Ĵ�С�����������FG��Ѷ̫�࣬��ʱ�������ŵ���Threshold
// 3.����pattern�Ĵ�С����������ɫ�������������
// 
// ӰƬ����Ԥ�裺
// 1.ʵ��Χǽ��ǽ������Ҫ�������˼������
// 2.��Ӱ��������ǶȺ�Χǽ�ķ������нǲ��ܳ���45������
// 3.��Ӱ������ĸ߶�Լ2.5������3.5��������
// 4.����ʵ��Χǽ�ָ���������ڻ���1/2������
//***********************************************************************
int main(int args, char* argv[])
{
	CvCapture*   cap                = NULL;
	IplImage*    pFrame          = NULL;

	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 1 , 1);
	int IntoOutFlag=0;		// ֻ�г�ȥ��alarm
	int OuttoInFlag=0;		// ֻ�н�����alarm
	int AllFlag=1;			// ��������alarm

	cvNamedWindow("Video", 1);                    //�����Ӵ�
	cvNamedWindow("FG", 1);  
	cvNamedWindow("TRACE", 1);

	const char* filename = "D:\\ROSOO\\rslab\\a0-easyivs\\clips\\6.IntruderDetect.avi";//"libintruderdetect_sample.avi";
	if (args == 2)
		filename = argv[1];

	cap = cvCaptureFromFile(filename);
	//cap = cvCaptureFromFile("IntruderDetect.avi");
	//cap = cvCaptureFromFile("IntruderDetect2.avi");
	//cap = cvCaptureFromFile("00100.avi");
	//cap = cvCaptureFromFile("00102.avi");			
	//p_x0=92;p_y0=192;p_x1=292;p_y1=130;
	//p_x0=150;p_y0=220;p_x1=200;p_y1=60;
	//p_x0=150;p_y0=60;p_x1=200;p_y1=220;
	//p_x0=100;p_y0=110;p_x1=240;p_y1=110;      //00102ˮƽ��
	g_nMousePointX0=160;g_nMousePointY0=30;g_nMousePointX1=160;g_nMousePointY1=200;     //00100,00101��ֱ��
	//p_x0=10;p_y0=30;p_x1=10;p_y1=200;     //00100,00101��ֱ��

	//cap = cvCaptureFromFile("00164(NEW).avi");		p_x0=180;p_y0=110;p_x1=340;p_y1=95;
	//cap = cvCaptureFromFile("00165(NEW).avi");		p_x0=80;p_y0=117;p_x1=345;p_y1=117;
	//cap = cvCaptureFromFile("01(NEW).avi");		    p_x0=30;p_y0=120;p_x1=120;p_y1=135;
	//cap = cvCaptureFromFile("01-cut(NEW).avi");		p_x0=30;p_y0=120;p_x1=120;p_y1=135;

	if(cap ==NULL)				// ��������ӰƬ
	{
		cap = cvCaptureFromCAM(0);  // �Ӽ���������ӰƬ

		pFrame = cvQueryFrame(cap);

		//rect.x= p_x0;rect.y= 0;rect.width= p_x1-p_x0;rect.height= tmp_frame->height;
		{
			if(g_nMousePointX0 > g_nMousePointX1)
			{
				int tmp = g_nMousePointX0; g_nMousePointX0 = g_nMousePointX1; g_nMousePointX1 = tmp;
				tmp = g_nMousePointY0; g_nMousePointY0 = g_nMousePointY1; g_nMousePointY1 = tmp;
			}

			int xx = (g_nMousePointX0-g_nMousePointX1>=0 ? g_nMousePointX0-g_nMousePointX1 : g_nMousePointX1-g_nMousePointX0);
			int yy = (g_nMousePointY0-g_nMousePointY1>=0 ? g_nMousePointY0-g_nMousePointY1 : g_nMousePointY1-g_nMousePointY0);
			if(yy >= xx)
			{
				g_tROI.x = (g_nMousePointX0+g_nMousePointX1)/2 - yy/2;
				g_tROI.width = yy;
				if(g_tROI.x < 0)
					g_tROI.x = 0;
				if(g_tROI.x+g_tROI.width > pFrame->width)
					g_tROI.width = pFrame->width - g_tROI.x;
			}
			else
			{
				g_tROI.x = g_nMousePointX0;
				g_tROI.width = xx;
			}

			g_tROI.y = 0;
			g_tROI.height = pFrame->height;
		}	/////////////////////////////////////////////////////////
		count_mouse=2;
	}

	if (!cap)                                  //Ӱ��ߢȡʧ��
	{
		printf("------");
		exit(0);
	} 

	pFrame = cvQueryFrame(cap);             //ͼƬ��ȡ

	if(!pFrame)                             //ͼƬ��ȡʧ��
	{
		printf("bad video \n");
		exit(0);
	}


	cvSetMouseCallback( "Video", on_mouse );
	cvCreateTrackbar( "pattern", "Video",&test,30 ,on_change);
	on_change(0);
	cvCreateTrackbar( "threshold", "Video",&test2,40 ,change);
	change(0);

	IntruderDetect detect1(pFrame->width, pFrame->height, pFrame->nChannels, pFrame->widthStep);

	TEZIVINFO_INTRUDER tConfig = {0};
	strcpy(tConfig.m_achConfPath, "D:\\ROSOO\\rslab\\a0-easyivs\\conf\\intruder");
	tConfig.m_nFrameWidth		= pFrame->width;
	tConfig.m_nFrameHeight		= pFrame->height;
	tConfig.m_tROI.x			= 229;
	tConfig.m_tROI.y			= 0;
	tConfig.m_tROI.width		= 56;
	tConfig.m_tROI.height		= 288;
	tConfig.m_nThreshold		= 60;
	tConfig.m_fPatternScale		= 1.0;

	detect1.Init(tConfig);

	int inNumber,outNumber;

	//------------------------��ʼload frame
	for( int fr = 1;pFrame; pFrame = cvQueryFrame(cap), fr++ ) 
	{
		char pause;
		if((pause = cvWaitKey(1)) >= 0)
		{
			if(pause == 'p' || pause == 'P') cvWaitKey(0);	// ��ͣ
			else break;										// ��ֹ��ʽ
		}

		if(count_mouse>=2)
		{
			//rect.x= p_x0;rect.y= 0;rect.width= p_x1-p_x0;rect.height= tmp_frame->height;
			{
				//////////////////////////////////////////////////////////
				//Jacky: if X coordinate of the first point bigger than the
				//	second one, we'll exchange them to make sure the line 
				//	direction if what we allowed
				if(g_nMousePointX0 > g_nMousePointX1)
				{
					int tmp = g_nMousePointX0;
					g_nMousePointX0 = g_nMousePointX1;
					g_nMousePointX1 = tmp;

					tmp = g_nMousePointY0;
					g_nMousePointY0 = g_nMousePointY1;
					g_nMousePointY1 = tmp;
				}

				//////////////////////////////////////////////////////////////////////////
				//Jacky: Calculate the center point
				int xx = (g_nMousePointX0-g_nMousePointX1>=0 ? g_nMousePointX0-g_nMousePointX1 : g_nMousePointX1-g_nMousePointX0);
				int yy = (g_nMousePointY0-g_nMousePointY1>=0 ? g_nMousePointY0-g_nMousePointY1 : g_nMousePointY1-g_nMousePointY0);

				//////////////////////////////////////////////////////////////////////////
				//Jacky: Calculate the ROI rect
				if(yy >= xx)
				{
					g_tROI.x = (g_nMousePointX0+g_nMousePointX1)/2 - yy/2;
					g_tROI.width = yy;
					if(g_tROI.x < 0)
						g_tROI.x = 0;
					if(g_tROI.x+g_tROI.width > pFrame->width)
						g_tROI.width = pFrame->width - g_tROI.x;
				}
				else
				{
					g_tROI.x = g_nMousePointX0;
					g_tROI.width = xx;
				}

				g_tROI.y = 0;
				g_tROI.height = pFrame->height;
			}	/////////////////////////////////////////////////////////

			detect1.SetThreshold(scale,threshold);
			detect1.SetRoi(g_tROI);
			detect1.SetRoad(g_nMousePointX0, g_nMousePointY0, g_nMousePointX1, g_nMousePointY1);
		}

		detect1.loadframe(pFrame);	// ��ʼ��ͼ
		if(detect1.m_bInitializing)
			printf("Initializing...\n");
		else
			printf("Detecting...\n");

		im1=detect1.GetImage(1);	//��ȡ��ǰframe
		im2=detect1.GetImage(2);	//��ȡǰ��
		im3=detect1.GetImage(3);	//��ȡ�켣

		inNumber=detect1.getInOut(EM_EZIV_INTRUDER_IN);		//��ȡ��������a
		outNumber=detect1.getInOut(EM_EZIV_INTRUDER_OUT);		//��ȡ��ȥ����b

		char nPplCount_in[20];
		sprintf(nPplCount_in,"Warning!!!: %1d",inNumber);    //�м�����
		//sprintf(nPplCount_in,"Warning!!!");                    //û������
		char nPplCount_out[20];
		sprintf(nPplCount_out,"Warning!!!: %1d",outNumber);  //�м�����
		//sprintf(nPplCount_out,"Warning!!!");                   //û������


		/*
		if(OuttoInFlag==1 && inNumber>0)
		cvPutText( im1, nPplCount_in , cvPoint(20,220), &font, cvScalar(0,0,255));
		if(IntoOutFlag==1 && outNumber>0)
		cvPutText( im1, nPplCount_out , cvPoint(20,100), &font, cvScalar(0,0,255));
		*/


		if(AllFlag==1 && inNumber>0)
			cvPutText( im1, nPplCount_in , cvPoint(20,220), &font, cvScalar(0,0,255));
		if(AllFlag==1 && outNumber>0)
			cvPutText( im1, nPplCount_out , cvPoint(20,100), &font, cvScalar(0,255,0));



		//AlarmFun��ش�����ֵ��0��ʾ���¼���1��ʾ�����������ֵ��ڲ࣬2��ʾ������ڲൽ���
		int alarm = detect1.AlarmFun();
		if(alarm==1)
		{
			printf("1\n");
		}
		else if(alarm==2)
		{
			printf("2\n");
		}

		cvRectangle( im3, cvPoint(g_tROI.x, g_tROI.y), cvPoint(g_tROI.x+g_tROI.width, g_tROI.y+g_tROI.height), cvScalar(255));	// ��ʾROI
		cvRectangle( im1, cvPoint(g_tROI.x, g_tROI.y), cvPoint(g_tROI.x+g_tROI.width, g_tROI.y+g_tROI.height), cvScalar(255));	// ӰƬ��ʾROI

		cvShowImage("Video",im1)    ;	// show Image
		cvShowImage("FG",im2)	;		
		cvShowImage("TRACE",im3) ;


	}

	detect1.resetInOut();	       //��������
	cvReleaseCapture(&cap);	       
	cvDestroyAllWindows();         //�ر������Ӵ�

	return 0;
}

