/*
 * Storage.h
 *
 *  Created on: Apr 7, 2012
 *      Author: per
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <cassert>

template <typename T>
class Storage {
private:
	uint16_t mRefCnt;
	bool mAllocated;
public:
	T* mData;
	//T* mDataEnd;

	//Storage() : mRefCnt(1), mData(NULL), mDataEnd(NULL), mAllocated(false) {}
	Storage(uint32_t size) :   mRefCnt(1), mAllocated(true), mData(new T[size])/*, mDataEnd(mData+size)*/ {}
	Storage(uint32_t size, T* data, bool transferOwnership = false) :  mRefCnt(1), mAllocated(transferOwnership), mData(data)/*, mDataEnd(mData+size)*/ {}

	~Storage() {
//		assert(mRefCnt == 0);
		if(mAllocated) {
			delete [] mData;
		}
	}

//	void inc() { ++mRefCnt; }
//	void dec() { --mRefCnt; }
//	bool orphan() { return mRefCnt == 0; }

//	Storage clone() {
//		Storage<T> retval(mDataEnd-mData);
//		std::copy(mData, mDataEnd, retval.mData);
//		return retval;
//	}
private:
	Storage(const Storage& other);
	Storage& operator=(const Storage& other);
};


#endif /* STORAGE_H_ */
