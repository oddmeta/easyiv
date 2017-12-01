#include "ivs/algorithm/eziv_intruderdetect.h"

#pragma comment(lib, "ezframework.lib")

float scale=1;
int threshold=1;
int count_mouse=0;    //点滑鼠的次数
int temp;

//初值
int test=7,test2=12;
int g_nMousePointX0,g_nMousePointY0,g_nMousePointX1,g_nMousePointY1,p_x2,p_y2,p_x3,p_y3;  // 存虚拟围墙4点资讯

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
		// UniSVR:  如果是error scale, 那上面这些test & scale还可以拿来用吗? 是不是应该要是正确的才可以?
		// NTHU:要正确才可以
	}
}
//-----------------------------------------------------------------------
void change(int h)
{
	threshold=test2*5+1;
}

//***********************************************************************
// 使用者操作：
// 1.在视窗点选实体围墙的上面两角（顺序：左→右）
// 2.调整Threshold的大小：如果产生的FG杂讯太多，此时可以试着调高Threshold
// 3.调整pattern的大小：尽量让蓝色框包含整个人形
// 
// 影片场景预设：
// 1.实体围墙，墙高至少要到成年人肩膀以上
// 2.摄影机的拍摄角度和围墙的法向量夹角不能超过45度以上
// 3.摄影机架设的高度约2.5公尺至3.5公尺左右
// 4.调整实体围墙分隔线最好是在画面1/2处左右
//***********************************************************************
int main(int args, char* argv[])
{
	CvCapture*   cap                = NULL;
	IplImage*    pFrame          = NULL;

	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 1 , 1);
	int IntoOutFlag=0;		// 只有出去发alarm
	int OuttoInFlag=0;		// 只有进来发alarm
	int AllFlag=1;			// 进出都发alarm

	cvNamedWindow("Video", 1);                    //给出视窗
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
	//p_x0=100;p_y0=110;p_x1=240;p_y1=110;      //00102水平线
	g_nMousePointX0=160;g_nMousePointY0=30;g_nMousePointX1=160;g_nMousePointY1=200;     //00100,00101垂直线
	//p_x0=10;p_y0=30;p_x1=10;p_y1=200;     //00100,00101垂直线

	//cap = cvCaptureFromFile("00164(NEW).avi");		p_x0=180;p_y0=110;p_x1=340;p_y1=95;
	//cap = cvCaptureFromFile("00165(NEW).avi");		p_x0=80;p_y0=117;p_x1=345;p_y1=117;
	//cap = cvCaptureFromFile("01(NEW).avi");		    p_x0=30;p_y0=120;p_x1=120;p_y1=135;
	//cap = cvCaptureFromFile("01-cut(NEW).avi");		p_x0=30;p_y0=120;p_x1=120;p_y1=135;

	if(cap ==NULL)				// 读进测试影片
	{
		cap = cvCaptureFromCAM(0);  // 从监视器读进影片

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

	if (!cap)                                  //影像撷取失败
	{
		printf("------");
		exit(0);
	} 

	pFrame = cvQueryFrame(cap);             //图片读取

	if(!pFrame)                             //图片读取失败
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

	//------------------------开始load frame
	for( int fr = 1;pFrame; pFrame = cvQueryFrame(cap), fr++ ) 
	{
		char pause;
		if((pause = cvWaitKey(1)) >= 0)
		{
			if(pause == 'p' || pause == 'P') cvWaitKey(0);	// 暂停
			else break;										// 终止程式
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

		detect1.loadframe(pFrame);	// 开始读图
		if(detect1.m_bInitializing)
			printf("Initializing...\n");
		else
			printf("Detecting...\n");

		im1=detect1.GetImage(1);	//读取当前frame
		im2=detect1.GetImage(2);	//读取前景
		im3=detect1.GetImage(3);	//读取轨迹

		inNumber=detect1.getInOut(EM_EZIV_INTRUDER_IN);		//读取进来人数a
		outNumber=detect1.getInOut(EM_EZIV_INTRUDER_OUT);		//读取出去人数b

		char nPplCount_in[20];
		sprintf(nPplCount_in,"Warning!!!: %1d",inNumber);    //有记人数
		//sprintf(nPplCount_in,"Warning!!!");                    //没记人数
		char nPplCount_out[20];
		sprintf(nPplCount_out,"Warning!!!: %1d",outNumber);  //有记人数
		//sprintf(nPplCount_out,"Warning!!!");                   //没记人数


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



		//AlarmFun会回传三种值，0表示无事件，1表示物件从外侧入侵到内侧，2表示物件从内侧到外侧
		int alarm = detect1.AlarmFun();
		if(alarm==1)
		{
			printf("1\n");
		}
		else if(alarm==2)
		{
			printf("2\n");
		}

		cvRectangle( im3, cvPoint(g_tROI.x, g_tROI.y), cvPoint(g_tROI.x+g_tROI.width, g_tROI.y+g_tROI.height), cvScalar(255));	// 显示ROI
		cvRectangle( im1, cvPoint(g_tROI.x, g_tROI.y), cvPoint(g_tROI.x+g_tROI.width, g_tROI.y+g_tROI.height), cvScalar(255));	// 影片显示ROI

		cvShowImage("Video",im1)    ;	// show Image
		cvShowImage("FG",im2)	;		
		cvShowImage("TRACE",im3) ;


	}

	detect1.resetInOut();	       //人数归零
	cvReleaseCapture(&cap);	       
	cvDestroyAllWindows();         //关闭所有视窗

	return 0;
}

