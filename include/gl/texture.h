// texture.h - Declaration of the Texture (OpenGL) class
// Ethan Rudy

#ifndef TEXTURE_H
#define TEXTURE_H

// Graphics Header(s)
#include <glad/glad.h>

// Standard Header(s)
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Custom OpenGL related namespace
namespace gl {

	/**
	* Texture Classs
	* 
	* Takes a img filepath as input
	* and extracts it's pixel data
	*/
	class Texture {
	public:
		// Texture ID
		unsigned ID;

		/**
		* Constructor
		* 
		* @param path	Texture's image path
		*/
		Texture(const char* path);

		/**
		* Bind Texture
		*/
		void bind();

		/**
		* Reload Texture
		*/
		void reload();

	private:

		unsigned char* data;
		int width, height, channels;
		const char* path;
	};

}

#endif // !TEXTURE_H