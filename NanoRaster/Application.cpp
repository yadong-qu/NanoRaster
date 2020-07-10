#include "Application.h"

Application::Application(int w, int h) : mWidth(w), mHeight(h), shader() {
    mWindow.init(mWidth, mHeight);
    rasterizer.init(this);
    Model head; 
    head.load("assets/african_head/african_head.obj");
    Model eyeInner;
    eyeInner.load("assets/african_head/african_head_eye_inner.obj");
    models.emplace_back(head);
    models.emplace_back(eyeInner);
 /*   models.emplace_back(eyeOuter);*/
}




void Application::run() {
    
    glm::mat4 viewMatrix = glm::lookAt(glm::fvec3(0.0f, 0.0f, 2.0f), glm::fvec3(0.0f, 0.0f, 0.0f), glm::fvec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projectMatrix = glm::perspective(glm::radians(60.f), (float)mWidth / (float)mHeight, 0.1f, 100.f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    shader.mvp = projectMatrix * viewMatrix * modelMatrix;
    glm::mat4 mv = viewMatrix * modelMatrix;
    shader.modelViewMatrix = mv;
    shader.normalMatrix = glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));
    shader.cameraPos = mv * glm::fvec4(0.0f, 0.0f, 2.0f, 1.0f);
    Light L;
    L.pos = mv * glm::fvec4(2, 2, 2, 1);

    shader.light = L;

    
    glm::fvec4 vertices[3];

   
    glm::dvec4 ndc;
    SDL_Event event;
    bool running = true;
    while (running) {
        const Uint64 start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&event)) {
            if ((SDL_QUIT == event.type) ||
                (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
            {
                running = false;
                break;
            }
        }
        mWindow.clear();
        for (std::size_t m = 0; m < models.size(); m++) {
            Model* model = &models[m];
            shader.model = model;
            for (std::size_t f = 0; f < model->faces.size(); f++) {
                for (std::size_t n = 0; n < 3; n++) {
                    shader.normals[n] = model->normal(f, n);
                    shader.texCoords[n] = model->texCoord(f, n);
                    vertices[n] = shader.vertex(model->vert(f, n), n);
                   
                }
                rasterizer.rasterTriangle(vertices);
            }
        }
       

        mWindow.update();
        mWindow.draw();
        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = (end - start) / static_cast<double>(freq);
        std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
    }
}
