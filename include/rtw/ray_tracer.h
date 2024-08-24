// ray_tracer.h - Declaration of the RayTracer class
// Ethan Rudy

#ifndef RAY_TRACER_H
#define RAY_TRACER_H

// Standard Header(s)
#include <algorithm>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <iomanip>

// Ray Tracing Header(s)
#include "../../include/rtw/consts.hpp"
#include "../../include/rtw/camera.h"
#include "../../include/rtw/hittable.hpp"
#include "../../include/rtw/hittable_list.h"
#include "../../include/rtw/sphere.hpp"
#include "../../include/rtw/material.hpp"
#include "../../include/rtw/bvh.h"


// "Ray Tracing in One Weekend" namespace
namespace rtw {

	/**
	* Ray Tracer object
	* 
	* Handles main level calls
	*/
	class RayTracer {
	public:

		/**
		* Constructor
		* 
		* @param w	Width
		* @param h	Height
		*/
		RayTracer(unsigned w, unsigned h);

		/**
		* Render
		*/
		void render();

		/**
		* Write
		*/
		void write() const;

		/**
		* Get Progress (bar)
		*/
		void get_progress() const;

		/**
		* Done
		* 
		* @return Whether all the subspan threads are complete
		*/
		bool done() const;

	private:
		// Image details
		unsigned WIDTH, HEIGHT;
		unsigned char* output_data;

		// Number of subspan threads
		int N_THREADS;

		// Pixel spans and progress
		std::vector<coord> pixels;
		int so_far, total_pixels;
		bool _done;

		// Camera and master object list
		Camera camera;
		HittableList world;
	};
}

#endif // !RAY_TRACER_H