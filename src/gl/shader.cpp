// shader.cpp - Implementation of the Shader class
// Ethan Rudy

#include "../../include/gl/shader.h"

namespace gl {
	
	// Constructor
	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Load from respective files
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// Convert to char pointers
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// Shader creation and compilation
		unsigned int vertex, fragment;
		// Error helpers
		int success;
		char infoLog[512];

		// Vertex
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Fragment
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Linked Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Clean up
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Use Program
	void Shader::use() {
		glUseProgram(ID);
	}

	// Set Uniform (Boolean)
	void Shader::setBool(const char* name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name), (bool)value);
	}

	// Set Uniform (Integer)
	void Shader::setInt(const char* name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name), (int)value);
	}

	// Set Uniform (Float)
	void Shader::setFloat(const char* name, float value) const {
		glUniform1i(glGetUniformLocation(ID, name), (float)value);
	}

}