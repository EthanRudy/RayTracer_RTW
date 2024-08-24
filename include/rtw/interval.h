// interval.h - Declaration of the Interval class
// Ethan Rudy

#ifndef INTERVAL_H
#define INTERVAL_H

#include "consts.hpp"

namespace rtw {

	/**
	* Interval class
	* Holds a min and max value
	* Provides functionality of a continuous
	* set. Stuff like contains, surrounds, etc
	*/
	class Interval {
	public:

		// Bounds
		double min, max;

		/**
		* Default Constructor
		*/
		Interval();

		/**
		* Min/Max Constructor
		* 
		* @param min	Minimum value
		* @param max	Maximum value
		*/
		Interval(double min, double max);

		/**
		* Union Constructor
		* Performs a union operator on the two Intervals
		* 
		* @param a	Interval One
		* @param a	Interval Two
		*/
		Interval(const Interval& a, const Interval& b);

		/**
		* Size / Span
		*/
		double size() const;

		/**
		* Contains Number 'x'
		* 
		* @return whether min <= x <= max
		*/
		double contains(double x) const;

		/**
		* Surrounds Number 'x'
		* 
		* @return whether min < x < max
		*/
		bool surrounds(double x) const;

		/**
		* Clamp 'x'
		* 
		* @return A new value of x clamped to the Interval's range
		*/
		double clamp(double x) const;

		/**
		* Expand
		* Expands the Interval evenly
		* (.5 * delta) to the left, and (.5 * delta) to the right
		* 
		* @return The expanded Interval
		*/
		Interval expand(double delta) const;

		// A couple of useful Invervals
		static const Interval empty;
		static const Interval universe;
	};
	
	
	

}

#endif // !INTERVAL_H
