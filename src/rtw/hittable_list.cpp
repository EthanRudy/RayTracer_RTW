// hittable_list.cpp - Implementation of the HittableList class

#include "../../include/rtw/hittable_list.h"


namespace rtw {

    // Default Constructor
    HittableList::HittableList() {}
    
    // Starter Object Constructor
    HittableList::HittableList(std::shared_ptr<Hittable> object) { add(object); }

    // Clear
    void HittableList::clear() { objects.clear(); }

    // Add Hittable
    void HittableList::add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    // Hit
    bool HittableList::hit(const ray& r, Interval ray_t, hit_record& rec) const {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        // Loop over Hittables
        for (const auto& object : objects) {
            
            // Check if anything was hit
            if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    // Bounding Box
    aabb HittableList::bounding_box() const { return bbox; }
}