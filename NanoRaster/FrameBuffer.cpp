#include "FrameBuffer.h"

void FrameBuffer::init(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    pixels.assign(w * h * 4, 0);
    depths.assign(w * h, -1.0f);
}

void FrameBuffer::setColor(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {

    int offset = width * 4 * y + x * 4;

    pixels[offset + 0] = r;
    pixels[offset + 1] = g;
    pixels[offset + 2] = b;
    pixels[offset + 3] = a;
}
glm::fvec4 FrameBuffer::getColor(int x, int y) {
    glm::fvec4 ret;
    int offset = width * 4 * y + x * 4;
    ret.x = pixels[offset + 0];
    ret.y = pixels[offset + 1];
    ret.z = pixels[offset + 2];
    ret.w = pixels[offset + 3];
    return ret;
}

void FrameBuffer::setDepth(int x, int y, float d) {
    int offset = width * y + x;
    depths[offset] = d;
}

float FrameBuffer::getDepth(int x, int y) {
    int offset = width * y + x;
    return depths[offset];
}

void FrameBuffer::clear() {
    pixels.assign(width * height * 4, 0);
    depths.assign(width * height, 1.0f);
}
