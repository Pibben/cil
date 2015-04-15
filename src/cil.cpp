//============================================================================
// Name        : cil.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>

#include "gtest/gtest.h"

#include "core/Image.h"
#include "core/Pyramid.h"
#include "filter/Conv.h"
#include "backends/OpenCVBackend.h"

//#include "backends/OpenNIBackend.h"

static unsigned char invert(unsigned char val) { return 255-val; }

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	cil::Image<unsigned char> a(4,4,1);
	cil::Image<unsigned char> b(4,4,4);
	cil::Image<unsigned char> c;
	cil::Image<unsigned char> d(4,4,7);

	cil::Image<bool> lt;

	c = a + b * d;
	c.print();

	a[a > b] = d;
	a.print();
}
#if 0
void opennitest() {
	//Image<unsigned char> i(std::string("/home/per/code/cil/mandrill.png"));
	//Image<unsigned short> i(std::string("/home/per/code/cil/64kSnake.png"));
	//Image<unsigned short> i(std::string("/home/per/code/cil/lena_16bit.png"));
	//Image<unsigned short> i(std::string("/home/per/code/cil/grad16.png"));

	Image<unsigned char> i;

//	Image<unsigned short> i;
//
//	OpenCVBackend be;
//
//	be.initCapture();
//	while(1) {
//		be.capture(i);
//		printf("Got frame!\n");
//		i.display();
//	}

	CVKinectWrapper* k = CVKinectWrapper::getInstance();

	k->init("/home/per/code/lib/OpenNI/Data/SamplesConfig.xml");

	while(1) {

		k->update();

		k->getDisplayDepth(i);
		i.display();

		std::this_thread::sleep_for( std::chrono::milliseconds(4));
	}

	//ImmutableImage<unsigned char> i(512,512,127);
	//Image<unsigned char> i("/home/per/code/cil/lenna512.jpg");
	//Image<unsigned char> i("/home/per/code/cil/qr.png");


//	Image<unsigned char> sub = i.subImage(402, 404, 421, 501);
//	for(ImmutableImage<unsigned char>::PixelIterator it = sub.end(); it != sub.begin(); --it) {
//		*it = 0;
//	}
//	for(ImmutableImage<unsigned char>::PixelIterator it = sub.end(1); it != sub.begin(1); --it) {
//		*it = 0;
//	}
//	for(ImmutableImage<unsigned char>::PixelIterator it = sub.end(2); it != sub.begin(2); --it) {
//		*it = 0;
//	}

//	std::sort(sub.begin(), sub.end());
//	std::sort(sub.begin(1), sub.end(1));
//	std::sort(sub.begin(2), sub.end(2));

//	for(int plane = 0; plane < 3; ++plane) {
//		for(Image<unsigned char>::row_iterator r = sub.beginRow(plane); r != sub.endRow(plane); ++r) {
//			std::transform(r.begin(), r.end(), r.begin(), invert);
//		}
//	}

#if 0
	Pyramid<unsigned char> p(i, 2);
	//Image<unsigned char> all = p.renderAll();
	//printf("X: %d, Y: %d\n", all.getSizeX(), all.getSizeY());

	//all.display();
#endif
	//p[2].display();

//	Image<unsigned char> ic(p[1].getSizeX(), p[1].getSizeY(), p[1].getNumPlanes(), 0);
//	Image<unsigned char> k(9, 1);
//	k.at(0, 0) = 0;
//	k.at(1, 0) = 230;
//	k.at(2, 0) = 0;
//	k.at(3, 0) = 230;
//	k.at(4, 0) = 230;
//	k.at(5, 0) = 230;
//	k.at(6, 0) = 0;
//	k.at(7, 0) = 230;
//	k.at(8, 0) = 0;
//
//	convHor(i, ic, k);
//
//	ic.display();
	while(1);
}
#endif
