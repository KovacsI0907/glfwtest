#pragma once

#include <glm/glm.hpp>
#include <filesystem>
#include "GPUTexture2D.h"
#include "CubeMapTexture.h"
#include "glad/glad.h"
#include <exception>
#include <vector>

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

class ImageCubeMapTexture : public CubeMapTexture {
public:
	ImageCubeMapTexture(const std::vector<std::filesystem::path> imagePaths);

	void load();
private:
	std::vector<std::filesystem::path> paths;
};