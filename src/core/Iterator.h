/*
 * Iterator.h
 *
 *  Created on: Apr 1, 2012
 *      Author: per
 */

#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <cassert>
#include <iterator>

template<typename T>
class Channel;

template<typename T, typename A, typename B, typename C>
class PixelIterator : public std::iterator<std::random_access_iterator_tag, T> {
private:
	C mChannel;
	B mPtr;
	B mStartOfLine;
	B mEndOfLine;
	int mDeadSpace;
public:

	typedef typename std::iterator<std::random_access_iterator_tag, T>::difference_type difference_type;

	PixelIterator(C channel, B ptr) : mChannel(channel), mPtr(ptr),
	                                  mStartOfLine(channel.getData()+(((ptr-channel.getData())/channel.getPitch())*channel.getPitch())),
	                                  mEndOfLine(mStartOfLine+channel.getSizeX()),
	                                  mDeadSpace(channel.getPitch()-channel.getSizeX()) {
		assert(mPtr >= mStartOfLine);
		assert(mPtr <= mEndOfLine);
		assert(mPtr >= mChannel.getData());
	}

	//PixelIterator(const PixelIterator& other) : mPlane(other.mPlane), mPtr(other.mPtr), mStartOfLine(other.mStartOfLine), mEndOfLine(other.mEndOfLine), mDeadSpace(other.mDeadSpace) {}

	PixelIterator& operator++() {
		assert(mPtr != dataEnd());
		++mPtr;
		if(mPtr == mEndOfLine) {
			mStartOfLine+=mChannel.getPitch();
			mEndOfLine+=mChannel.getPitch();
			//assert(mEndOfLine <= mPlane.getData()+mPlane.getPitch()*mPlane.getSizeY());
			mPtr = mStartOfLine;
		}

		return *this;
	}

	PixelIterator operator++(int) {
		PixelIterator tmp(*this);
		operator++();
		return tmp;
	}

	PixelIterator& operator--() {
		assert(mPtr != mChannel.getData());
		if(mPtr == mStartOfLine) {
			mStartOfLine-=mChannel.getPitch();
			mEndOfLine-=mChannel.getPitch();
			//assert(mEndOfLine <= mPlane.getData()+mPlane.getPitch()*mPlane.getSizeY());
			mPtr = mEndOfLine-1;
		} else {
			--mPtr;
		}

		return *this;
	}

	PixelIterator operator--(int) {
		PixelIterator tmp(*this);
		operator--();
		return tmp;
	}

	bool operator==(const PixelIterator& rhs) { return mPtr == rhs.mPtr; }
	bool operator!=(const PixelIterator& rhs) { return mPtr != rhs.mPtr; }

	A operator*() { return *mPtr; }

	PixelIterator& operator=(const PixelIterator& other) {
		mChannel = other.mChannel;
		mPtr = other.mPtr;
		mStartOfLine = other.mStartOfLine;
		mEndOfLine = other.mEndOfLine;
		mDeadSpace = other.mDeadSpace;
		return *this;
	}

	PixelIterator operator+(difference_type offset) {
		PixelIterator retval(*this);

		retval += offset;

		return retval;
	}

	PixelIterator& operator+=(difference_type offset) {
//			if(offset == 0 || mDeadSpace == 0) {
//				return PixelIterator(mPlane, mPtr+offset);
//			}

		const unsigned int x = (mPtr-mChannel.getData()) % mChannel.getPitch();

		int rows = offset / mChannel.getSizeX();
		const unsigned int rest = offset % mChannel.getSizeX();

		if(x + rest >= mChannel.getSizeX()) {
			rows++;
		}

		mPtr += offset+rows*mDeadSpace;

		return *this;
	}

	PixelIterator operator-(difference_type offset) {
		//if(offset == 1) { PixelIterator tmp(*this); return --tmp; }

		const int x = (mPtr-mChannel.getData()) % mChannel.getPitch();

		int rows = offset / mChannel.getSizeX();
		const int rest = offset % mChannel.getSizeX();

		if(x - rest < 0) {
			rows++;
		}

		return PixelIterator(mChannel, mPtr-offset-rows*mDeadSpace);
	}

	difference_type operator-(const PixelIterator& other) {
		int sign = 1;

		B ptr1 = other.mPtr;
		B ptr2 = mPtr;

		if(ptr2 < ptr1) {
			std::swap(ptr1, ptr2);
			sign = -1;
		}

		const int cp = ptr2-ptr1;
		const unsigned int x = (ptr1-mChannel.getData()) % mChannel.getPitch();

//			if(cp == 0 || mDeadSpace == 0) {
//				return cp;
//			}

		assert(cp >= 0);
		int rows = cp / mChannel.getPitch();
		const unsigned int rest = cp % mChannel.getPitch();

		if(x + rest >= mChannel.getSizeX()) {
			rows++;
		}

		//printf("0x%08x - 0x%08x (%d) -> %d\n", ptr1, ptr2, ptr2-ptr1, cp-rows*mDeadSpace);
		assert(cp >= rows*mDeadSpace);

		return sign*(cp-rows*mDeadSpace);
	}

	bool operator<(const PixelIterator& other) {
		return mPtr < other.mPtr;
	}

	const T* dataEnd() {
		return mChannel.getData()+mChannel.getPitch()*mChannel.getSizeY();
	}

	/*
	void swap(PixelIterator& other) {
		std::swap(mPlane, other.mPlane);
		std::swap(mPtr, other.mPtr);
		std::swap(mStartOfLine, other.mStartOfLine);
		std::swap(mEndOfLine, other.mEndOfLine);
	}
	*/
};





#endif /* ITERATOR_H_ */
