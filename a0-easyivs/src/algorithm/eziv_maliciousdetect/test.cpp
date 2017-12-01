#include "../../common/unissdefine.h"
#include "MaliciousDetect.h"

#include <stdio.h>

int main(int args, char* argv[])
{
	CvCapture* capture = 0;													//�趨camera
	IplImage *Frame=NULL,*tempFrame;										//image
	CvPoint pt1;
	CvFont font;															
	CvScalar Sum;
	double I;
	CvScalar color={0,0,255};
	//cvInitFont( &font, 10 , 1 , 1 , 0.0 , 0 , CV_AA );						//����ʾ����
	cvInitFont (&font, CV_FONT_HERSHEY_SIMPLEX , 1.0, 1.0, 0, 1, CV_AA);
	pt1.x = 10;																//��ʾλ��
	pt1.y = 20;
 
	cvNamedWindow( "result", 1 );

	const char* filename = "libmaliciousdetect_sample.avi";
	if (args == 2)
		filename = argv[1];

	capture = cvCaptureFromFile(filename);
	if( capture == NULL )
	{
		//capture = cvCaptureFromFile("MaliciousDetect.avi");                 //���ھ�ͷӰƬ(���ɹ�)
		//capture = cvCaptureFromFile("ǰ���仯.AVI");                        //����仯ӰƬ(���ʧ��)
		//capture = cvCaptureFromFile("����.AVI");                            //ˮ�����۸���ӰƬ(�������ɹ�)
		//capture = cvCaptureFromFile("00106.AVI");                           //��1��2����ˮӰƬ��(���ʧ��)
		//capture = cvCaptureFromFile("00107.AVI");                           //������ˮӰƬ��(���ʧ��)
		//capture = cvCaptureFromFile("I:\RG4.NET\EasyIV\clips\1.MaliciousDetect.avi");//��ɡ�ھ�ͷӰƬ��(��Χ�ڵ������ɹ�)
		//capture = cvCaptureFromFile("00110.AVI");                           //�Ĵ�ͷ1ӰƬ��(���ɹ�)
		//capture = cvCaptureFromFile("00112.AVI");                           //�Ĵ�ͷ2ӰƬ��(���ɹ�)
		//capture = cvCaptureFromFile("00113.AVI");                           //�����ƶ���ͷӰƬ��(���ʧ��)
		capture = cvCaptureFromCAM(0);
	}
	while (1)
	{
		Frame = cvQueryFrame( capture );
		tempFrame=cvCreateImage( cvSize(Frame->width, Frame->height), 8, 1 );	//�ݴ�
		cvCvtColor( Frame, tempFrame, CV_BGR2GRAY );							//color to gray
		Sum=cvSum(tempFrame);													//����tempFrame��Ԫ��
		I=Sum.val[0]/(Frame->width*Frame->height);								//������ԭӰ��֮����
		if ((Frame != NULL)&&(I>40)&&(I<230))	break;
	}
	cvReleaseImage(&tempFrame);

	CMaliciousDetect detect1(Frame->width, Frame->height, 25);				//��һ��ͷ

	int viewPoint = 0;


	detect1.SetImage(Frame);
	for(int stat = 0 ; ; Frame = cvQueryFrame(capture) )  //��state���ܿ���ʼframe��warning
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