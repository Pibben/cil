/*
 * Color.h
 *
 *  Created on: Apr 22, 2012
 *      Author: per
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "core/Image.h"

template <typename T>
Image<T> toGray(const Image<T>& in) {
	assert(in.getNumChannels() == 3);

	Image<T> retval(in.getSizeX(), in.getSizeY());

	typename Image<T>::const_iterator red  =  in.begin(0);
	typename Image<T>::const_iterator green = in.begin(1);
	typename Image<T>::const_iterator blue  = in.begin(2);

	typename Image<T>::iterator out = retval.begin(0);

	while(out != retval.end(0)) {
		*out++ = (*red++ + *green++ + *blue++) / 3;
	}

	return retval;
}


#endif /* COLOR_H_ */
