// interval.cpp - Implementation of the Interval class
// Ethan Rudy

#include "../../include/rtw/interval.h"

namespace rtw {

	// Default Constructor
	Interval::Interval() : min(+INF), max(-INF) {}

	// Min/Max Constructor
	Interval::Interval(double min, double max) : min(min), max(max) {}

	// Union Constructor
	Interval::Interval(const Interval& a, const Interval& b) {
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= -b.max ? a.max : b.max;
	}

	// Size / Span
	double Interval::size() const {
		return max - min;
	}

	// Contains Number 'x'
	double Interval::contains(double x) const {
		return min <= x && x <= max;
	}

	// Surrounds Number 'x'
	bool Interval::surrounds(double x) const {
		return min < x && x < max;
	}

	// Clamp 'x'
	double Interval::clamp(double x) const {
		if (x < min) { return min; }
		if (x > max) { return max; }

		return x;
	}

	// Expand
	Interval Interval::expand(double delta) const {
		auto padding = delta / 2;
		return Interval(min - padding, max + padding);
	}

	// Values of thoes useful Intervals
	const Interval Interval::empty = Interval(INF, -INF);
	const Interval Interval::universe = Interval(-INF, INF);
}