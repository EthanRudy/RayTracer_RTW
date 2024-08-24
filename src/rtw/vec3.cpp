// vec3.cpp - Partial Implementation of the vec3 class
// Ethan Rudy

#include "../../include/rtw/vec3.hpp"

namespace rtw {

	// Default Constructor
	vec3::vec3() : e{ 0, 0, 0 } {}

	// Element Constructor
	vec3::vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// Accessors
	double vec3::x() const { return e[0]; }
	double vec3::y() const { return e[1]; }
	double vec3::z() const { return e[2]; }

	// Negate Operator
	vec3 vec3::operator-() const { return vec3(-e[0], -e[1], -e[2]); }

	// Accessors (indexed)
	double vec3::operator[](int i) const { return e[i]; }
	double& vec3::operator[](int i) { return e[i]; }

	// Addition & Assigment
	vec3& vec3::operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];

		return *this;
	}

	// Scalar Multiplication & Assignment
	vec3& vec3::operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;

		return *this;
	}

	// Scalar Division & Assignment
	vec3& vec3::operator/=(double t) {
		return *this *= 1 / t;
	}

	// Length
	double vec3::length() const {
		return std::sqrt(length_squared());
	}

	// Length Squared
	double vec3::length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// Near Zero (approx)
	bool vec3::near_zero() const {
		auto s = 1e-8;

		return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
	}
}