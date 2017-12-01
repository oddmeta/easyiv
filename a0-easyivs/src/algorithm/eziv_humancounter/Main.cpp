#include "HumanCounter.h"
        
       
float scale=1;
int threshold=1;
int count_mouse=0;    //�I�ƹ�������
int test=7,test2=16,temp; //���
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
// UniSVR:  ROI���j�p�O�u�n�b(w,h)>=(30,70)�H�W�N�i�H, �٬O�P��l���ɪ��e�׻P���צ�����Ҫ����Y?
//			�p(w, h)�� (320,240)�i�H��(30,70), ���O�p�G�O(640, 480)����, �]�O(30,70)�H�W�N�i�H, 
//			�٬O�����O(60, 140) or (50, 120)�~�i�H?

// NTHU:���l���ɪ��e�׻P���׵L���A�o�̥u�O�Q�n�קKROI�Ϥ��l��pattern�p��case
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
// �ϥΪ̾ާ@�G
// 1.�n�b�����W�I��U����ROI
// 2.�վ�Threshold���j�p�G�p�G���ͪ�FG���T�Ӧh�A���ɥi�H�յ۽հ�Threshold
// 3.�վ�pattern���j�p�Ga.�ɶq���Ŧ�إ]�t��ӤH�� 
//                      b.�Ŧ�ت����פ���W�LROI����4/5
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

	if (!cap)                                  //�v���^������
	{
		printf("------");
		exit(0);
	} 
	 
	tmp_frame = cvQueryFrame(cap);             //�Ϥ�Ū��
	width=tmp_frame->width;
	height=tmp_frame->height;
    
	if(!tmp_frame)                             //�Ϥ�Ū������
	{
		printf("bad video \n");
        exit(0);
	}   
   
    cvNamedWindow("BG", 1);                    //���X����
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
		            tmp_frame->widthStep,0);   //�Ϥ���� 0 find_circle off   

	
//	detect1.loadtempl();                     //Ū���˥�

    int inNumber,outNumber;


//------------------------�}�lload frame
	
    for( int fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), fr++ ) 
    {
		if(fr%1==0)                //�h�ֱi�@�@���p��
		{        
			double t = (double)cvGetTickCount();
			char k = cvWaitKey(30);
			if( k == 27 ) break;		// ��ESC�䵲��

			if(count_mouse>=2)
			{
				detect1.SetThreshold(scale,threshold);
				detect1.SetRoi(setROI.x,setROI.y,setROI.width,setROI.height,tmp_frame);

			}

		   
			detect1.loadframe(tmp_frame); //�}�lŪ��
			for(int gg=0;gg<10;gg++)
			{
				if(detect1.GetPoint(gg,1)!=0)
					for(int kk=1;kk<5;kk++)
					{
						printf("%d  ",detect1.GetPoint(gg,kk));  //�i�H��Ū��
						if(kk==4)
							printf("\n");
					}
				else 
					break;
								
			}
			detect1.ResetPoint();           //���O�nreset

			im1=detect1.GetImage(1);//Ū����eframe
			im2=detect1.GetImage(2);//Ū���e��
			im3=detect1.GetImage(3);//Ū���y��

			inNumber=detect1.getInOut(1);   //Ū���i�ӤH��
			outNumber=detect1.getInOut(2);  //Ū���X�h�H��

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

	detect1.resetInOut();	           //�H���k�s
    detect1.releaseimage();	
    cvReleaseCapture(&cap);	       
    cvDestroyAllWindows();         //�����Ҧ�����
	
    return 0;
}

