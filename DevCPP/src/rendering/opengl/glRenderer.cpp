//
// Created by sebas on 02/01/2025.
//

#include "glRenderer.h"

#include <iostream>
#include "log.h"
#include "RenderEngine.h"
#include "shaders.h"
#include "cubemaps.h"
#include "vaos.h"
#include "textures.h"
#include "uiElements.h"

#include "glm.hpp"

static GLuint
    frameBuffer,
    renderBuffer;

static glm::ivec2 resolution;
static glm::ivec2 maxResolution;

static void APIENTRY gl_debug_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* user
){
    if (
        severity == GL_DEBUG_SEVERITY_LOW ||
        severity == GL_DEBUG_SEVERITY_MEDIUM ||
        severity == GL_DEBUG_SEVERITY_HIGH
    ) {
        log(LOG_ERROR, message);
    } else {
        log(LOG_INFO, message);
    }
}

void glInit() {
    maxResolution = RenderEngine::getMaxWindowSize();
    load_gl_functions();
    glInitCfg();
    initShaders();
    loadCubeMaps();
    loadTextures();
    initVAOs();
    initUIElements();
    glInitFrameBuffers();
}

void glInitCfg() {
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void glInitFrameBuffers() {
    log(LOG_INFO,"OpenGL Rendering Init Frame Buffer");
    // Frame Buffer
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    frameTexture->useAsTarget();
    log(LOG_INFO,"OpenGL Rendering Init Render Buffer");
    // Render Buffer
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,maxResolution.x,maxResolution.y);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        renderBuffer
    );
    log(LOG_INFO,"OpenGL Rendering Init Check");
    FATAL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,"Scene framebuffer incomplete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void glClearScreen() {
    glClearColor(0.8,0.8,0.8,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void glRender() {
    resolution = RenderEngine::getWindowSize();
    glRenderWorld();
    glRenderUI();
}

void glRenderSkyBox() {
    glDepthMask(GL_FALSE);
    skybox->use();
    skyboxShader->use();
    skyboxVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_CUBE);
    glDepthMask(GL_TRUE);
}

void glRenderWorld() {
    glViewport(0, 0, resolution.x, resolution.y);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST);
    glClearScreen();
    glRenderSkyBox();
    ///TODO draw world displayables
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glWorldPostProcessing();
    glRenderUI();
}

void glWorldPostProcessing() {
    glClearScreen();
    postProcessingShader->use();
    quadVAO->bind();
    frameTexture->bind();
    glDrawArrays(GL_TRIANGLES,0,VERTEX_PER_QUAD);
}

void glRenderUI() {
    for (int i = 0; i < UI_SPRITE_COUNT; i++) {
        ui_sprites[i]->draw();
    }
}