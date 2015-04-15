/*
 * Image.cpp
 *
 *  Created on: 20 mar 2012
 *      Author: Per Zetterlund
 */

#include "backends/OpenCVBackend.h"
#include "Image.h"

namespace cil {

template <>
void Image<unsigned char>::display() {
	OpenCVBackend::display(*this);
}

template<>
Image<unsigned char>::Image(std::string filename) {
	OpenCVBackend::load(filename, *this);
}

template <>
void Image<unsigned short>::display() {
	OpenCVBackend::display(*this);
}

template<>
Image<unsigned short>::Image(std::string filename) {
	OpenCVBackend::load(filename, *this);
}

}; //ns
