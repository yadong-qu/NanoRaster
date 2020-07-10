#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TGAImage.h"
class Model
{
public:
    Model() {

    }

    void load(const std::string& filename);

    void load_texture(const std::string& filename, const char* suffix, TGAImage& img);

    TGAColor diffuse(glm::fvec2 uvf);

    glm::fvec3 normal(glm::fvec3 uvf);
    glm::fvec3 normal(int iface, int nthvert) {
        return glm::normalize(normals[faces[iface][nthvert][2]]);
    }

    glm::fvec3 vert(int iface, int nthvert) {
        return vertices[faces[iface][nthvert][0]];
    }

    glm::fvec2 texCoord(int iface, int nthvert) {
        return texCoords[faces[iface][nthvert][1]];
    }
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<std::vector<glm::ivec3>> faces;

    TGAImage diffusemap_;
    TGAImage normalmap_;
};

