#pragma once

#include <glm/glm.hpp>
#include <filesystem>
#include "Texture2D.h"
#include "glad/glad.h"
#include <exception>

class ImageLoadException : public std::runtime_error {
public:
	ImageLoadException(const std::string& message) : std::runtime_error(message) {}
};

class ImageTexture2D : public Texture2D
{
public:
	ImageTexture2D(std::filesystem::path path);

	void load();

	void bind() override;

	void unbind() override;

	unsigned int getWidth() override;

	unsigned int getHeight() override;

	GLuint getID();

	~ImageTexture2D();

private:
	GLuint textureID;
	int width;
	int height;
	bool loaded = false;
	std::filesystem::path path;
};