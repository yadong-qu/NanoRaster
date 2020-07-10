#pragma once
#include <algorithm>
#include "TGAImage.h"
#include "Model.h"

struct IShader {
    
    virtual glm::fvec4 vertex(const glm::fvec3& vertice, int index) = 0;
    virtual bool fragment(const glm::fvec3& bar, glm::fvec4& color) = 0;
};

struct Material {
    glm::fvec3 Kd;
    glm::fvec3 Ka;
    glm::fvec3 ks;
    float shininess;
};

struct Light {
    glm::fvec3 pos;
    glm::fvec3 Ld;
    glm::fvec3 La;
    glm::fvec3 Ls;
};

class GouraudShader : public IShader {
public:
    glm::fvec3 varyingIntensity;
    
    glm::mat4 mvp;
    glm::mat3 normalMatrix;
    glm::mat4 modelViewMatrix;
    Model* model;
    glm::fvec3 cameraPos;
    // Material material;
    Light light;

    glm::fvec3 normals[3];
    glm::fvec3 viewCoords[3];
    glm::fvec2 texCoords[3];

    virtual glm::fvec4 vertex(const glm::fvec3& vertice,  int index) override {
        normals[index] = glm::normalize(normalMatrix * normals[index]);
        viewCoords[index] = glm::fvec3(modelViewMatrix * glm::fvec4(vertice, 1.0));
        return mvp * glm::fvec4(vertice, 1.0);
    }

    virtual bool fragment(const glm::fvec3& bar, glm::fvec4& color) override {
        glm::fvec2 texCoord = (texCoords[0] * (float)bar[0] + texCoords[1] * (float)bar[1] + texCoords[2] * (float)bar[2]);
        // glm::fvec3 ambient = light.La * material.Ka;
        glm::fvec3 viewCoord = viewCoords[0] * bar[0] + viewCoords[1] * bar[1] + viewCoords[2] * bar[2];
        glm::fvec3 normal = normals[0] * bar[0] + normals[1] * bar[1] + normals[2] * bar[2];
        
        glm::fvec3 s = glm::normalize(light.pos-viewCoord);
        float sDotN = std::max(glm::dot(s, normal), 0.0f);
       
        glm::fvec4 diffuse = model->diffuse(texCoord).toVec4() * sDotN;
        glm::fvec4 spec = glm::fvec4(0.0, 0.0, 0.0, 1.0);
      
        if (sDotN > 0.0) {
            glm::fvec3 v = glm::normalize(cameraPos - viewCoord);
            glm::fvec3 h = glm::normalize(v + s);
            spec = model->diffuse(texCoord).toVec4() * pow(std::max(dot(h, normal), 0.0f), 128);
        }

        color = diffuse + spec;
        
        return false;
    }
};

