/*
 * Conv.h
 *
 *  Created on: Apr 10, 2012
 *      Author: per
 */

#ifndef CONV_H_
#define CONV_H_

#include <algorithm>
#include <cassert>

namespace cil {

void convHor(const Image<unsigned char>& in, Image<unsigned char>& out, Image<unsigned char>& kernel) {
	out.fill(0);

	assert(kernel.getNumChannels() == 1);
	assert(kernel.getSizeY() == 1);
	assert(kernel.getSizeX() & 1); // Odd

	unsigned int kRad = kernel.getSizeX() / 2;

	for(int p = 0; p < in.getNumChannels(); ++p) {
		typename Image<unsigned char>::const_row_iterator srcRow = in.beginRow(p);
		for(typename Image<unsigned char>::row_iterator dstRow = out.beginRow(p); dstRow != out.endRow(p); ++dstRow) {
			typename Row<unsigned char>::const_iterator srcPtr = srcRow.begin() + kRad;
			for(typename Row<unsigned char>::iterator dstPtr = dstRow.begin() + kRad; dstPtr != dstRow.end() - kRad; ++dstPtr) {
				int sum = 0;
				typename Row<unsigned char>::const_iterator srcPtr2 = srcPtr-kRad;
				for(typename Image<unsigned char>::iterator kPtr = kernel.begin(0); kPtr != kernel.end(0); ++kPtr) {
					sum += (int(*kPtr)-128)*(*srcPtr2++);
				}
				*dstPtr = std::max(0, std::min(255, (sum/256)+128));
				srcPtr++;
			}
			srcRow++;
		}
	}
}

};//ns

#endif /* CONV_H_ */
