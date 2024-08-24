// aabb.h - Declaration of the aabb class
// Ethan Rudy

#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"

namespace rtw {

	/**
	* AABB Class
	* aka
	* Bounding Box for the BVH
	*/
	class aabb {
	public:

		// Side lengths
		Interval x, y, z;

		// Default Consructor
		aabb();

		/**
		* Interval/Side Constructor
		* Think like passing a length, width, and depth
		* And forming a rectangular prism with that
		* 
		* @param x	First side of the box
		* @param y	Second side of the box
		* @param z	Third and final side of the box
		*/
		aabb(const Interval& x, const Interval& y, const Interval& z);

		/**
		* Point Constructor
		* Think like the bottom front right of a cube,
		* And then the upper back left of the cube
		* Then finally formalling a rectangular prism with that
		* 
		* @param a	First point
		* @param b	Second point
		*/
		aabb(const point3& a, const point3& b);

		/**
		* Union Constructor
		* Just like the above point constructor
		* Don't let the set meaning of union confuse you
		* 
		* @param box0	First Box
		* @param box1	Second Box
		*/
		aabb(const aabb& box0, const aabb& box1);

		/**
		* Interval / "Side Length" of Axis
		* 
		* @param n	Axis
		*/
		const Interval& axis_interval(int n) const;
	
		/**
		* Hit
		* 
		* @param r		Ray
		* @param ray_t	Time Interval of the ray
		* 
		* @return Whether the box has been hit
		*/
		bool hit(const ray& r, Interval ray_t) const;

		/**
		* Longest Axis (index)
		*/
		int longest_axis() const;

		// Useful bounding boxes
		static const aabb empty, universe;
	};

	

}

#endif // !AABB_H