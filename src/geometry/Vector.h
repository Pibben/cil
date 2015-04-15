/*
 * Vector.h
 *
 *  Created on: 21 sep 2011
 *      Author: Per Zetterlund
 */

#ifndef VECTOR_H_
#define VECTOR_H_

template <typename T>
class Vector2Tpl {
public:
	T x;
	T y;
};

template <typename T>
class Vector3Tpl {
public:
	T x;
	T y;
	T z;
};

template <typename T>
class Vector4Tpl {
public:
	T x;
	T y;
	T z;
	T w;
};

typedef Vector3Tpl<double> Vector3;

#endif /* VECTOR_H_ */


