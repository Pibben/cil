/*
 * RowIterator.h
 *
 *  Created on: Apr 1, 2012
 *      Author: per
 */

#ifndef ROWITERATOR_H_
#define ROWITERATOR_H_

#include <cassert>
#include <iterator>
#include "core/Row.h"

template <typename T, typename B, typename C>
class RowIterator : public std::iterator<std::random_access_iterator_tag, Row<T> > {
private:
	typedef typename std::iterator<std::random_access_iterator_tag, Row<T> >::difference_type difference_type;

	C mChannel;
	B mPtr;
public:
	RowIterator(C channel, B ptr) : mChannel(channel), mPtr(ptr) {
		assert(mPtr >= mChannel.getData());
	}

	RowIterator& operator++() {
		assert(mPtr != mChannel.getDataEnd());
		mPtr += mChannel.getPitch();

		return *this;
	}

	RowIterator operator++(int) {
		RowIterator tmp(*this);
		operator++();
		return tmp;
	}

	RowIterator& operator--() {
		assert(mPtr != mChannel.getData());
		mPtr -= mChannel.getPitch();
		return *this;
	}

	RowIterator operator--(int) {
		RowIterator tmp(*this);
		operator--();
		return tmp;
	}

//	typename Row<T>::iterator begin() {
//		return Row<T>(mPlane, mPtr).begin();
//	}
//
//	typename Row<T>::iterator end() {
//		return Row<T>(mPlane, mPtr).end();
//	}

	typename Row<T>::iterator begin() {
		return typename Row<T>::iterator(mPtr);
	}

	typename Row<T>::iterator end() {
		return typename Row<T>::iterator(mPtr+mChannel.getSizeX());
	}

	const typename Row<T>::iterator begin() const {
		return typename Row<T>::iterator(mPtr);
	}

	const typename Row<T>::iterator end() const {
		return typename Row<T>::iterator(mPtr+mChannel.getSizeX());
	}

	bool operator==(const RowIterator& rhs) { return mPtr == rhs.mPtr; }
	bool operator!=(const RowIterator& rhs) { return mPtr != rhs.mPtr; }

	Row<T> operator*() { return Row<T>(mChannel, mPtr); }

	RowIterator& operator=(const RowIterator& other) {
		mChannel = other.mChannel;
		mPtr = other.mPtr;
		return *this;
	}

	RowIterator operator+(difference_type offset) {
		return RowIterator(mChannel, mPtr+offset*mChannel.getPitch());
	}

	const RowIterator operator-(difference_type offset) {

		return RowIterator(mChannel, mPtr-offset*mChannel.getPitch());
	}

	difference_type operator-(const RowIterator& other) {
		difference_type d = mPtr-other;
		assert((std::abs(d) % mChannel.getPitch()) == 0);
		return d / mChannel.getPitch();
	}

	bool operator<(const RowIterator& other) {
		return mPtr < other.mPtr;
	}

};

#endif /* ROWITERATOR_H_ */
