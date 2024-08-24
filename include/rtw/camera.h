// camera.h - Declaration of the Camera class
// Ethan Rudy

#ifndef CAMERA_H
#define CAMERA_H

#include "consts.hpp"
#include "hittable_list.h"
#include "vec3.hpp"
#include "ray.h"
#include "interval.h"
#include "hittable.hpp"
#include "material.hpp"

namespace rtw {
	
	/**
	* Camera class
	* Customizable parameters!
	*/
	class Camera {
	public:

		// Ray Options
		int samples;
		int max_depth;

		// Position and FOV
		double  vfov = 90;
		point3 lookfrom = point3(0, 0, 0);
		point3 lookat = point3(0, 0, -1);
		vec3 vup = vec3(0, 1, 0);

		// "Depth of Field"
		double defocus_angle = 0;
		double focus_dist = 10;

		/**
		* Default Constructor
		*/
		Camera() = default;

		/**
		* Dimensional Constructor
		*/
		Camera(int width, int height);

		/**
		* Render Span (threaded)
		* 
		* @param world		Object List
		* @param s_start	Span start index
		* @param s_end		Span ending index
		* @param output		Pixel data output
		* @param pixels		Randomized span pixel list
		* @param n_pixels	Number of completed pixels
		*/
		void render_span(HittableList& world, int s_start, int s_end, unsigned char* output, std::vector<coord>& pixels, int& n_pixels);
	
		/**
		* Initialize
		* 
		* One time object setup, run after constructor, after setting public parameters,
		* but before rendering
		*/
		void init();

	private:
		// Image dimensions
		int image_height, image_width;

		// Weighting of samples
		double sample_scale;
		
		point3 center;         // Camera center
		point3 pixel00_loc;    // Location of pixel (0, 0)
		vec3   pixel_delta_u;  // Offset to pixel to the right
		vec3   pixel_delta_v;  // Offset to pixel below

		// DOF
		vec3 u, v, w;
		vec3 defocus_disk_u;
		vec3 defocus_disk_v;
		
		/**
		* Ray Color
		* Finds color of ray r
		* 
		* @param r		Ray
		* @param depth	Depth of ray r
		* @param world	Hittable to check against
		*/
		color ray_color(const ray& r, int depth, const Hittable& world) const;

		/**
		* Get Ray
		* Creates a ray given (x, y) pixel coords and calculated offsets
		* 
		* @param x
		* @param y
		*/
		ray get_ray(int x, int y) const;

		/**
		* Sample Square
		* 
		* @return a random vec3 in the sample square
		*/
		vec3 sample_square() const;

		/**
		* Defocus Disk Sample
		* 
		* @return Random point on defocus disk
		*/
		point3 defocus_disk_sample() const;

		/**
		* Linear to Gamma
		* Gamma Correction
		* 
		* @param linear_component
		*/
		double linear_to_gamma(double linear_component);

	};
}

#endif // !CAMERA_H
