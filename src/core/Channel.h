/*
 * Plane.h
 *
 *  Created on: Apr 1, 2012
 *      Author: per
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <memory>
#include <vector>

#include "core/Iterator.h"
#include "core/Storage.h"
#include "core/RowIterator.h"

template<typename T>
class Channel {
private:
	uint_fast16_t mSizeX;
	uint_fast16_t mSizeY;
	uint_fast16_t mPitch;
	std::shared_ptr<std::vector<T>> mStorage;
	T* mData;

public:
	typedef RowIterator<T, T*, Channel<T>&> row_iterator;
	typedef RowIterator<T, const T*, const Channel<T>&> const_row_iterator;

	typedef PixelIterator<T, T&, T*, Channel<T>&> iterator;
	typedef PixelIterator<T, const T&, const T*, const Channel<T>&> const_iterator;

	Channel() : mSizeX(0), mSizeY(0), mPitch(0), mStorage(NULL), mData(NULL) {}

	Channel(uint_fast16_t sizeX, uint_fast16_t sizeY) : mSizeX(sizeX), mSizeY(sizeY), mPitch(sizeX+17), mStorage(new std::vector<T>(mPitch*mSizeY)), mData(mStorage->data()) {}

	Channel(uint_fast16_t sizeX, uint_fast16_t sizeY, uint_fast16_t pitch, T* data, bool transferOwnership = true) : mSizeX(sizeX), mSizeY(sizeY), mPitch(pitch), mStorage(), mData(data) {}

	const Channel& operator=(const Channel& other) {
		Channel tmp(other);
		swap(tmp);
		return *this;
	}

	Channel subImage(uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) const {
		Channel retval;
		retval.mSizeX = x1-x0+1;
		retval.mSizeY = y1-y0+1;
		retval.mPitch = mPitch;
		retval.mStorage = mStorage;
		retval.mData = mData + y0*mPitch + x0;

		return retval;
		//return Plane(x1-x0+1, y1-y0+1, mPitch, mData + y0*mPitch + x0, false);
	}

	const T& at(uint_fast16_t x, uint_fast16_t y) const {
		return mData[y*mPitch+x];
	}

	T& at(uint_fast16_t x, uint_fast16_t y) {
		return mData[y*mPitch+x];
	}

	const T* getRowPtr(uint_fast16_t row) const {
		return mData + row*mPitch;
	}

	T* getRowPtr(uint_fast16_t row) {
		return mData + row*mPitch;
	}

	uint_fast16_t getSizeX() const {
		return mSizeX;
	}

	uint_fast16_t getSizeY() const {
		return mSizeY;
	}

	uint_fast16_t getPitch() const {
		return mPitch;
	}

	T* getData() const {
		return mData;
	}

	T* getDataEnd() const {
		return mData+mPitch*mSizeY;
	}

	void print() {
		for(uint_fast16_t row = 0; row < mSizeY; ++row) {
			for(uint_fast16_t col = 0; col < mSizeX; ++col) {
				std::cout << static_cast<int>(at(row, col)) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	iterator begin() {
		return iterator(*this, getData());
	}

	iterator end() {
		return iterator(*this, getDataEnd());
	}

	const_iterator begin() const {
		return const_iterator(*this, getData());
	}

	const_iterator end() const {
		return const_iterator(*this, getDataEnd());
	}

	row_iterator beginRow() {
		return row_iterator(*this, getData());
	}

	row_iterator endRow() {
		return row_iterator(*this, getDataEnd());
	}

	const_row_iterator beginRow() const {
		return const_row_iterator(*this, getData());
	}

	const_row_iterator endRow() const {
		return const_row_iterator(*this, getDataEnd());
	}

	void copyFrom(const Channel& other) {
		assert(other.getSizeX() == getSizeX());
		assert(other.getSizeY() == getSizeY());
		std::copy(other.begin(), other.end(), begin());
		//std::fill(begin(), end(), 127);
	}

	void fill(T value) {
		std::fill(begin(), end(), value);
	}

private:
	void swap(Channel& other) {
		std::swap(mSizeX, other.mSizeX);
		std::swap(mSizeY, other.mSizeY);
		std::swap(mPitch, other.mPitch);
		std::swap(mStorage, other.mStorage);
		std::swap(mData, other.mData);
	}
};


#endif /* CHANNEL_H_ */
