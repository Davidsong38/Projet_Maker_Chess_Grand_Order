//
// Created by sebas on 29/12/2024.
//

#include "RenderEngine.h"

#include "platform.h"
#include "glRenderer.h"
#include "log.h"

#include <iostream>

RenderEngine::RenderEngine() {
    std::cout << "Creating Render Engine" << std::endl;
    platform_create_window(1080,724,"Grrrrrrr");
    maxWindowSize = glm::ivec2(1980,1080);
    glInit();
}

RenderEngine::~RenderEngine() {
    std::cout << "Destroying Render Engine" << std::endl;
}

RenderEngine* RenderEngine::getInstance() {
    if (instance == nullptr) {
        instance = new RenderEngine();
    }
    return instance;
}

void RenderEngine::update(double deltaTime_ms) {
    platform_update_window();
    windowSize = platform_get_window_size();
    aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    glRender();
    platform_swap_buffers();
}

glm::ivec2 RenderEngine::getWindowSize() {
    return windowSize;
}

glm::ivec2 RenderEngine::getMaxWindowSize() {
    return maxWindowSize;
}

float RenderEngine::getWindowAspectRatio() {
    return aspectRatio;
}