// bvh.h - Declaration of the bvh_node class
// Ethan Rudy

#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.hpp"
#include "hittable_list.h"
#include <algorithm>

namespace rtw {

	/**
	* Bounding Volume Hierarchy (BVH) class
	* Tree like structure of bounding boxes
	* for optimizing ray collisions
	* 
	* Subclass of Hittable
	*/
	class bvh_node : public Hittable {
	public:

		/**
		* List Constructor
		* 
		* @param list	HittableList object
		*/
		bvh_node(HittableList list);

		/**
		* Vector Constructor
		* 
		* @parm objects		Vector of Hittable object pointers
		* @param start		Start of the selected range
		* @param end		End of the selected range
		*/
		bvh_node(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);

		/**
		* Hit
		* 
		* @param r		Ray
		* @param ray_t	Interval (time) of ray r
		* @param rec	Hit Record
		*/
		bool hit(const ray& r, Interval ray_t, hit_record& rec) const override;

		/**
		* Bound Box
		* 
		* @return Whether the node was hit
		*/
		aabb bounding_box() const override;

	private:

		std::shared_ptr<Hittable> left;
		std::shared_ptr<Hittable> right;
		aabb bbox;



		// Comparators
        static bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index);
		static bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
		static bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
		static bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
	};


}

#endif // !BVH_H
