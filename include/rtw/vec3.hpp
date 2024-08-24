// vec3.hpp - Declaration & Partial Implementation of the vec3 class
// Ethan Rudy

#ifndef VEC3_H
#define VEC3_H

#include "consts.hpp"

namespace rtw {

	// 3D Vector Class
	class vec3 {
	public:
		// Elements
		double e[3];

		/**
		* Default Constructor
		*/
		vec3();

		/**
		* Element Constructor
		* 
		* @parm e0	First (x) component
		* @parm e1	Second (y) component
		* @parm e2	Third (z) component
		*/
		vec3(double e0, double e1, double e2);

		// Accessors
		double x() const;
		double y() const;
		double z() const;

		// Negate Operator
		vec3 operator-() const;

		// Accessors (indexed)
		double operator[](int i) const;
		double& operator[](int i);

		// Addition & Assigment
		vec3& operator+=(const vec3& v);

		// Scalar Multiplication & Assignment
		vec3& operator*=(double t);

		// Scalar Division & Assignment
		vec3& operator/=(double t);

		// Length
		double length() const;

		// Length Squared
		double length_squared() const;

		bool near_zero() const;

		// Random
		static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}

		static vec3 random(double min, double max) {
			return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
		}

	};

	// Alternate Naming conventions, 3d point, and 3 channel rgb color
	using point3 = vec3;
	using color = vec3;

	// Ostream Overload
	inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
		return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
	}

	// Addition Operator Overload
	inline vec3 operator+(const vec3& u, const vec3& v) {
		return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
	}

	// Subtraction Operator Overload
	inline vec3 operator-(const vec3& u, const vec3& v) {
		return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
	}

	// Multiplication overload
	inline vec3 operator*(const vec3& u, const vec3& v) {
		return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
	}

	// Multiplication overload (scalar, type 1)
	inline vec3 operator*(double t, const vec3& v) {
		return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
	}

	// Multiplication overload (scalar, type 2)
	inline vec3 operator*(const vec3& v, double t) {
		return t * v;
	}

	// Division overload
	inline vec3 operator/(const vec3& v, double t) {
		return (1 / t) * v;
	}

	// Dot Product
	inline double dot(const vec3& u, const vec3& v) {
		return u.e[0] * v.e[0]
			+ u.e[1] * v.e[1]
			+ u.e[2] * v.e[2];
	}

	// Cross Product
	inline vec3 cross(const vec3& u, const vec3& v) {
		return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
			u.e[2] * v.e[0] - u.e[0] * v.e[2],
			u.e[0] * v.e[1] - u.e[1] * v.e[0]);
	}

	// Unit Vector
	inline vec3 unit_vector(const vec3& v) {
		return v / v.length();
	}

	// Random Vector in Unit Disk
	// 2D vector, where the z component is 0
	inline vec3 random_in_unit_disk() {
		while (true) {
			auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
			if (p.length_squared() < 1) {
				return p;
			}
		}
	}

	// Random Vector in Unit Sphere
	inline vec3 random_in_unit_shpere() {
		while (true) {
			auto p = vec3::random(-1, 1);
			if (p.length_squared() < 1) {
				return p;
			}
		}
	}

	// Random unit Vector
	inline vec3 random_unit_vector() {
		return unit_vector(random_in_unit_shpere());
	}

	// Random Vector on Hemisphere
	inline vec3 random_on_hemisphere(const vec3& normal) {
		vec3 on_unit_sphere = random_unit_vector();
		if (dot(on_unit_sphere, normal) > 0.0) {
			return on_unit_sphere;
		}
		else {
			return -on_unit_sphere;
		}
	}

	// Reflect Vector
	inline vec3 reflect(const vec3& v, const vec3& n) {
		return v - 2 * dot(v, n) * n;
	}

	// Refract Vector
	inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
		auto cos_theta = std::fmin(dot(-uv, n), 1.0);
		vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
		vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
		return r_out_perp + r_out_parallel;
	}

}

#endif // !VEC3_H