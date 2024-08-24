// shader.h - Declaration of the Shader class
// Ethan Rudy

#ifndef SHADER_H
#define SHADER_H

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
	* Shader Class
	* 
	* Takes a vertex and fragment shader file path as 
	* input and compiles them into one program
	*/
	class Shader {
	public:
		// Program ID
		unsigned ID;

		/**
		* Constructor
		* 
		* @param vertexPath		.vert Filepath
		* @param fragmentPath	.frag Filepath
		*/
		Shader(const char* vertexPath, const char* fragmentPath);

		/**
		* Use Program
		*/
		void use();

		/**
		* Set Uniform (Boolean)
		* 
		* @param Name of Uniform
		* @param New Uniform value
		*/
		void setBool(const char* name, bool value) const;

		/**
		* Set Uniform (Integer)
		*
		* @param Name of Uniform
		* @param New Uniform value
		*/
		void setInt(const char* name, int value) const;

		/**
		* Set Uniform (Float)
		*
		* @param Name of Uniform
		* @param New Uniform value
		*/
		void setFloat(const char* name, float value) const;
	};

}

#endif // !SHADER_H