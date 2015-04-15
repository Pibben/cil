/*
 * Image.h
 *
 *  Created on: 6 sep 2011
 *      Author: Per Zetterlund
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iterator>
#include <stdint.h>
#include <string>
#include <iostream>
#include <memory>

#include "core/Channel.h"
#include "core/Iterator.h"
#include "core/RowIterator.h"
#include "core/SubImage.h"

namespace cil  {

template <typename T, class Lhs, class Rhs, class Op, typename Ret = T >
struct TplView {
    using Base = Ret;
    TplView(const Lhs& lhs, const Rhs& rhs) : mLhs(lhs), mRhs(rhs) { }
    const Lhs& mLhs;
    const Rhs& mRhs;

    uint_fast16_t getSizeX() const { return mLhs.getSizeX(); }
    uint_fast16_t getSizeY() const { return mLhs.getSizeY(); }

    Ret at(uint_fast16_t x, uint_fast16_t y) const {
        Op op;
        return op(mLhs.at(x,y), mRhs.at(x,y));
    }
};

template <typename T, class Index>
struct IndexView {
    friend class Image<T>;
    using Base = T;
    T& mRef;
    const Index& mIndex;

    IndexView(T& ref, const Index& index) : mRef(ref), mIndex(index) {}

    template <class U>
    void operator=(const U& other) {
        const uint_fast16_t sizeX = other.getSizeX();
        const uint_fast16_t sizeY = other.getSizeX();

        assert(sizeX == mRef.getSizeX());
        assert(sizeY == mRef.getSizeY());

        for(unsigned int y = 0; y < sizeY; ++y) {
            for(unsigned int x = 0; x < sizeX; ++x) {
                if(mIndex.at(x,y)) {
                    mRef.at(x,y) = other.at(x,y);
                }
            }
        }
    }
};

template <class Lhs, class Rhs>
using PlusView = TplView<typename Lhs::Base, Lhs, Rhs, std::plus<typename Lhs::Base>>;

template <class Lhs, class Rhs>
using MinusView = TplView<typename Lhs::Base, Lhs, Rhs, std::minus<typename Lhs::Base>>;

template <class Lhs, class Rhs>
using MultView = TplView<typename Lhs::Base, Lhs, Rhs, std::multiplies<typename Lhs::Base>>;

template <class Lhs, class Rhs>
using DivView = TplView<typename Lhs::Base, Lhs, Rhs, std::divides<typename Lhs::Base>>;

template <class Lhs, class Rhs>
using LTView = TplView<typename Lhs::Base, Lhs, Rhs, std::less<typename Lhs::Base>, bool>;

template <class Lhs, class Rhs>
using LTEView = TplView<typename Lhs::Base, Lhs, Rhs, std::less_equal<typename Lhs::Base>, bool>;

template <class Lhs, class Rhs>
using EQView = TplView<typename Lhs::Base, Lhs, Rhs, std::equal_to<typename Lhs::Base>, bool>;

template <class Lhs, class Rhs>
using NEQView = TplView<typename Lhs::Base, Lhs, Rhs, std::not_equal_to<typename Lhs::Base>, bool>;

template <class Lhs, class Rhs>
using GTView = TplView<typename Lhs::Base, Lhs, Rhs, std::greater<typename Lhs::Base>, bool>;

template <class Lhs, class Rhs>
using GTEView = TplView<typename Lhs::Base, Lhs, Rhs, std::greater_equal<typename Lhs::Base>, bool>;


template <typename T>
class Image {

private:

    std::vector<Channel<T> > mChannels;

public:
    using Base = T;
    typedef typename Channel<T>::iterator iterator;
    typedef typename Channel<T>::const_iterator const_iterator;

    typedef typename Channel<T>::row_iterator row_iterator;
    typedef typename Channel<T>::const_row_iterator const_row_iterator;
    typedef T element_type;

    Image() {}

    template <typename U>
    Image(const U& other) : mChannels(other.clone().mChannels) {}

    Image(uint_fast16_t sizeX, uint_fast16_t sizeY) {
        mChannels.emplace_back(sizeX, sizeY);
    }

    Image(uint_fast16_t sizeX, uint_fast16_t sizeY, uint_fast8_t channels, uint_fast8_t format) {
        mChannels.reserve(channels);
        for(int i = 0; i < channels; ++i) {
            mChannels.emplace_back(sizeX, sizeY);
        }
    }

    Image(uint_fast16_t sizeX, uint_fast16_t sizeY, const T& val) {
        mChannels.emplace_back(sizeX, sizeY);
        fill(val);
    }

    Image(uint_fast16_t sizeX, uint_fast16_t sizeY, uint_fast16_t pitch, T* data) {
        mChannels.emplace_back(sizeX, sizeY, pitch, data);
    }

    Image(std::string filename);

    template <typename U>
    Image& operator=(const U& other) {
        static_assert(std::is_same<Base, typename U::Base>::value, "Types must be same!");

        const uint_fast16_t sizeX = other.getSizeX();
        const uint_fast16_t sizeY = other.getSizeY();
        Image tmp(sizeX, sizeY);

        //TODO: Put this in copy constructor
        for(unsigned int y = 0; y < sizeY; ++y) {
            for(unsigned int x = 0; x < sizeX; ++x) {
                tmp.at(x,y) = other.at(x,y);
            }
        }

        swap(tmp);

        return *this;
    }

    template <class U>
    IndexView<Image<T>, U> operator[](const U& index) {
        static_assert(std::is_same<bool, typename U::Base>::value, "Index type must be bool");

        return IndexView<Image<T>, U>(*this, index);
    }

    SubImage<T, Image<T> > subImage(uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) {
        return SubImage<T,Image<T> >(*this, x0, y0, x1, y1);
    }

    SubImage<T, const Image<T> > subImage(uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) const {
        return SubImage<T, const Image<T> >(*this, x0, y0, x1, y1);
    }

    const T& at(uint_fast16_t x, uint_fast16_t y, uint_fast8_t c = 0) const {
        return mChannels[c].at(x,y);
    }

    T& at(uint_fast16_t x, uint_fast16_t y, uint_fast8_t c = 0) {
        return mChannels[c].at(x,y);
    }

    const T* getRowPtr(uint_fast16_t row) const {
        return mChannels[0].getRowPtr(row);
    }

    T* getRowPtr(uint_fast16_t row) {
        return mChannels[0].getRowPtr(row);
    }

    uint_fast16_t getSizeX() const {
        return mChannels[0].getSizeX();
    }

    uint_fast16_t getSizeY() const {
        return mChannels[0].getSizeY();
    }

    uint_fast16_t getPitch() const {
        return mChannels[0].getPitch();
    }

    const T* getData(uint_fast8_t channel = 0) const {
        return mChannels[channel].getData();
    }

    T* getData(uint_fast8_t channel = 0) {
        return mChannels[channel].getData();
    }

    uint_fast8_t getNumChannels() const {
        return mChannels.size();
    }

    void print() {
        mChannels[0].print();
    }

    void display();
    void load(std::string filename);

    iterator begin(int channel = 0) {
        return mChannels[channel].begin();
    }


    iterator end(int channel = 0) {
        return mChannels[channel].end();
    }

    const_iterator begin(int channel = 0) const {
        return mChannels[channel].begin();
    }

    const_iterator end(int channel = 0) const {
        return mChannels[channel].end();
    }

    row_iterator beginRow(int channel = 0) {
        return mChannels[channel].beginRow();
    }

    row_iterator endRow(int channel = 0) {
        return mChannels[channel].endRow();
    }

    const_row_iterator beginRow(int channel/* = 0*/) const {
        return mChannels[channel].beginRow();
    }


    const_row_iterator endRow(int channel/* = 0*/) const {
        return mChannels[channel].endRow();
    }

    void copyFrom(const Image& other) {
        assert(other.getNumChannels() == getNumChannels());
        assert(other.getSizeX() == getSizeX());
        assert(other.getSizeY() == getSizeY());
        for(size_t i = 0; i < mChannels.size(); ++i) {
            mChannels[i].copyFrom(other.mChannels[i]);
        }
    }

    void fill(T value) {
        for(size_t i = 0; i < mChannels.size(); ++i) {
            mChannels[i].fill(value);
        }
    }

