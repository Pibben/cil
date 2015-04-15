/*
 * Matrix.h
 *
 *  Created on: 21 sep 2011
 *      Author: Per Zetterlund
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Angle.h"

template <typename T>
class Matrix4x4Tpl {
private:
	T data[4][4];
public:
	Matrix4x4Tpl mmult(const Matrix4x4Tpl& other) const {
		Matrix4x4Tpl ret;
		for(int row1 = 0; row1 < 4; ++row1) {
			for(int col1 = 0; col1 < 4; ++col1) {
				for(int i = 0; i < 4; ++i) {
					ret.data[row1][col1] += data[row1][i] * other.data[i][col1];
				}
			}
		}
		return ret;
	}

	friend const Matrix4x4Tpl operator*(const Matrix4x4Tpl& lhs, const Matrix4x4Tpl& rhs) {
		return lhs.mmult(rhs);
	}

	static Matrix4x4Tpl identity() {
		Matrix4x4Tpl ret;
		for(int row = 0; row < 4; ++row) {
			for(int col = 0; col < 4; ++col) {
				ret.data[row][col] = row == col ? 1 : 0;
			}
		}
		return ret;
	}

	static Matrix4x4Tpl rotX(Angle a) {
		Matrix4x4Tpl ret = identity();
		ret.data[1][1] = a.cos();
		ret.data[1][2] = -a.sin();
		ret.data[2][1] = a.sin();
		ret.data[2][2] = a.cos();
	}
};

#endif
