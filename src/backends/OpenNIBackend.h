/*
 * OpenNIBackend.h
 *
 *  Created on: Nov 9, 2012
 *      Author: per
 */

#ifndef OPENNIBACKEND_H_
#define OPENNIBACKEND_H_

#define linux 1

#include <XnOS.h>
#include <XnCppWrapper.h>

#include <math.h>

#include "core/Image.h"

#define DISPLAY_MODE_OVERLAY	1
#define DISPLAY_MODE_DEPTH		2
#define DISPLAY_MODE_IMAGE		3
#define DEFAULT_DISPLAY_MODE	DISPLAY_MODE_OVERLAY

namespace cil {

class CVKinectWrapper {

public:

	bool init(std::string CalibFilePath);
	static CVKinectWrapper* getInstance();

    bool update();

    void getRGB(Image<unsigned char>& rgb);
    void getRawDepth(Image<unsigned short>& rawDepth);
    void getCombo(Image<unsigned char>& combo);
    void getDisplayDepth(Image<unsigned char>& displayDepth);

private:
	static CVKinectWrapper *_instance;

	CVKinectWrapper();
	virtual ~CVKinectWrapper();


	std::string CalibFilePath;
	bool started;

	xn::Context g_context;

	xn::ScriptNode g_scriptNode;
	xn::DepthGenerator g_depth;
	xn::ImageGenerator g_image;

	xn::DepthMetaData g_depthMD;
	xn::ImageMetaData g_imageMD;

	Image<unsigned char> mRgbImage;
	Image<unsigned char> mDepthImage;
	Image<unsigned char> mComboImage;
	Image<unsigned short> mRawDepth;
};

};//ns

#endif /* OPENNIBACKEND_H_ */
