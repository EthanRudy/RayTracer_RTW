// hittable_list.h - Declaration of the HittableList class
// Ethan Rudy

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "consts.hpp"
#include "hittable.hpp"
#include "aabb.h"
#include <vector>

namespace rtw {

    /**
    * Hittable List class
    * 
    * Subclass of Hittable
    */
    class HittableList : public Hittable {
    public:

        // List of hittable object pointers
        std::vector<std::shared_ptr<Hittable>> objects;

        /**
        * Default Constructor
        */
        HittableList();

        /**
        * Start Object Constructor
        * 
        * @param object     Pointer to a Hittable object
        */
        HittableList(std::shared_ptr<Hittable> object);

        /**
        * Clear
        * Straightforward, clears the object vector
        */
        void clear();

        /**
        * Add Hittable
        * 
        * @param object Pointer to a Hittable object
        */
        void add(std::shared_ptr<Hittable> object);

        /**
        * Hit
        * 
        * @returns Whether a ray hits ANY of the contained objects
        */
        bool hit(const ray& r, Interval ray_t, hit_record& rec) const override;

        /**
        * Bounding box
        * 
        * @return The bounding box of the entire Hittable List
        */
        aabb bounding_box() const override;

    private:
        aabb bbox;

    };

}

#endif // !HITTABLE_LIST_H