// consts.hpp - 'Constants' / Helper function file
// Ethan Rudy

#ifndef CONSTS_HPP
#define CONSTS_HPP

// Standard Header(s)
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

namespace rtw {

	const double INF = std::numeric_limits<double>::infinity();
	const double PI = 3.1415926535897932385;

	/**
	* Degress to Radians
	* 
	* @param deg	Degrees to convert
	*/
	inline double deg_to_rad(double deg) {
		return deg * PI / 180.0;
	}

	/**
	* Random Double
	* 
	* @return a random double in the range (0, 1.0)
	*/
	inline double random_double() {
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}

	/**
	* Random Double (Ranged)
	* 
	* @param min	Minimum Value
	* @param max	Maximum value
	*/
	inline double random_double(double min, double max) {
		return min + (max - min) * random_double();
	}

	/**
	* Random Integer
	* 
	* @param min	Minimum Value
	* @param max	Maximum Value
	*/
	inline int random_int(int min, int max) {
		return int(random_double(min, max + 1));
	}

	/**
	* Coord Structue
	* Used only in thread's span/pixel designation
	*/
	struct coord {
		int x, y;

		coord(int X, int Y) { x = X; y = Y; }
	};
}


#endif // !CONSTS_HPP