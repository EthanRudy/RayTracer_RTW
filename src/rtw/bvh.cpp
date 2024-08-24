// bvh.cpp - Implementation of the bv_node class
// Ethan Rudy

#include "../../include/rtw/bvh.h"

namespace rtw {

	// List Constructor
	bvh_node::bvh_node(HittableList list) : bvh_node(list.objects, 0, list.objects.size()) {}

	// Vector Constructor
	bvh_node::bvh_node(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
		// Create new bbox
		bbox = aabb::empty;
		
		// Add all selected objects
		for (size_t object_index = start; object_index < end; ++object_index) {
			bbox = aabb(bbox, objects[object_index]->bounding_box());
		}
		
		// Axis to split
		int axis = bbox.longest_axis();

		// Custom comparing function, see std::sort call below
		auto comparator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare
			: box_z_compare;

		size_t object_span = end - start;

		// One object
		if (object_span == 1) {
			left = right = objects[start];
		}
		// Two objects
		else if (object_span == 2) {
			left = objects[start];
			right = objects[start + 1];
		}
		// Three or more objects
		else {
			// Sort the objects by axis
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			// Send the objects to the next level in the hierarchy
			auto mid = start + object_span / 2;
			left = std::make_shared<bvh_node>(objects, start, mid);
			right = std::make_shared<bvh_node>(objects, mid, end);
		}

		// bbox = aabb(left->bounding_box(), right->bounding_box());
	}

	// Hit
	bool bvh_node::hit(const ray& r, Interval ray_t, hit_record& rec) const {
		if (!bbox.hit(r, ray_t)) { return false; }

		bool hit_left = left->hit(r, ray_t, rec);
		bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

		return hit_left || hit_right;
	}

	// Bounding Box
	aabb bvh_node::bounding_box() const {
		return bbox;
	}



	// Master Compare
	bool bvh_node::box_compare(
		const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index
	) {
		auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
		auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
		return a_axis_interval.min < b_axis_interval.min;
	}

	// X Axis Compare
	bool bvh_node::box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return box_compare(a, b, 0);
	}

	// Y Axis Compare
	bool bvh_node::box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return box_compare(a, b, 1);
	}

	// Z Axis Compare
	bool bvh_node::box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return box_compare(a, b, 2);
	}

}