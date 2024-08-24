// ray_tracer.cpp - Implementation of the RayTracer class
// Ethan Rudy

#include "../../include/rtw/ray_tracer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>

using std::make_shared;
using std::shared_ptr;

namespace rtw {

	// Constructor
	RayTracer::RayTracer(unsigned w, unsigned h) {
		WIDTH = w, HEIGHT = h;
		_done = false;

		// Create camera
		camera = Camera(w, h);

		// Allocate memory
		output_data = new unsigned char[3 * WIDTH * HEIGHT];

		// Blackout output
		for (int y = 0; y < HEIGHT; ++y) {
			for (int x = 0; x < WIDTH; ++x) {
				output_data[3 * (y * WIDTH + x) + 0] = 0;
				output_data[3 * (y * WIDTH + x) + 1] = 0;
				output_data[3 * (y * WIDTH + x) + 2] = 0;

				pixels.push_back(coord(x, y));
			}
		}

		/**
		* I wanted to do a threadpool and have it render like cinebench,
		* But my threadpool wasn't happy with me, so I store all pixel
		* (x, y) coordinates, randomize the entire list, and then
		* divide it into spans that each thread renders individually.
		*/

		// Scramble pixels
		auto rng = std::default_random_engine{};
		std::shuffle(pixels.begin(), pixels.end(), rng);

		// Write blackout pixels
		stbi_write_jpg("./textures/output.jpg", WIDTH, HEIGHT, 3, output_data, WIDTH * 3);

		// Get number of "available" threads
		N_THREADS = std::thread::hardware_concurrency();
		// Okay for usability im subracting 2 threads
		// One thread renders the screen, one thread manages the ray tracer, and the (N_THREADS - 2) do the math
		// If I stick with the normal (16 in my case), everything else on my PC gets stuttery bc all cores are firing
		// at this ONE program
		N_THREADS -= 2;

		// Progress indicators
		so_far = 0;
		total_pixels = WIDTH * HEIGHT;




		// WORLD CREATION

		auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
		world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = random_double();
				point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

				if ((center - point3(4, 0.2, 0)).length() > 0.9) {
					shared_ptr<material> sphere_material;

					if (choose_mat < 0.8) {
						// diffuse
						auto albedo = color::random() * color::random();
						sphere_material = make_shared<lambertian>(albedo);
						auto center2 = center + vec3(0, random_double(0, .5), 0);
						world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
					}
					else if (choose_mat < 0.95) {
						// metal
						auto albedo = color::random(0.5, 1);
						auto fuzz = random_double(0, 0.5);
						sphere_material = make_shared<metal>(albedo, fuzz);
						world.add(make_shared<Sphere>(center, 0.2, sphere_material));
					}
					else {
						// glass
						sphere_material = make_shared<dielectric>(1.5);
						world.add(make_shared<Sphere>(center, 0.2, sphere_material));
					}
				}
			}
		}

		auto material1 = make_shared<dielectric>(1.5);
		world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

		auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
		world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

		auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
		world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

		world = HittableList(make_shared<bvh_node>(world));


		// Camera settings
		// 500+ for REALLY GOOD RENDERS
		// ~10 for debugging
		camera.samples = 10;
		camera.max_depth = 50;

		camera.vfov = 20;
		camera.lookfrom = point3(13, 2, 3);
		camera.lookat = point3(0, 0, 0);
		camera.vup = vec3(0, 1, 0);

		camera.defocus_angle = 0.6;
		camera.focus_dist = 10.0;

		// Initialize Camera
		camera.init();
	}

	// Render
	void RayTracer::render() {
		// Create thread vector and calculate span width
		std::vector<std::thread> render_threads;
		int span = pixels.size() / N_THREADS;
		int s_start = 0;

		// Create subspan threads, walking along the span
		for (int i = 0; i < N_THREADS; ++i) {
			render_threads.push_back(std::thread(&Camera::render_span, camera, std::ref(world), s_start, s_start + span, output_data, std::ref(pixels), std::ref(so_far)));
			s_start += span;
		}

		// Join threads
		for (auto& t : render_threads) {
			if (t.joinable()) {
				t.join();
			}
		}

		// Flag the render as complete
		_done = true;
	}

	// Write
	void RayTracer::write() const {
		stbi_write_jpg("./textures/output.jpg", WIDTH, HEIGHT, 3, output_data, WIDTH * 3);
	}

	// Get Progress (bar)
	void RayTracer::get_progress() const {
		int n_chars = 75;
		float progress = ((float)so_far / total_pixels) * n_chars;
		float percent = float(so_far) / total_pixels * 100;

		std::string bar = "\r";

		// If the render is complete, display as such
		if (_done) { 
			percent = 100; 
			bar += "COMPLETE ";
		}

		bar += "[";
		for (int i = 0; i < n_chars; ++i) {
			if (i < progress) { bar += "#"; }
			else { bar += " "; }
		}
		bar += "]";


		std::cout << bar << " " << std::setprecision(4) << " " << percent << "%      ";
	}

	// Done
	bool RayTracer::done() const {
		return _done;
	}

}

