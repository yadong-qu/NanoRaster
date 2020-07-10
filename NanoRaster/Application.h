#pragma once

#include <vector>
#include <iostream>


#include "Window.h"
#include "Rasterizer.h"
#include "Model.h"
#include "Shader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Application
{
public:
    Application(int w, int h);;
 

    void run();

    Window* getWindow() {
        return &mWindow;
    }

    int getWidth() {
        return mWidth;

    }

    int getHeight() {
        return mHeight;
    }

    //Model* getModel() {
    //    return &model;
    //}

    IShader* getShader() {
        return &shader;
    }

private:
    int mWidth;
    int mHeight;
    Window mWindow;
    Rasterizer rasterizer;
    std::vector<Model> models;
    GouraudShader shader;
};
