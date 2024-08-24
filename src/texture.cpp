#include "../include/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(const std::string& path) {
    this->path = path;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    stbi_image_free(data);
	
}

void Texture::intParam(int what, int val) {
	glTexParameteri(GL_TEXTURE_2D, what, val);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::reload() {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    stbi_image_free(data);
}