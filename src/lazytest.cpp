/*
 * lazytest.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: per
 */

#include <cassert>
#include <iostream>
#include <vector>

namespace test {

	template <class T>
	struct PlusOp {
		static inline T apply(const T& rhs, const T& lhs) { return rhs + lhs; }
	};

	template <class T>
	struct MinusOp {
		static inline T apply(const T& rhs, const T& lhs) { return rhs - lhs; }
	};

	template <class T>
	struct MultOp {
		static inline T apply(const T& rhs, const T& lhs) { return rhs * lhs; }
	};

	template <class T>
	struct DivOp {
		static inline T apply(const T& rhs, const T& lhs) { return rhs / lhs; }
	};


	template <class T, class Lhs, class Rhs, template <class> class Op>
	struct TplView {
	    typedef T Base;
		TplView(const Lhs& lhs, const Rhs& rhs) : mLhs(lhs), mRhs(rhs) { }
		const Lhs& mLhs;
		const Rhs& mRhs;
		T getVal(int x) const {
		    Op<T> op;
			return op(mLhs.getVal(x), mRhs.getVal(x));
		}
		size_t size() const {
		    assert(mLhs.size() == mRhs.size());
		    return mLhs.size();
		}
	};

	template <class Lhs, class Rhs>
	using PlusView = TplView<typename Lhs::Base, Lhs, Rhs, std::plus>;

	template <class Lhs, class Rhs>
	using MinusView = TplView<typename Lhs::Base, Lhs, Rhs, std::minus>;

	template <class Lhs, class Rhs>
	using MultView = TplView<typename Lhs::Base, Lhs, Rhs, std::multiplies>;

	template <class Lhs, class Rhs>
	using DivView = TplView<typename Lhs::Base, Lhs, Rhs, std::divides>;


	template <class T>
	struct Image {
	    typedef T Base;
		Image(const std::vector<T>& vec) : mVal(vec) {}
		std::vector<T> mVal;
		T getVal(int x) const { return mVal[x]; }
		size_t size() const { return mVal.size(); }

		template <class U>
		Image& operator=(const U& other) {
		    mVal.clear();
		    for(int i = 0; i < other.size(); ++i) {
		        mVal[i] = other.getVal(i);
		    }
			return *this;
		}
	};

	template <class Lhs, class Rhs>
	inline const PlusView<Lhs,Rhs> operator+(const Lhs& lhs, const Rhs& rhs) {
		return PlusView<Lhs, Rhs>(lhs, rhs);
	}

	template <class Lhs, class Rhs>
    inline const MultView<Lhs,Rhs> operator*(const Lhs& lhs, const Rhs& rhs) {
        return MultView<Lhs, Rhs>(lhs, rhs);
    }
};

void foo() {
    std::vector<unsigned char> a({1});
    std::vector<unsigned char> b({2});
    std::vector<unsigned char> c({3});
    std::vector<unsigned char> d({0});

    for(size_t i = 0; i < a.size(); ++i) {
        d[i] = (a[i]+b[i])*c[i];
    }

    std::cout << int{d[0]} << std::endl;
}

int main() {
	test::Image<unsigned char> a({1});
	test::Image<unsigned char> b({2});
	test::Image<unsigned char> c({3});
	test::Image<unsigned char> d({0});
	d = (a+b)*c;

	std::cout << int{d.getVal(0)} << std::endl;
}
