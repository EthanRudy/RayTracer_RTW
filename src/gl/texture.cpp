// texture.cpp - Implementation of the Texture (OpenGL) class
// Ethan Rudy

#include "../../include/gl/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gl {

	// Constructor
	Texture::Texture(const char* path) {
		this->path = path;

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(true);

		// Load from file
		data = stbi_load(path, &width, &height, &channels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// Free memory
		stbi_image_free(data);
	}

	// Bind Texture
	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	// Reload Texture
	void Texture::reload() {
		stbi_set_flip_vertically_on_load(true);
		
		// Load from file
		data = stbi_load(path, &width, &height, &channels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// Free memory
		stbi_image_free(data);
	}
}

