#include "Rasterizer.h"
#include "Application.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>
Rasterizer::Rasterizer()
{

}


void Rasterizer::rasterTriangle(glm::fvec4* vertices) {
    
    //TODO: clipping

    


    
    
    // conversion to screen space
    for (int n = 0; n < 3; n++) {
       
        vertices[n].x /= vertices[n].w;
        vertices[n].y /= vertices[n].w;
        vertices[n].z /= vertices[n].w;
        vertices[n].x = (vertices[n].x + 1) / 2 * app->getWidth();
        vertices[n].y = (1 - vertices[n].y) / 2 * app->getHeight();
    }
    
    glm::fvec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::fvec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    glm::fvec2 clamp(app->getWidth() - 1, app->getHeight() - 1);
    glm::fvec2 triangles[3];
    for (int i = 0; i < 3; i++) {
        triangles[i] = glm::fvec2(vertices[i].x, vertices[i].y);
    }

    // back face culling
    glm::fvec3 v1 = glm::fvec3(vertices[0]);
    glm::fvec3 v2 = glm::fvec3(vertices[1]);
    glm::fvec3 v3 = glm::fvec3(vertices[2]);

    glm::fvec3 facenormal = glm::cross(v2 - v1, v3-v1);
    if (facenormal.z > 0) {
        return;
    }

    for (int i = 0; i < 3; i++) {
        bboxmin.x = std::max(0.f, std::min(bboxmin.x, triangles[i].x));
        bboxmin.y = std::max(0.f, std::min(bboxmin.y, triangles[i].y));
        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, triangles[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, triangles[i].y));
    }
    glm::ivec2 p;
    glm::fvec4 color;
    for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++) {
        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++) {
            glm::dvec3 bc_screen = barycentric(triangles, p);

            if (bc_screen.x < 0 || bc_screen.x > 1 || bc_screen.y < 0 || bc_screen.y > 1
                || bc_screen.z < 0 || bc_screen.z > 1) continue;
            float z = 0;
            glm::fvec3 bc_clip = glm::f32vec3(bc_screen.x / vertices[0].w, bc_screen.y / vertices[1].w, bc_screen.z / vertices[2].w);
            bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
            app->getShader()->fragment(bc_clip, color);
            for (int i = 0; i < 3; i++)  z += vertices[i][2] * bc_clip[i];
            if (app->getWindow()->getFrameBuffer()->getDepth(p.x, p.y) > z) {
                app->getWindow()->getFrameBuffer()->setDepth(p.x, p.y, z);
                app->getWindow()->getFrameBuffer()->setColor((int)p.x, (int)p.y, color[3], color[2], color[1], color[0]);
            }

        }
    }
}

glm::fvec3 Rasterizer::barycentric(glm::fvec2* pts, glm::fvec2 p)
{
    glm::fvec2 v0 = pts[1] - pts[0];
    glm::fvec2 v1 = pts[2] - pts[0];
    glm::fvec2 v2 = p - pts[0];
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return glm::fvec3(u, v, w);
}
