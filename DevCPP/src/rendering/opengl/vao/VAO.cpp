//
// Created by sebas on 12/01/2025.
//

#include "VAO.h"

#include <iostream>
#include "glFunctions.h"

#include "log.h"

VAO::VAO(const GLuint vaoID, const float buffer[], const GLsizeiptr buffer_size, const int attribConfig) {
    this->vaoID = vaoID;
    glGenBuffers(1,&this->vboID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER,this->vboID);
    glBufferData(GL_ARRAY_BUFFER,buffer_size,buffer,GL_STATIC_DRAW);
    initAttributes(attribConfig, GL_FLOAT);
    glBindVertexArray(0);
}

VAO::VAO(const GLuint vaoID, const double buffer[], const GLsizeiptr buffer_size, const int attribConfig) {
    this->vaoID = vaoID;
    glGenBuffers(1,&this->vboID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER,this->vboID);
    glBufferData(GL_ARRAY_BUFFER,buffer_size,buffer,GL_STATIC_DRAW);
    initAttributes(attribConfig, GL_DOUBLE);
    glBindVertexArray(0);
}

void VAO::bind() const {
    glBindVertexArray(vaoID);
}

void VAO::initAttributes(const int attribConfig, const GLuint dataType) {
    switch (attribConfig) {
        case VAO_2D_TEXTURE: {
            constexpr int attribSize[2]{2, 2};
            initAttributes(2, attribSize, dataType);
            break;
        }
        case VAO_3D_TEXTURE: {
            constexpr int attribSize[2]{3, 2};
            initAttributes(2, attribSize, dataType);
            break;
        }
        case VAO_3D_NORMAL: {
            constexpr int attribSize[2]{3, 3};
            initAttributes(2, attribSize, dataType);
            break;
        }
        case VAO_3D_TEXTURE_NORMAL: {
            constexpr int attribSize[3]{3, 2, 3};
            initAttributes(3, attribSize, dataType);
            break;
        }
        case VAO_3D: {
            constexpr int attribSize[1]{3};
            initAttributes(1, attribSize, dataType);
            break;
        }
        default: {
            ltr_log_error("VAO::initAttributes: Unsupported attrib config : ", attribConfig);
        }
    }
}

void VAO::initAttributes(const int attribCount, const int attribSize[], const GLuint dataType) {
    const GLsizei valueSize = (dataType == GL_FLOAT) ? sizeof(float) : sizeof(double);
    GLsizei currentSize = 0;
    GLsizei totalSize = 0;
    for (int i = 0; i < attribCount; i++) totalSize += valueSize * attribSize[i];
    for (int i = 0; i < attribCount; i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            attribSize[i],
            dataType,
            GL_FALSE,
            totalSize,
            reinterpret_cast<void *>(currentSize)
        );
        currentSize += valueSize * attribSize[i];
    }
}