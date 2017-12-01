#include <stdio.h>
#include <tchar.h>

#include "ivs/algorithm/eziv_lostdetect.h"

#ifdef _MSC_VER
#pragma comment(lib, "eziv_lostdetect.lib")
#pragma comment(lib, "ezframework.lib")
#endif

/*
 *
 *
 * 
 * 
 *
 *
 */
int select_object = 0;
int select_over = 0;  // 判斷框框畫好沒 20100827 added by tsao 拿來當滑鼠反應用的
int track_object = 0;
CvPoint origin;
CvRect selection;
CvRect track_window;
IplImage *Frame=NULL, *CopyFrame=NULL;

void on_mouse( int event, int x, int y, int flags, void* param );

int main(int args, char* argv[])
{
	selection.x = 65;
	selection.y = 88;
	selection.width = 94;
	selection.height = 73;

	CvCapture* capture = 0;
	int i=1;

    CvPoint pt1;
    CvFont font;
    CvScalar color={0,0,255};
	cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX , 1 , 1 );
    pt1.x = 10;
	pt1.y = 20 ;

	cvNamedWindow( "result", 1 );
	cvSetMouseCallback( "result", on_mouse, 0 );

	const char* filename = "liblostdetect_sample.avi";
	if (args == 2)
		filename = argv[1];

	capture = cvCaptureFromFile(filename);
	if( capture == NULL )
		capture = cvCaptureFromCAM(0);

	while (1)
	{
		Frame = cvQueryFrame( capture );
		if (Frame != NULL)
			break;
	}

	CLostDetect detect1;
	TEZIVINFO_LOSTDETECT tConfig;
	tConfig.m_nFrameWidth		= Frame->width;
	tConfig.m_nFrameHeight		= Frame->height;
	tConfig.m_afHistRangesArr[0]= 0;
	tConfig.m_afHistRangesArr[1]= 180;
	tConfig.m_nAlarmTimer		= 3;
	tConfig.m_nHistDimSize		= 16;
	detect1.Init(tConfig);

	//2009 8 13 upgrade
	CopyFrame =cvCreateImage( cvSize(Frame->width, Frame->height), Frame->depth, Frame->nChannels );
	//2009 8 13 upgrade

	detect1.SetROI(Frame, selection.x, selection.y, selection.width, selection.height);

	select_over = 1;
	int stat = 0;
    TEZIV_DETECT_RESULT tResult;

	for( ;Frame; Frame = cvQueryFrame(capture) )
	{
		//2009 8 13 upgrade
		memcpy (CopyFrame->imageData, Frame->imageData, CopyFrame->imageSize);
		CopyFrame->origin=Frame->origin;

        detect1.InputImage(Frame);

		//2009 8 13 upgrade
		if (stat == 0)
		{
			detect1.DetectAndDraw(tResult);
			if( detect1.Alarm() )
			{
				if(select_over == 1)
					cvPutText( CopyFrame, "Warning!!!" , pt1, &font, color);
			}
		}
		if( track_object < 0 )
        {
            track_window = selection;
			detect1.SetROI(Frame, selection.x, selection.y, selection.width, selection.height);
			track_object = 1;
        }

		cvRectangle(CopyFrame, cvPoint(selection.x, selection.y), cvPoint(selection.x+selection.width, selection.y+selection.height), cvScalar(255));
		
		char key = cvWaitKey(33);

		if (key == 'r' )
		{
			detect1.SetROI(Frame, 100, 100, 100, 100);
			printf("ObjectUpdated\n");
		}
		else if(key == 'p')
		{
			cvWaitKey(0);
		}
		else if(key == 't')
		{
			break;
		}

		cvShowImage( "result", CopyFrame );
	}

	cvReleaseCapture( &capture );
    cvDestroyWindow("result");

	return 0;

}

void on_mouse( int event, int x, int y, int flags, void* param )
{
    if( !Frame )
        return;

    if( Frame->origin )
        y = Frame->height - y;

    if( select_object )
    {
        selection.x = MIN(x,origin.x);
        selection.y = MIN(y,origin.y);
        selection.width = selection.x + CV_IABS(x - origin.x);
        selection.height = selection.y + CV_IABS(y - origin.y);
        
        selection.x = MAX( selection.x, 0 );
        selection.y = MAX( selection.y, 0 );
        selection.width = MIN( selection.width, Frame->width );
        selection.height = MIN( selection.height, Frame->height );
        selection.width -= selection.x;
        selection.height -= selection.y;
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = cvPoint(x,y);
        selection = cvRect(x,y,0,0);
        select_object = 1;
        break;
    case CV_EVENT_LBUTTONUP:
		if(select_object == 1)
			select_over = 1;
        select_object = 0;
        if( selection.width > 0 && selection.height > 0 )
            track_object = -1;
        break;
    }


}