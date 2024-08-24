// ray.h - Declaration file for the ray class
// Ethan Rudy

#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

namespace rtw {

	/**
	* Ray Class
	* Represents a photon using two vectors and a double
	* for time
	* Start point, direction, and time (along the ray)
	*/
	class ray {
	public:

		/**
		* Default Constructor
		*/
		ray();

		/**
		* Time Constructor
		* 
		* @param origin		Start of the ray
		* @param direction	Direction of the ray
		* @param time		Represents where the photon is along the ray
		*/
		ray(const point3& origin, const vec3& direction, double time);

		/**
		* Timeless Constructor
		* 
		* @param origin		Start of the ray
		* @param direction	Direction of the Ray
		*/
		ray(const point3& origin, const vec3& direction);

		// Accessors
		const point3& origin() const;
		const point3& direction() const;
		double time() const;

		/**
		* Ray at Time 't'
		* 
		* @return Where the simulated ray is at a specified time
		*/
		point3 at(double t) const;

	private:
		point3 orig;
		vec3 dir;
		double tm;
	};

}

#endif // !RAY_H