// hittable.h - Declaration & Implementation of the hit_record and Hittable classes
// Ethan Rudy

#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "consts.hpp"
#include "ray.h"
#include "interval.h"
#include "aabb.h"

namespace rtw {

    // Funky 'declaration?' of the material class (see material.hpp)
    // Its in the book, and I think it's because hit_record relies on
    // materials, but hit_record(s) are passed into material functions
    // circular definition but not really
    class material;

    /**
    * Hit Record class
    * Keeps track of what material was hit as the ray bounces
    */
    class hit_record {
    public:
        point3 p;
        vec3 normal;
        std::shared_ptr<material> mat;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };


	// Abstract "hittable" object class
	class Hittable {
	public:
		virtual ~Hittable() = default;

        // Returns whether or not a ray makes contact with the Hittable object
		virtual bool hit(const ray& r, Interval ray_t, hit_record& rec) const = 0;
	
        // Returns the bounding box of the Hittable object
        virtual aabb bounding_box() const = 0;
    };
}

#endif // !HITTABLE_HPP