#pragma once

#include <glm/glm.hpp>
#include <filesystem>

class Texture2D
{
public:
	virtual ~Texture2D() = default;
public:
	virtual unsigned int getWidth() = 0;
	virtual unsigned int getHeight() = 0;
};