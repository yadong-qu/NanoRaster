#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <cmath>
class FrameBuffer
{
public:
    FrameBuffer()
    {

    };

    void init(unsigned int w, unsigned int h);

    void setColor(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    glm::fvec4 getColor(int x, int y);

    void setDepth(int x, int y, float d);
    float getDepth(int x, int y);
    void clear();

    std::vector<unsigned char> pixels;
    std::vector<float> depths;
    int width;
    int height;
};
