//
// Created by sebas on 29/12/2024.
//

#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Engine.h"
#include "glm.hpp"

#define VERTEX_PER_QUAD 6
#define VERTEX_PER_CUBE 36


class RenderEngine final : Engine {
public:
    RenderEngine();
    ~RenderEngine() override;
    static RenderEngine* getInstance();
    void update(double deltaTime_ms) override;
    static glm::ivec2 getWindowSize();
    static glm::ivec2 getMaxWindowSize();
    static float getWindowAspectRatio();
    static float getWindowInverseAspectRatio();
private:
    static RenderEngine* instance;
    static glm::ivec2 windowSize;
    static glm::ivec2 maxWindowSize;
    static float aspectRatio, inverseAspectRatio;
    static void updateWindowSize();
};



#endif //RENDERENGINE_H
