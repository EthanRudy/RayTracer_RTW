// material.hpp - Declaration & Implementation of the material class
// Ethan Rudy

#ifndef MATERIAL_HPP
#define MATERIAL_HPP


namespace rtw {

    // Same funky declaration, see hittable.hpp for what I mean
    class hit_record;

    /**
    * Abstract Material class
    */
    class material {
    public:
        virtual ~material() = default;

        /**
        * Scatter
        * Scatter's the ray given the material's properties
        * 
        * @param r_in           Ray Input (ref)
        * @param rec            Hit Record (ref)
        * @param attenuation    Color (ref)
        * @param scattered      Ray Output (ref)
        */
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            return false;
        }
    };

    /**
    * Lambertian Material
    * Simplest Material. Think perfectly smooth (not shiny) plastics
    * Blender default cube type shit
    * 
    * Subclass of material
    */
    class lambertian : public material {
    public:
        /**
        * Albedo Constructor
        * 
        * @param albedo "Color"
        */
        lambertian(const color& albedo) : albedo(albedo) {}

        /**
        * Scatter
        * Scatter's the ray given the material's properties
        *
        * @param r_in           Ray Input (ref)
        * @param rec            Hit Record (ref)
        * @param attenuation    Color (ref)
        * @param scattered      Ray Output (ref)
        */
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
            const override {
            
            // Simplest bounce
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction, r_in.time());
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
    };

    /**
    * Metal Material
    * Subclass of material
    */
    class metal : public material {
    public:
        /**
        * Parameter Constructor
        * 
        * @param albedo     "color" of the metal
        * @param fuzz       fuzz factor of the metal, ex matte, semi-gloss, full-gloss
        *                   range = [0, 1.0] -> [no fuzz, full fuzz]
        */
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        /**
        * Scatter
        * Scatter's the ray given the material's properties
        *
        * @param r_in           Ray Input (ref)
        * @param rec            Hit Record (ref)
        * @param attenuation    Color (ref)
        * @param scattered      Ray Output (ref)
        */
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
            const override {
            
            // REFLECT
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

            // Scatter the reflected
            scattered = ray(rec.p, reflected, r_in.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
    };

    /**
    * Dialectric Material
    * Subclass of material
    * 
    * Glass, Crystals, TRANSPARENT, not translucent
    */
    class dielectric : public material {
    public:
        
        /**
        * Parameter Constructor
        * 
        * @param refraction_index   Index of Refraction. One can find them online for glass, diamond, etc
        */
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        /**
        * Scatter
        * Scatter's the ray given the material's properties
        *
        * @param r_in           Ray Input (ref)
        * @param rec            Hit Record (ref)
        * @param attenuation    Color (ref)
        * @param scattered      Ray Output (ref)
        */
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
            const override {

            // ALWAYS clear/white, idk if I want to add tinting :D
            // Just sitting here, you can change this value and all the glass
            // will inherit said color
            attenuation = color(1, 1, 1);


            // Alrighty theres more here than refraction
            // When one looks at glass, at some certain angle
            // you actually stop looking through the glass
            // and see reflections.
            // So we refract to a certain angle, and then reflect
            // We ofc use an approximation bc I don't have all day

            // Refract/Reflect calculation
            double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;
            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            // Cuttof
            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
                // Reflect
                direction = reflect(unit_direction, rec.normal);
            }
            else {
                // Refract
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction, r_in.time());
            return true;
        }

    private:
        double refraction_index;

        /**
        * Reflectance
        * 
        * @param cosine
        * @param refraction_index
        * 
        * @return How much the material reflects based off ri and angle
        */
        static double reflectance(double cosine, double refraction_index) {
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 *= r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }
    };
}

#endif // !MATERIAL_HPP