#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glad/glad.h>

using std::filesystem::path;

//TODO make this not horrible (error handling)

class OBJParser {
public:
    OBJParser(path filePath) : filePath(filePath) {
    }

    void load() {
        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec2> tempUvs;
        std::vector<glm::vec3> tempNormals;

        std::ifstream file(filePath); // Open the OBJ file
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream stream(line);
            char start;
            stream >> start;

            if (start == 'v') {
                char type = stream.peek(); 
                if (type == 't') {
                    stream.get();
                    float u, v, w = 0.0f;
                    stream >> u >> v >> w;
                    tempUvs.push_back(glm::vec2(u, v));
                } else if (type == 'n') {
                    stream.get();
                    float nx, ny, nz;
                    stream >> nx >> ny >> nz;
                    tempNormals.push_back(glm::vec3(nx, ny, nz));
                } else {
                    float x, y, z, w = 1.0f;
                    stream >> x >> y >> z >> w;
                    tempVertices.push_back(glm::vec3(x, y, z));
                }
            } else if (start == 'f') {
                stream.get();
                handleFaceNonIndexed(stream, tempVertices, tempUvs, tempNormals);
            }
        }
    }

private:
    void handleFaceNonIndexed(std::istringstream& stream, std::vector<glm::vec3>& tempVertices, std::vector<glm::vec2>& tempUvs, std::vector<glm::vec3>& tempNormals) {
        std::string faceSegment;
        std::vector<glm::vec3> faceVertices;
        std::vector<glm::vec2> faceUvs;
        while(getline(stream, faceSegment, ' ')){
            unsigned int indices[3];
            int i = 0;
            std::istringstream faceStream(faceSegment);
            std::string vertex;
            while(getline(faceStream, vertex, '/')){
                int index = std::stoi(vertex);
                if (index < 0) {
                    index = tempVertices.size() + index;
                } else {
                    index -= 1;
                }
                indices[i++] = index;
            }

            vertices.push_back(tempVertices[indices[0]]);
            faceVertices.push_back(tempVertices[indices[0]]);
            uvs.push_back(tempUvs[indices[1]]);
            faceUvs.push_back(tempUvs[indices[1]]);
            normals.push_back(tempNormals[indices[2]]);
        }

        glm::vec3 edge1 = faceVertices[1] - faceVertices[0];
        glm::vec3 edge2 = faceVertices[2] - faceVertices[0];
        glm::vec2 deltaUV1 = faceUvs[1] - faceUvs[0];
        glm::vec2 deltaUV2 = faceUvs[2] - faceUvs[0];

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
    }

public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<GLuint> indices;

private:
    path filePath;
};