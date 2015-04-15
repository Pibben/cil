/*
 * SubImage.h
 *
 *  Created on: Nov 8, 2012
 *      Author: per
 */

#ifndef SUBIMAGE_H_
#define SUBIMAGE_H_

template <typename T>
class Image;

template <typename T, typename Base = Image<T> >
class SubImage {
private:
	Base& mBase;
	uint_fast16_t mX0;
	uint_fast16_t mY0;
	uint_fast16_t mX1;
	uint_fast16_t mY1;
	std::vector<Channel<T> > mChannels;

public:
	SubImage(Base& base, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) : mBase(base), mX0(x0), mY0(y0), mX1(x1), mY1(y1) {
		const int ysize = mY1 - mY0;
		const int xsize = mX1 - mX0;
		for(int i = 0; i < mBase.getNumChannels(); ++i) {
			mChannels.emplace_back(xsize, ysize, mBase.getPitch(), mBase.getData(i)+mX0, false);
		}
	}

	Image<T> clone() const {
		const int ysize = mY1 - mY0;
		const int xsize = mX1 - mX0;
		Image<T> img(xsize, ysize, mBase.getNumChannels(), 0);
		for(int y = 0; y < ysize; ++y) {
			for(int x = 0; x < xsize; ++x) {
				for(int c = 0; c < mBase.getNumChannels(); ++c) {
					img.at(x, y, c) = mBase.at(x+mX0, y+mY0, c);
				}
			}
		}

		return img;
	}

	const T& at(uint_fast16_t x, uint_fast16_t y, uint_fast8_t c = 0) const {
		return mBase.at(x+mX0,y+mY0, c);
	}

	T& at(uint_fast16_t x, uint_fast16_t y, uint_fast8_t c = 0) {
		return mBase.at(x+mX0,y+mY0, c);
	}

	uint_fast8_t getNumChannels() const {
		return mBase.getNumChannels();
	}

	SubImage<T, SubImage<T, Base> > subImage(uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) {
		return SubImage<T, SubImage<T, Base> >(*this, x0, y0, x1, y1);
	}

	const T* getData(uint_fast8_t channel = 0) const {
		return mBase.getData()+mX0;
	}

	T* getData(uint_fast8_t channel = 0) {
		return mBase.getData()+mX0;
	}

	uint_fast16_t getPitch() const {
		return mBase.getPitch();
	}
};


#endif /* SUBIMAGE_H_ */
