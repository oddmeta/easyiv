
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
	//SetImage()��ʼͼ�μ�����
	void SetImage(IplImage *img);
	//grayΪ����ͼ��֮�ҽ�ͼ
	//inputImgΪ��¼����ͼ��ָ��
	//eqgrayΪ��¼grayͼ��ֱ��ͼ�Ȼ���Ľ��
	//small_imgΪ��¼grayͼ����С��֮���
	//faceImg��¼����ͼ��
	//eqfaceImg��¼����ͼ��ֱ��ͼ�Ȼ�֮���
	IplImage *gray,*inputImg,*eqgray,*small_img,*faceImg,*eqfaceImg;
	//BGRfaceΪ��¼���֮���
	IplImage *BGRface;
	//��̬ͼ�μ�����ָ֮��
	u8 *eqfaceB,*faceImgB,*bgrB;
	//facetotal��¼��֮������flag��¼�Ƿ�����������
	int facetotal,flag;
	//�ǲ��ķ���ָ���¼
	int nob,not,nol,nor;
	//�첿�ķ���ָ���¼
	int mob,mot,mol,mor;
	//���۲��ķ���ָ���¼
	int reb,ret,rel,rer;
	//���۲��ķ���ָ���¼
	int leb,let,lel,ler;
	int lbound,rbound,hRec;
	//scaleΪ���ű�
	double scale;
	//���ܵ����������ۡ����ۡ��첿���ǲ��ļ������
	CvSeq *faces,*leye,*reye,*mouth,*nose;
	//wh��¼ͼ�γ�����
	CvSize wh;
	//���׿ɿ�����
	double confidence;

private:
	CvMemStorage *storage,*storager,*storagel,*storagen,*storagem;
	CvHaarClassifierCascade *cascade,*cascader,*cascadel,*cascaden,*cascadem;
	BOOL32 isSetImg;
};

//#endif

