/*
 * Row.h
 *
 *  Created on: Apr 1, 2012
 *      Author: per
 */

#ifndef ROW_H_
#define ROW_H_

template <typename T>
class Channel;

template <typename T>
class Row {
private:
	Channel<T>& mChannel;
	T* mPtr;
public:
	typedef T* iterator;
	typedef const T* const_iterator;

	Row() {}

	Row(Channel<T>& channel, T* ptr) : mChannel(channel), mPtr(ptr) {}

	Row(Channel<T>& channel) : mChannel(channel), mPtr(mChannel.getData()) {}

	T* begin() {
		return mPtr;
	}

	T* end() {
		return mPtr+mChannel.getSizeX();
	}

	const T* begin() const {
		return mPtr;
	}

	const T* end() const {
		return mPtr+mChannel.getPitch();
	}
};

template <typename T>
class RowClone : public Row<T> {
private:
	Channel<T> mOwnedChannel;
public:
	RowClone(const Row<T>& row) : mOwnedChannel(row.end()-row.start(), 1), Row<T>(mOwnedChannel) {}

	RowClone(uint16_t size) : mOwnedChannel(size, 1), Row<T>(mOwnedChannel) {}
};

#endif /* ROW_H_ */
