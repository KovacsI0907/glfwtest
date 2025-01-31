#pragma once

#include <glm/glm.hpp>
#include <filesystem>
#include "GPUTexture2D.h"
#include "glad/glad.h"
#include <exception>

class ImageLoadException : public std::runtime_error {
public:
	ImageLoadException(const std::string& message) : std::runtime_error(message) {}
};

class ImageTexture2D : public GPUTexture2D
{
public:
	enum Type {
		Color,
		Normal,
		Data
	};

	ImageTexture2D(std::filesystem::path path, Type type = Type::Color);

	void load();

private:
	int width;
	int height;
	std::filesystem::path path;
};