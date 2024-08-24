// ray.cpp - Implementation file for the ray class
// Ethan Rudy

#include "../../include/rtw/ray.h"

namespace rtw {

	// Default Constructor
	ray::ray() {}

	// Time Constructor
	ray::ray(const point3& origin, const vec3& direction, double time) {
		orig = origin;
		dir = direction;
		tm = time;
	}

	// Timeless Constructor
	ray::ray(const point3& origin, const vec3& direction) {
		orig = origin;
		dir = direction;
		tm = 0;
	}

	

	// Origin Accessor
	const point3& ray::origin() const {
		return orig;
	}

	// Direction Accessor
	const point3& ray::direction() const {
		return dir;
	}

	// Time Accessor
	double ray::time() const { return tm; }

	// Ray at Time 't'
	point3 ray::at(double t) const {
		return orig + t * dir;
	}
}