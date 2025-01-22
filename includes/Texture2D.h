#pragma once

#include <glm/glm.hpp>
#include <filesystem>

class Texture2D
{
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual unsigned int getWidth() = 0;
	virtual unsigned int getHeight() = 0;
private:
	unsigned int width;
	unsigned int height;
};