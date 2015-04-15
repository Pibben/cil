/*
 * FreenectBackend.h
 *
 *  Created on: Jul 2, 2013
 *      Author: per
 */

#ifndef FREENECTBACKEND_H_
#define FREENECTBACKEND_H_

#include "core/Image.h"

namespace cil {

class FreenectWrapper {

public:
    static FreenectWrapper* getInstance();

    bool update();

    void getRGB(Image<unsigned char>& rgb);
    void getRawDepth(Image<unsigned short>& rawDepth);
    void getCombo(Image<unsigned char>& combo);
    void getDisplayDepth(Image<unsigned char>& displayDepth);

private:
    static FreenectWrapper *_instance;

    FreenectWrapper();
    virtual ~FreenectWrapper();

    bool started;

    Image<unsigned char> mRgbImage;
    Image<unsigned char> mDepthImage;
    Image<unsigned char> mComboImage;
    Image<unsigned short> mRawDepth;
};

}; //ns
#endif /* FREENECTBACKEND_H_ */
