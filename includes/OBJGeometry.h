#pragma once
#include "IndexedGeometry.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>

class OBJGeometry : public IndexedGeometry {
public:
    OBJGeometry(const std::filesystem::path &path) {
        parseObjFile(path);
        uploadData();
    }

protected:
    void generateGeometry() override {
        // Not used; data populated by parseObjFile
    }

    void drawOnly() override {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    }

private:
    struct VertexKey {
        unsigned v, t, n;
        bool operator==(const VertexKey &o) const {
            return (v == o.v && t == o.t && n == o.n);
        }
    };

    struct VertexKeyHash {
        std::size_t operator()(const VertexKey &k) const {
            // simple combination hash
            return std::hash<unsigned>()(k.v) ^
                   (std::hash<unsigned>()(k.t) << 1) ^
                   (std::hash<unsigned>()(k.n) << 2);
        }
    };

    void parseObjFile(const std::filesystem::path &path) {
        std::ifstream file(path);
        if (!file.is_open()) return;

        std::vector<glm::vec3> tempPositions;
        std::vector<glm::vec2> tempUVs;
        std::vector<glm::vec3> tempNormals;
        std::unordered_map<VertexKey, unsigned, VertexKeyHash> uniqueMap;

        std::string line;
        while (std::getline(file, line)) {
            if (line.rfind("v ", 0) == 0) {
                std::istringstream ss(line.substr(2));
                glm::vec3 pos;
                ss >> pos.x >> pos.y >> pos.z;
                tempPositions.push_back(pos);
            } else if (line.rfind("vt ", 0) == 0) {
                std::istringstream ss(line.substr(3));
                glm::vec2 uv;
                ss >> uv.x >> uv.y;
                tempUVs.push_back(uv);
            } else if (line.rfind("vn ", 0) == 0) {
                std::istringstream ss(line.substr(3));
                glm::vec3 norm;
                ss >> norm.x >> norm.y >> norm.z;
                tempNormals.push_back(norm);
            } else if (line.rfind("f ", 0) == 0) {
                std::istringstream ss(line.substr(2));
                // parse 3 vertices
                for(int i=0; i<3; i++) {
                    std::string group;
                    ss >> group; // e.g. "1/1/1"
                    unsigned vID=0, tID=0, nID=0;
                    char slash;
                    std::stringstream vs(group);
                    vs >> vID >> slash >> tID >> slash >> nID; 
                    // Wavefront indices are 1-based
                    VertexKey key{vID-1, tID-1, nID-1};
                    if (!uniqueMap.count(key)) {
                        uniqueMap[key] = static_cast<unsigned>(vertices.size());
                        vertices.push_back(tempPositions[key.v]);
                        uvs.push_back((key.t < tempUVs.size()) ? tempUVs[key.t] : glm::vec2(0));
                        normals.push_back((key.n < tempNormals.size()) ? tempNormals[key.n] : glm::vec3(0,0,1));
                    }
                    indices.push_back(uniqueMap[key]);
                }
            }
        }
        file.close();
    }
};