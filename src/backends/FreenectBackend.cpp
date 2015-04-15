/*
 * FreenectBackend.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: per
 */

#include "backends/FreenectBackend.h"

namespace cil {

FreenectWrapper* FreenectWrapper::_instance;

FreenectWrapper::FreenectWrapper() {

    started = false;
}

FreenectWrapper* FreenectWrapper::getInstance(){

    if(!_instance){

        _instance = new CVKinectWrapper();
    }

    return _instance;
}

FreenectWrapper::~FreenectWrapper() {

    started = false;
    _instance = 0;

}

bool FreenectWrapper::update() {
    uint8_t data;
    unsigned int timestamp;
    freenect_sync_get_video((void**)(&data), &timestamp, 0, FREENECT_VIDEO_RGB);

}

}; //ns


