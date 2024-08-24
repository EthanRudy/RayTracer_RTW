// sphere.hpp - Declaration & Implementation of the Sphere class
// Ethan Rudy

#ifndef SHPERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "consts.hpp"
#include "vec3.hpp"
#include "ray.h"
#include "interval.h"
#include "material.hpp"

namespace rtw {

	/**
	* Sphere class
	* Subclass of Hittable
	* 
	* Mathematicall represented with a position and radius
	* and of a load of helpers
	*/
	class Sphere : public Hittable {
	public:

		/**
		* Stationary Sphere
		* 
		* @param center		Center of the sphere
		* @param radius		Radius of the sphere
		* @param mat		Material of the sphere
		*/
		Sphere(const point3& center, double radius, std::shared_ptr<material> mat)
			: center1(center), radius(std::fmax(0, radius)), mat(mat), is_moving(false) {
		
			auto rvec = vec3(radius, radius, radius);
			bbox = aabb(center1 - rvec, center1 + rvec);
		}

		/**
		* Moving Sphere
		* 
		* @param center1	Starting center
		* @param center2	Ending center
		* @param radius		Radius
		* @param mat		Material of the sphere
		*/
		Sphere(const point3& center1, const point3& center2, double radius, std::shared_ptr<material> mat)
			: center1(center1), radius(std::fmax(0, radius)), mat(mat), is_moving(true) {

			auto rvec = vec3(radius, radius, radius);
			aabb box1(center1 - rvec, center1 + rvec);
			aabb box2(center2 - rvec, center2 + rvec);
			bbox = aabb(box1, box2);

			center_vec = center2 - center1;
		}

		/**
		* Hit
		* 
		* @param r		Ray
		* @param ray_t	Ray (time) Interval
		* @param rec	Hit Record of the sphere
		*/
		bool hit(const ray& r, Interval ray_t, hit_record& rec) const override {
			// Center calculation based of movement
			point3 center = is_moving ? sphere_center(r.time()) : center1;

			// Most the needed calculations for if a point lies within/on the surface of a sphere
			vec3 oc = center - r.origin();
			auto a = r.direction().length_squared();
			auto h = dot(r.direction(), oc);
			auto c = oc.length_squared() - radius * radius;
			auto discriminant = h * h - a * c;

			if (discriminant < 0) { return false; }

			auto sqrtd = std::sqrt(discriminant);

			auto root = (h - sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				root = (h + sqrtd) / a;
				if (!ray_t.surrounds(root)) {
					return false;
				}
			}

			// Hit Record settings
			rec.t = root;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.mat = mat;

			return true;
		}

		/**
		* Bounding Box
		* 
		* @return The containing Bounding Box
		*/
		aabb bounding_box() const override { return bbox; }

	private:
		point3 center1;
		double radius;
		std::shared_ptr<material> mat;
		bool is_moving;
		vec3 center_vec;
		aabb bbox;

		/**
		* Sphere Center Helper
		* Finds the center of a moving sphere based on the given time
		* time == 0 -> center 1
		* time == 1 -> center 2
		* else somewhere in the middle
		* 
		* @param time Time along difference
		*/
		point3 sphere_center(double time) const {
			return center1 + time * center_vec;
		}
	};

}

#endif // !SPHERE_HPP