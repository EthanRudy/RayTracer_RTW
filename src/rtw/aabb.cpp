// aabb.cpp - Implementation of the aabb class
// Ethan Rudy

#include "../../include/rtw/aabb.h"

namespace rtw {

	// Default Constructor
	aabb::aabb() {}

	// Interval/Side Constructor
	aabb::aabb(const Interval& x, const Interval& y, const Interval& z)
		: x(x), y(y), z(z) {}

	// Point Constructor
	aabb::aabb(const point3& a, const point3& b) {
		x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
	}

	// Union Constructor
	aabb::aabb(const aabb& box0, const aabb& box1) {
		x = Interval(box0.x, box1.x);
		y = Interval(box0.y, box1.y);
		z = Interval(box0.z, box1.z);
	}

	// Interval / "Side Length" of Axis
	const Interval& aabb::axis_interval(int n) const {
		if (n == 1) { return y; }
		if (n == 2) { return z; }
		return x;
	}

	// Hit
	bool aabb::hit(const ray& r, Interval ray_t) const {
		const point3& ray_orig = r.origin();
		const point3& ray_dir = r.direction();

		for (int axis = 0; axis < 3; ++axis) {
			const Interval& ax = axis_interval(axis);
			const double adinv = 1.0 / ray_dir[axis];

			auto t0 = (ax.min - ray_orig[axis]) * adinv;
			auto t1 = (ax.max - ray_orig[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > ray_t.min) ray_t.min = t0;
				if (t1 > ray_t.max) ray_t.max = t1;
			}
			else {
				if (t1 > ray_t.min) ray_t.min = t1;
				if (t0 > ray_t.max) ray_t.max = t0;
			}

			if (ray_t.max < ray_t.min) { return false; }
		}

		return true;
	}

	// Longest Axis (index)
	int aabb::longest_axis() const{
		if (x.size() > y.size()) {
			return x.size() > z.size() ? 0 : 2;
		}
		return y.size() > z.size() ? 1 : 2;
	}

	// Values of those useful boxes
	const aabb aabb::empty = aabb(Interval::empty, Interval::empty, Interval::empty);
	const aabb aabb::universe = aabb(Interval::universe, Interval::universe, Interval::universe);
	
}