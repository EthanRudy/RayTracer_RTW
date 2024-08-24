// camera.cpp - Implementation of the Camera class
// Ethan Rudy

#include "../../include/rtw/camera.h"

namespace rtw {

	// Dimensional Constructor
	Camera::Camera(int width, int height) {
		//init(width, height);
		image_width = width;
		image_height = height;
	}

	// Render Span (threaded)
	void Camera::render_span(HittableList& world, int s_start, int s_end, unsigned char* output, std::vector<coord>& pixels, int& n_pixels) {
		// Loop over span
		for (int coordIndex = s_start; coordIndex < s_end; ++coordIndex) {
			int x = pixels[coordIndex].x, y = pixels[coordIndex].y;

			// Sample ray color
			color pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples; sample++) {
				ray r = get_ray(x, y);
				pixel_color += ray_color(r, max_depth, world);
			}
			// Scale with weighting
			pixel_color *= sample_scale;

			// Gamma correction
			auto r = pixel_color.x();
			auto g = pixel_color.y();
			auto b = pixel_color.z();
			r = linear_to_gamma(r);
			g = linear_to_gamma(g);
			b = linear_to_gamma(b);

			// 0 - 255 Clamping * writing to output
			// This is where color.hpp's write color would
			// normally be used
			static const Interval intensity(0.000, 0.999);
			output[3 * (y * image_width + x) + 0] = int(intensity.clamp(r) * 256);
			output[3 * (y * image_width + x) + 1] = int(intensity.clamp(g) * 256);
			output[3 * (y * image_width + x) + 2] = int(intensity.clamp(b) * 256);

			// Increment number of pixels completed (for the progress bar)
			++n_pixels;
		}
	}

	// Initialize
	void Camera::init() {
		// Calculate Scaling
		sample_scale = 1.0 / samples;

		// Camera pos
		center = lookfrom;

		// FOV, DOF calculations
		auto theta = deg_to_rad(vfov);
		auto h = std::tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

		// Offsets
		auto viewport_u = viewport_width * u;
		auto viewport_v = viewport_height * -v;
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// more DOF
		auto defocus_radius = focus_dist * std::tan(deg_to_rad(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	// Ray Color
	color Camera::ray_color(const ray& r, int depth, const Hittable& world) const {
		// Base Case
		if (depth <= 0) { return color(0, 0, 0); }

		hit_record rec;
		if (world.hit(r, Interval(0.001, INF), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered)) {
				// Color weighting and recursive call
				return attenuation * ray_color(scattered, depth - 1, world);
			}
			// No material == void
			return color(0, 0, 0);
		}

		// Background 'sky' fade
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	// Get Ray
	ray Camera::get_ray(int x, int y) const {
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
			+ ((x + offset.x()) * pixel_delta_u)
			+ ((y + offset.y()) * pixel_delta_v);

		auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;
		auto ray_time = random_double();

		return ray(ray_origin, ray_direction, ray_time);
	}

	// Sample Square
	vec3 Camera::sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	// Defocus Disk Sample
	point3 Camera::defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	// Linear to Gamma
	double Camera::linear_to_gamma(double linear_component) {
		if (linear_component > 0) {
			return std::sqrt(linear_component);
		}

		return 0;
	}
}