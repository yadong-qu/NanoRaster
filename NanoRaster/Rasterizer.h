#pragma once

#include <glm/glm.hpp>
class Application;

class Rasterizer
{
public:
    Rasterizer();

    void init(Application* a) {
        app = a;
    }

    void rasterTriangle(glm::fvec4* vertices);

    glm::fvec3 barycentric(glm::fvec2* pts, glm::fvec2 p);

    Application* app;
};
