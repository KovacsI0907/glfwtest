#pragma once

#include "NonIndexedGeometry.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <OBJParser.h>

namespace fs = std::filesystem;

class NonIndexedObj : public NonIndexedGeometry {
public:
    NonIndexedObj(const fs::path& filePath) : filePath(filePath) {
        generateGeometry();
    }

    void load() {
        OBJParser parser(filePath);
        parser.load();
        vertices = parser.vertices;
        normals = parser.normals;
        uvs = parser.uvs;
        tangents = parser.tangents;
    }

protected:
    void drawOnly() override {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    void generateGeometry() override {
        load();
        uploadData();
    }
private:
    fs::path filePath;
};