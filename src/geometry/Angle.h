/*
 * Angle.h
 *
 *  Created on: 21 sep 2011
 *      Author: Per Zetterlund
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include <cmath>

static const double PI = 3.14159265358979323846264338327;

class Angle {
public:
	Angle(double a) : radians(a) {}
	double asRad() { return radians; }
	double asDeg() { return radians*180/PI; }
	double sin() { return std::sin(radians); }
	double cos() { return std::cos(radians); }
private:
	double radians;
};

inline Angle Rad(double a) {
	return Angle(a);
}

inline Angle Deg(double a) {
	return Angle(PI/180.0*a);
}

#endif /* ANGLE_H_ */
