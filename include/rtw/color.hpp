// color.hpp - Holds a single, phased out, color function
// Ethan Rudy

#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"

namespace rtw {

	/**
	* Write Color
	* 
	* @param out			Output stream
	* @param pixel_color	Color to be written
	*/
	void write_color(std::ostream& out, const point3& pixel_color) {
		auto r = pixel_color.x();
		auto g = pixel_color.y();
		auto b = pixel_color.z();

		int rbyte = int(255.999 * r);
		int gbyte = int(255.999 * g);
		int bbyte = int(255.999 * b);

		out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
	}
}


#endif // !COLOR_HPP