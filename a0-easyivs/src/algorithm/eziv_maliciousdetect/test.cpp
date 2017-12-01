#include "../../common/unissdefine.h"
#include "MaliciousDetect.h"

#include <stdio.h>

int main(int args, char* argv[])
{
	CvCapture* capture = 0;													//设定camera
	IplImage *Frame=NULL,*tempFrame;										//image
	CvPoint pt1;
	CvFont font;															
	CvScalar Sum;
	double I;
	CvScalar color={0,0,255};
	//cvInitFont( &font, 10 , 1 , 1 , 0.0 , 0 , CV_AA );						//设显示字型
	cvInitFont (&font, CV_FONT_HERSHEY_SIMPLEX , 1.0, 1.0, 0, 1, CV_AA);
	pt1.x = 10;																//显示位置
	pt1.y = 20;
 
	cvNamedWindow( "result", 1 );

	const char* filename = "libmaliciousdetect_sample.avi";
	if (args == 2)
		filename = argv[1];

	capture = cvCaptureFromFile(filename);
	if( capture == NULL )
	{
		//capture = cvCaptureFromFile("MaliciousDetect.avi");                 //手遮镜头影片(侦测成功)
		//capture = cvCaptureFromFile("前景变化.AVI");                        //焦距变化影片(侦测失败)
		//capture = cvCaptureFromFile("擦拭.AVI");                            //水气脏污覆盖影片(部分侦测成功)
		//capture = cvCaptureFromFile("00106.AVI");                           //滴1、2滴清水影片档(侦测失败)
		//capture = cvCaptureFromFile("00107.AVI");                           //喷上清水影片档(侦测失败)
		//capture = cvCaptureFromFile("I:\RG4.NET\EasyIV\clips\1.MaliciousDetect.avi");//雨伞遮镜头影片档(大范围遮挡可侦测成功)
		//capture = cvCaptureFromFile("00110.AVI");                           //拍打镜头1影片档(侦测成功)
		//capture = cvCaptureFromFile("00112.AVI");                           //拍打镜头2影片档(侦测成功)
		//capture = cvCaptureFromFile("00113.AVI");                           //慢速移动镜头影片档(侦测失败)
		capture = cvCaptureFromCAM(0);
	}
	while (1)
	{
		Frame = cvQueryFrame( capture );
		tempFrame=cvCreateImage( cvSize(Frame->width, Frame->height), 8, 1 );	//暂存
		cvCvtColor( Frame, tempFrame, CV_BGR2GRAY );							//color to gray
		Sum=cvSum(tempFrame);													//计算tempFrame的元素
		I=Sum.val[0]/(Frame->width*Frame->height);								//计算与原影像之比例
		if ((Frame != NULL)&&(I>40)&&(I<230))	break;
	}
	cvReleaseImage(&tempFrame);

	CMaliciousDetect detect1(Frame->width, Frame->height, 25);				//单一镜头

	int viewPoint = 0;


	detect1.SetImage(Frame);
	for(int stat = 0 ; ; Frame = cvQueryFrame(capture) )  //用state来避开初始frame的warning
	{
		if (stat == 1)
		{
			detect1.SetImage(Frame);
			if( detect1.Alarm() )
			{
				cvPutText( Frame, "Warning!!!" , pt1, &font, color);
			}
		}

		char key = cvWaitKey(25);

		if( key == 27 )		break;

		cvShowImage( "result", Frame );
		stat=1;
	}

	cvReleaseCapture( &capture );
    cvDestroyWindow("result");

	return 0;
}