private:
    T* getDataRW(uint_fast8_t channel = 0) const {
        return mChannels[channel].getData();
    }


    void swap(Image& other) {
        mChannels.swap(other.mChannels);
    }


public:

    friend class OpenCVBackend;
    friend class Channel<T>;
};

template <class Lhs, class Rhs>
PlusView<Lhs, Rhs> operator+(const Lhs& lhs, const Rhs& rhs) {
    return PlusView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
MultView<Lhs, Rhs> operator*(const Lhs& lhs, const Rhs& rhs) {
    return MultView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
MinusView<Lhs, Rhs> operator-(const Lhs& lhs, const Rhs& rhs) {
    return MinusView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
DivView<Lhs, Rhs> operator/(const Lhs& lhs, const Rhs& rhs) {
    return DivView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
LTView<Lhs, Rhs> operator<(const Lhs& lhs, const Rhs& rhs) {
    return LTView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
LTEView<Lhs, Rhs> operator<=(const Lhs& lhs, const Rhs& rhs) {
    return LTEView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
EQView<Lhs, Rhs> operator==(const Lhs& lhs, const Rhs& rhs) {
    return EQView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
NEQView<Lhs, Rhs> operator!=(const Lhs& lhs, const Rhs& rhs) {
    return NEQView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
GTView<Lhs, Rhs> operator>(const Lhs& lhs, const Rhs& rhs) {
    return GTView<Lhs, Rhs>(lhs, rhs);
}

template <class Lhs, class Rhs>
GTEView<Lhs, Rhs> operator>=(const Lhs& lhs, const Rhs& rhs) {
    return GTEView<Lhs, Rhs>(lhs, rhs);
}

}; //ns

#endif /* Image_H_ */

