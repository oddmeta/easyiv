
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FACEFEATUREDETECT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EASYIV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#include <math.h>
#include <stdio.h>

#include "ivs/eziv_commondef.h"
#include "ivs/eziv_opencvdef.h"

#define prob(x,rho) exp(-((x)*(x))/2/(rho)/(rho))/(rho)

// This class is exported from the FaceFeatureDetect.dll
class EASYIV_API CFacefeatureDect    
{
public:
	CFacefeatureDect();
	virtual ~CFacefeatureDect();

public:
	BOOL32		Init(TEZIVINFO_FACEFEATURE &tFaceParam);
	IplImage*	GetImage(EM_EZIV_IPLIMGTYPE emIplImageType);

private:
	TEZIVINFO_FACEFEATURE	m_tFaceParam;

	int exactFace();
	//SetImage()初始图形记忆体
	void SetImage(IplImage *img);
	//gray为输入图形之灰阶图
	//inputImg为记录输入图形指标
	//eqgray为记录gray图形直方图等化後的结果
	//small_img为记录gray图形缩小後之结果
	//faceImg记录脸部图形
	//eqfaceImg记录脸部图形直方图等化之结果
	IplImage *gray,*inputImg,*eqgray,*small_img,*faceImg,*eqfaceImg;
	//BGRface为记录输出之结果
	IplImage *BGRface;
	//动态图形记忆体之指标
	u8 *eqfaceB,*faceImgB,*bgrB;
	//facetotal纪录脸之总数，flag记录是否有人脸存在
	int facetotal,flag;
	//鼻部的方框指标纪录
	int nob,not,nol,nor;
	//嘴部的方框指标纪录
	int mob,mot,mol,mor;
	//右眼部的方框指标纪录
	int reb,ret,rel,rer;
	//左眼部的方框指标纪录
	int leb,let,lel,ler;
	int lbound,rbound,hRec;
	//scale为缩放比
	double scale;
	//可能的脸部、左眼、右眼、嘴部、鼻部的记忆变数
	CvSeq *faces,*leye,*reye,*mouth,*nose;
	//wh记录图形长宽规格
	CvSize wh;
	//脸谱可靠分数
	double confidence;

private:
	CvMemStorage *storage,*storager,*storagel,*storagen,*storagem;
	CvHaarClassifierCascade *cascade,*cascader,*cascadel,*cascaden,*cascadem;
	BOOL32 isSetImg;
};

//#endif

