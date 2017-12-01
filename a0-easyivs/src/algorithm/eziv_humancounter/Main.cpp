#include "HumanCounter.h"
        
       
float scale=1;
int threshold=1;
int count_mouse=0;    //點滑鼠的次數
int test=7,test2=16,temp; //初值
int width;
int height;

int point_x1,point_y1,point_x2,point_y2;//---------------------
CvRect setROI;  ////set inner ROI     
CvRect rect;    //set outer ROI---------------------------------
IplImage*    im1                ; 
IplImage*    im2                ;
IplImage*    im3                ;   



void on_mouse( int event, int x, int y, int flags, void* param )
{

	if( event == CV_EVENT_LBUTTONDOWN )
    {
		if(count_mouse==0)
		{
			setROI.x=x;
			setROI.y=y;
		   
		}
        if(count_mouse==1)
		{
			setROI.width=abs(x-setROI.x);
			setROI.height=abs(y-setROI.y);
		   
		    if(x<setROI.x)
			{
				temp=x;
				x=setROI.x;
				setROI.x=temp;
			}
			if(y<setROI.y)
			{
				temp=y;
				y=setROI.y;
				setROI.y=temp;
			}           
// UniSVR:  ROI的大小是只要在(w,h)>=(30,70)以上就可以, 還是與原始圖檔的寬度與高度有等比例的關係?
//			如(w, h)為 (320,240)可以用(30,70), 但是如果是(640, 480)的話, 也是(30,70)以上就可以, 
//			還是必須是(60, 140) or (50, 120)才可以?

// NTHU:跟原始圖檔的寬度與高度無關，這裡只是想要避免ROI區比原始的pattern小的case
		    if(( setROI.width<30)||(setROI.height<70))
			count_mouse=-1;
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
	}

}
//-----------------------------------------------------------------------
void change(int h)
{
    
	threshold=test2*5+1;
	
}


//***********************************************************************
// 使用者操作：
// 1.要在視窗上點兩下產生ROI
// 2.調整Threshold的大小：如果產生的FG雜訊太多，此時可以試著調高Threshold
// 3.調整pattern的大小：a.盡量讓藍色框包含整個人形 
//                      b.藍色框的長度不能超過ROI長的4/5
//***********************************************************************
int main(int args, char* argv[])
{
	CvCapture*   cap                = NULL;
	IplImage*    tmp_frame          = NULL;
	int ShowFlag_FG=1;           // Show FG (1 mean Yes ; 0 mean NO)
	int ShowFlag_TRACE=1;        // Show TRACE (1 mean Yes ; 0 mean NO)

	const char* filename = "libhumancounter_sample.avi";
	if (args == 2)
		filename = argv[1];

	////for test  	     
	//// inner ROI*************************************
	setROI.x= 230;setROI.y= 30;setROI.width= 60;setROI.height= 90;
	count_mouse=2;
	cap = cvCaptureFromFile(filename);

	if(cap ==NULL){
		cap = cvCaptureFromCAM(0);
	}

	if (!cap)                                  //影像擷取失敗
	{
		printf("------");
		exit(0);
	} 
	 
	tmp_frame = cvQueryFrame(cap);             //圖片讀取
	width=tmp_frame->width;
	height=tmp_frame->height;
    
	if(!tmp_frame)                             //圖片讀取失敗
	{
		printf("bad video \n");
        exit(0);
	}   
   
    cvNamedWindow("BG", 1);                    //給出視窗
	if(ShowFlag_FG==1)
	{
		cvNamedWindow("FG", 1);
	}
	if(ShowFlag_TRACE==1)
	{
		cvNamedWindow("TRACE", 1);
	}




    cvSetMouseCallback( "BG", on_mouse );    
	cvCreateTrackbar( "pattern", "BG",&test,30 ,on_change);
	on_change(0);
    cvCreateTrackbar( "threshold", "BG",&test2,40 ,change);
	change(0);

    CHumanCounter detect1(tmp_frame->width, tmp_frame->height, tmp_frame->nChannels,
		            tmp_frame->widthStep,0);   //圖片資料 0 find_circle off   

	
//	detect1.loadtempl();                     //讀取樣本

    int inNumber,outNumber;


//------------------------開始load frame
	
    for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), fr++ ) 
    {
		if(fr%1==0)                //多少張作一次計算
		{        
			double t = (double)cvGetTickCount();
			char k = cvWaitKey(30);
			if( k == 27 ) break;		// 按ESC鍵結束

			if(count_mouse>=2)
			{
				detect1.SetThreshold(scale,threshold);
				detect1.SetRoi(setROI.x,setROI.y,setROI.width,setROI.height,tmp_frame);

			}

		   
			detect1.loadframe(tmp_frame); //開始讀圖
			for(int gg=0;gg<10;gg++)
			{
				if(detect1.GetPoint(gg,1)!=0)
					for(int kk=1;kk<5;kk++)
					{
						printf("%d  ",detect1.GetPoint(gg,kk));  //可以不讀取
						if(kk==4)
							printf("\n");
					}
				else 
					break;
								
			}
			detect1.ResetPoint();           //但是要reset

			im1=detect1.GetImage(1);//讀取當前frame
			im2=detect1.GetImage(2);//讀取前景
			im3=detect1.GetImage(3);//讀取軌跡

			inNumber=detect1.getInOut(1);   //讀取進來人數
			outNumber=detect1.getInOut(2);  //讀取出去人數

			cvShowImage("BG",im1)    ;
			if(ShowFlag_FG==1)
			{
			cvShowImage("FG",im2)	;
			}
			if(ShowFlag_TRACE==1)
			{
			cvShowImage("TRACE",im3) ;
			}
			printf("In %4d Out %4d\n",inNumber,outNumber);


		}  

	}

	detect1.resetInOut();	           //人數歸零
    detect1.releaseimage();	
    cvReleaseCapture(&cap);	       
    cvDestroyAllWindows();         //關閉所有視窗
	
    return 0;
}

