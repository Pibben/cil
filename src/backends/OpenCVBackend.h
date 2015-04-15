
#ifndef OPENCVBACKEND_H_
#define OPENCVBACKEND_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "core/Image.h"

namespace cil {

template <typename T>
class Image;

template <typename T>
struct TypeConverter;

template <>
struct TypeConverter<unsigned char> {
	enum { type =  CV_8U };
};

template <>
struct TypeConverter<unsigned short> {
	enum { type =  CV_16U };
};

class OpenCVBackend {
public:
	template <typename T>
	static void convert(Image<T>& image, cv::Mat& mat) {
		uint_fast8_t numPlanes = image.getNumChannels();

		if(numPlanes == 1) {
			cv::Mat t(image.getSizeX(), image.getSizeY(), TypeConverter<T>::type, image.getData(), image.getPitch()*sizeof(T));
			std::swap(mat, t);
		} else {
			std::vector<cv::Mat> cvPlanes(numPlanes);
			for(int i = 0; i < numPlanes; ++i) {
				cvPlanes[i] = cv::Mat(image.getSizeX(), image.getSizeY(), TypeConverter<T>::type, image.getData(i), image.getPitch()*sizeof(T));
			}
			cv::merge(cvPlanes, mat);
		}
	}

	template <typename T>
	static void convert(const Image<T>& image, cv::Mat& mat) {
		uint_fast8_t numPlanes = image.getNumChannels();

		if(numPlanes == 1) {
			cv::Mat t(image.getSizeY(), image.getSizeX(), TypeConverter<T>::type, image.getDataRW(), image.getPitch()*sizeof(T));
			std::swap(mat, t);
		} else {
			std::vector<cv::Mat> cvPlanes(numPlanes);
			for(int i = 0; i < numPlanes; ++i) {
				cvPlanes[i] = cv::Mat(image.getSizeY(), image.getSizeX(), TypeConverter<T>::type, image.getDataRW(i), image.getPitch()*sizeof(T));
			}
			cv::merge(cvPlanes, mat);
		}
	}

	template <typename T>
	static void convert(cv::Mat& img, Image<T>& image) {
		if(img.channels() == 1) {
			Image<T> imimg(img.cols, img.rows);

			assert(img.step == img.cols*sizeof(T));

			std::copy((T*)img.datastart, (T*)img.dataend, imimg.begin(0));

			image.swap(imimg);
		} else  {
			Image<T> imimg(img.cols, img.rows, img.channels(), 1);

			std::vector<cv::Mat> cvPlanes(img.channels());
			cv::split(img, cvPlanes);
			for(int i = 0; i < img.channels(); ++i) {
				std::copy((T*)cvPlanes[i].datastart, (T*)cvPlanes[i].dataend, imimg.begin(i));
			}

			image.swap(imimg);
		}
	}

	template <typename T>
	static void display(const Image<T>& image) {
		cv::Mat img;
		convert(image, img);

		cv::namedWindow("Foo");
		cv::imshow("Foo", img);

		cv::waitKey(1); // Handle window events
	}

	template <typename T>
	static void load(std::string filename, Image<T>& image) {
		cv::Mat img = cv::imread(filename, -1);

		convert(img, image);
	}

	cv::VideoCapture cap;

	void initCapture() {
		cap = cv::VideoCapture(0);
		//assert(cap.isOpened());

		printf("CV_CAP_PROP_FRAME_WIDTH: %f\n", cap.get(CV_CAP_PROP_FRAME_WIDTH));
		printf("CV_CAP_PROP_FRAME_HEIGHT: %f\n", cap.get(CV_CAP_PROP_FRAME_HEIGHT));
		printf("CV_CAP_PROP_FPS: %f\n", cap.get(CV_CAP_PROP_FPS));
		printf("CV_CAP_PROP_FOURCC: %f\n", cap.get(CV_CAP_PROP_FOURCC));
	}

	template <typename T>
	void capture(Image<T>& image) {
		cv::Mat frame;

		cap >> frame;
		convert(frame, image);
	}
};

}; //ns


#endif
