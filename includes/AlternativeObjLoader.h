#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "IndexedGeometry.h"

class OBJIndexedGeometry : public IndexedGeometry {
public:
    OBJIndexedGeometry(const std::filesystem::path& path) : objPath(path) {
        generateGeometry();
        uploadData();
    }

protected:
    void drawOnly() override {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    
    void generateGeometry() override {
        std::ifstream file(objPath);
        if(!file.is_open()) {
            std::cerr << "Failed to open file: " << objPath << std::endl;
            return;
        }
        std::vector<glm::vec3> tempVerts;
        std::vector<glm::vec3> tempNorms;
        std::vector<glm::vec2> tempUVs;

        std::string line;
        while(std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if(prefix == "v") {
                glm::vec3 v; iss >> v.x >> v.y >> v.z;
                tempVerts.push_back(v);
            } else if(prefix == "vt") {
                glm::vec2 uv; iss >> uv.x >> uv.y;
                tempUVs.push_back(uv);
            } else if(prefix == "vn") {
                glm::vec3 n; iss >> n.x >> n.y >> n.z;
                tempNorms.push_back(n);
            } else if(prefix == "f") {
                for(int i = 0; i < 3; i++) {
                    std::string vertexData; iss >> vertexData;
                    int vIdx = 0, tIdx = 0, nIdx = 0;
                    sscanf(vertexData.c_str(), "%d/%d/%d", &vIdx, &tIdx, &nIdx);
                    vertices.push_back(tempVerts[vIdx - 1]);
                    if(tIdx > 0) { uvs.push_back(tempUVs[tIdx - 1]); } 
                    else { uvs.push_back({0.f, 0.f}); }
                    if(nIdx > 0) { normals.push_back(tempNorms[nIdx - 1]); } 
                    else { normals.push_back({0.f,0.f,0.f}); }
                    indices.push_back(static_cast<unsigned int>(indices.size()));
                }
            }
        }
    }

private:
    std::filesystem::path objPath;
};