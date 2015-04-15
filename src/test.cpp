/*
 * lazytest.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: per
 */

#include <iostream>

struct B {
	/*
	B(int val) : mVal{val} {}
	int mVal;
	void bar() const { printf("B(%d) is at (0x%08x)\n", mVal, this); }

	template <class T>
	void foo(const T& other) {
		other.bar();
	}
	*/
};

struct A {
	A(const B& ref) : mRef{ref} { printf("B is at (0x%08x) or (0x%08x)\n", &mRef, &ref); }
	//A(const B& ref) : mRef(ref) { printf("B is at (0x%08x) or (0x%08x)\n", &mRef, &ref); }
	const B& mRef;
	void bar() const { mRef.bar(); }
};


int main() {
	B b1;

	A a(b1);
}
