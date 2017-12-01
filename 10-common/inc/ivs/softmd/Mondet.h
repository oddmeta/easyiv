#ifndef __MONDET__H
#define __MONDET__H
//#include"ioBMP.h"
#include"IMCollection.h"
#include<iostream>

/*****
百花齊放蘭花嬌

蘭嬌   蘭嬌    蘭嬌香

風吹草動草枝擺

枝擺   枝擺    草枝擺

人說詩好人叫好

人叫   人叫    人叫好
****/

class Mondet{
	friend std::ostream& operator<<(std::ostream& ooo, const Mondet& md);
public:
	IMCollection blocks;
	unsigned char*imageSave;
	int*imageMask;
private:
	bool isFirstPic;
	bool isSwap;
//	int*imageArray1;
//	int*imageArray2;

	int eDotSize;
	IMCollection changedBlks;
	Mondet() { }
	Mondet(const Mondet&) { }
	bool dectBlock() ;
	int defaultSens;
	void getImageData( int * useImageArray, unsigned char * imageArray );

	void doing(void);
	bool testing(int inx, int iny, int lx, int ly, int msens);

public:
	int*imageArray1;
	int*imageArray2;
	int w, h;
	Mondet(int w, int h, int sens)
		: w(w), h(h), isFirstPic(true), imageArray1(0), imageArray2(0), 
		  blocks(sens), defaultSens(sens) 
	{  
		imageArray1 = new int[w*h*3];
		imageArray2 = new int[w*h*3];
		imageMask   = new int[w*h];
		isSwap= false;
		eDotSize= 0 ;
	}
	bool setPic(unsigned char* imageArray);
	void setEDot(int inn){ eDotSize = inn ;}
    void autoSplit(int m, int n, int sens);  //: auto split m*n block, and add to list.
	inline void autoSplit(int m, int n) { autoSplit(m, n, defaultSens); }
	inline void setNeedDect(int i, bool needDect){ blocks.setNeedDect(i, needDect); }

	void addBlock(int x1, int y1, int x2, int y2, int sens);

	void setReset(void){ isFirstPic = true ; isSwap= false; }
	IMCollection getChangedBlocks() const ;
	~Mondet() { 
		delete[] imageArray1;
		delete[] imageArray2;
		delete[] imageMask;
	}
/*
	
	void callSave(void)
	{

		imageSave= new unsigned char[w*h*3];
	
		int k,l;

		for(k=0;k<w*h;k++)
		{
			l= k*3;

			if ( imageMask[k]== 254) 
			{
				imageSave[l] =0;
				imageSave[l+1]=0; 
				imageSave[l+2] =255;
			}
			else
			{
				imageSave[l]  = (unsigned char)imageMask[k];
				imageSave[l+1]= (unsigned char)imageMask[k];
				imageSave[l+2]= (unsigned char)imageMask[k];
			}
		
		}

		
		BMPFile sFile;
		
		sFile.SetPic(imageSave,  w, h);
		sFile.SaveFile("my.bmp");
	}
*/	


};

#endif