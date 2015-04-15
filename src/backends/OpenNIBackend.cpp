/*
 * OpenNIBackend.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: per
 */

#include "backends/OpenNIBackend.h"

namespace cil {

CVKinectWrapper* CVKinectWrapper::_instance;

CVKinectWrapper::CVKinectWrapper() {

	started = false;
}

CVKinectWrapper* CVKinectWrapper::getInstance(){

	if(!_instance){

		_instance = new CVKinectWrapper();
	}

	return _instance;
}

CVKinectWrapper::~CVKinectWrapper() {

	started = false;
	_instance = 0;

}


bool CVKinectWrapper::init(std::string CalibFilePath)
{

	this->CalibFilePath = CalibFilePath;

	XnStatus rc;

	xn::EnumerationErrors errors;
	rc = g_context.InitFromXmlFile(CalibFilePath.c_str(), g_scriptNode, &errors);

	if (rc == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		return started;
	}
	else if (rc != XN_STATUS_OK)
	{
		printf("Open failed: %s\n", xnGetStatusString(rc));
		return started;
	}

	rc = g_context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_depth);
	if (rc != XN_STATUS_OK)
	{
		printf("No depth node exists! Check your XML.");
		return started;
	}

	rc = g_context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
	if (rc != XN_STATUS_OK)
	{
		printf("No image node exists! Check your XML.");
		return started;
	}

	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	// Hybrid mode isn't supported in this sample
	if (g_imageMD.FullXRes() != g_depthMD.FullXRes() || g_imageMD.FullYRes() != g_depthMD.FullYRes())
	{
		printf ("The device depth and image resolution must be equal!\n");
		return started;
	}

	// RGB is the only image format supported.
	if (g_imageMD.PixelFormat() != XN_PIXEL_FORMAT_RGB24)
	{
		printf("The device image format must be RGB24\n");
		return started;
	}

	//g_depth.GetAlternativeViewPointCap().SetViewPoint(g_image);

	mRgbImage = Image<unsigned char>(640, 480, 3, 7);
	mComboImage = Image<unsigned char>(640, 480, 3, 7);
	mDepthImage = Image<unsigned char>(640, 480);
	mRawDepth = Image<unsigned short>(640, 480);

	started = true;
	return started;
}

bool CVKinectWrapper::update(){

	if (!started) return false;

	XnStatus rc = XN_STATUS_OK;

	const XnDepthPixel* pDepth;

	// Read a new frame
	rc = g_context.WaitAnyUpdateAll();
	if (rc != XN_STATUS_OK)
	{
		printf("Read failed: %s\n", xnGetStatusString(rc));
		return  false;
	}

	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	pDepth = g_depthMD.Data();

	const XnRGB24Pixel* pImageRow = g_imageMD.RGB24Data();
	const XnDepthPixel* pDepthRow = g_depthMD.Data();



	for (XnUInt y = 0; y < g_imageMD.YRes(); ++y){

		const XnRGB24Pixel* pImage = pImageRow;
		const XnDepthPixel* pDepth = pDepthRow;

		for (XnUInt x = 0; x < g_imageMD.XRes(); ++x, ++pImage,++pDepth){

			mRgbImage.at(x,y, 0) = pImage->nBlue;
			mRgbImage.at(x,y, 1) = pImage->nGreen;
			mRgbImage.at(x,y, 2) = pImage->nRed;

			if (*pDepth != 0)

				mRawDepth.at(640-x,y) = *pDepth;
			else

				mRawDepth.at(640-x,y) = 0;
		}

		pDepthRow += g_depthMD.XRes();
		pImageRow += g_imageMD.XRes();
	}

	auto max = std::max_element(mRawDepth.begin(), mRawDepth.end());

	std::transform(mRawDepth.begin(), mRawDepth.end(),
			       mDepthImage.begin(), [&max](unsigned short a) { return *max == 0 ? 0 : a*255/ *max; });

	// To gray

	for(uint_fast16_t y = 0; y < mComboImage.getSizeY(); ++y) {
		for(uint_fast16_t x = 0; x < mComboImage.getSizeX(); ++x) {
			if(mDepthImage.at(x,y) != 0) {
				mComboImage.at(x,y,0) = mDepthImage.at(x,y);
				mComboImage.at(x,y,1) = mDepthImage.at(x,y);
				mComboImage.at(x,y,2) = mDepthImage.at(x,y);
			} else {
				mComboImage.at(x,y,0) = mRgbImage.at(x,y,0);
				mComboImage.at(x,y,1) = mRgbImage.at(x,y,1);
				mComboImage.at(x,y,2) = mRgbImage.at(x,y,2);
			}
		}
	}

	return true;
}

void CVKinectWrapper::getRGB(Image<unsigned char>& rgb){
	rgb = mRgbImage;
}

void CVKinectWrapper::getRawDepth(Image<unsigned short>& rawDepth){
	rawDepth = mRawDepth;
}

void CVKinectWrapper::getCombo(Image<unsigned char>& combo){
	combo = mComboImage;
}

void CVKinectWrapper::getDisplayDepth(Image<unsigned char>& displayDepth){
	displayDepth = mDepthImage;
}

}; //ns
