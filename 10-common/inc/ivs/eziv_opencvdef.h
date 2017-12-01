#ifndef __OPENCV_COMMON_DEFINITION_INCLUDE
#define __OPENCV_COMMON_DEFINITION_INCLUDE

//#define _RG4_USING_STATIC_

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

#define IVS_DEFAULT_WIDTH	320
#define IVS_DEFAULT_HEIGHT	240

#define CV_SAFE_RELEASE_HIST(a) {if (a){cvReleaseHist(&(a)); (a) = NULL;}}
#define CV_SAFE_RELEASE_IMAGE(a) {if (a){cvReleaseImage(&(a)); (a) = NULL;}}

#define EASYIV_EXPORTS

#ifdef EASYIV_EXPORTS
#define EASYIV_API    __declspec(dllexport)
#elif defined(EASYIV_IMPORTS)
#define EASYIV_API    __declspec(dllimport)
#else
#define EASYIV_API
#endif

#ifdef _DEBUG
#pragma comment(lib, "opencv_contrib247d.lib")
#pragma comment(lib, "opencv_core247d.lib")
#pragma comment(lib, "opencv_highgui247d.lib")
#pragma comment(lib, "opencv_imgproc247d.lib")

#pragma comment(lib, "opencv_video247d.lib")
#pragma comment(lib, "opencv_legacy247d.lib")
#pragma comment(lib, "opencv_ml247d.lib")
#pragma comment(lib, "opencv_gpu247d.lib")

//#pragma comment(lib, "opencv_haartraining_engined.lib")
#pragma comment(lib, "opencv_flann247d.lib")
#pragma comment(lib, "opencv_features2d247d.lib")
#pragma comment(lib, "opencv_calib3d247d.lib")

#pragma comment(lib, "opencv_objdetect247d.lib") //face detect module

//jacky, if you are linking with static opencv libraries, then you need these.
#ifdef _RG4_USING_STATIC_
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libtiffd.lib")
#pragma comment(lib, "libjpegd.lib")
#pragma comment(lib, "libpngd.lib")
#pragma comment(lib, "libjasperd.lib")
#pragma comment(lib, "IlmImfd.lib")
#endif


#else

#pragma comment(lib, "opencv_contrib247.lib")
#pragma comment(lib, "opencv_core247.lib")
#pragma comment(lib, "opencv_highgui247.lib")
#pragma comment(lib, "opencv_imgproc247.lib")

#pragma comment(lib, "opencv_video247.lib")
#pragma comment(lib, "opencv_legacy247.lib")
#pragma comment(lib, "opencv_ml247.lib")
#pragma comment(lib, "opencv_gpu247.lib")

//#pragma comment(lib, "opencv_haartraining_engine.lib")
#pragma comment(lib, "opencv_flann247.lib")
#pragma comment(lib, "opencv_features2d247.lib")
#pragma comment(lib, "opencv_calib3d247.lib")

#pragma comment(lib, "opencv_objdetect247.lib") //face detect module

//jacky, if you are linking with static opencv libraries, then you need these.
#ifdef _RG4_USING_STATIC_
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libtiff.lib")
#pragma comment(lib, "libjpeg.lib")
#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "libjasper.lib")
#pragma comment(lib, "IlmImf.lib")
#endif

#endif


//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "highgui.lib")
//#pragma comment(lib, "cvaux.lib")
//#pragma comment(lib, "cvcam.lib")
//#pragma comment(lib, "ml.lib")
/*
#ifdef _DEBUG
#pragma comment(lib, "opencv_calib3d240d.lib")
#pragma comment(lib, "opencv_flann240d.lib")
#pragma comment(lib, "opencv_features2d240d.lib")
#pragma comment(lib, "opencv_gpu240d.lib")
#pragma comment(lib, "opencv_haartraining_engined.lib")
#pragma comment(lib, "opencv_imgproc240d.lib")


#pragma comment(lib, "opencv_contrib240d.lib")
#pragma comment(lib, "opencv_core240d.lib")
#pragma comment(lib, "opencv_highgui240d.lib")
#pragma comment(lib, "opencv_legacy240d.lib")
#pragma comment(lib, "opencv_ml240d.lib")
#pragma comment(lib, "opencv_video240d.lib")
//opencv_imgproc220d.lib
#else
#endif




#ifdef _DEBUG
#pragma comment(lib, "cv210d.lib")
#pragma comment(lib, "cxcore210d.lib")
#pragma comment(lib, "highgui210d.lib")
#else
#pragma comment(lib, "cv210.lib")
#pragma comment(lib, "cxcore210.lib")
#pragma comment(lib, "highgui210.lib")
#endif
*/
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

#endif //__OPENCV_COMMON_DEFINITION_INCLUDE