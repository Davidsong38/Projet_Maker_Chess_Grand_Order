//
// Created by sebas on 09/01/2025.
//

#include "type_ptr.hpp"

#include "Shader.h"
#include "glFunctions.h"

#include <iostream>
#include <ostream>

#include "log.h"
#include "fileIO.h"

Shader::Shader(const std::string& shaderName) {
    GLint success;

    const std::string vertexPath = "assets/shaders/opengl/" + shaderName + ".vert";
    const std::string fragmentPath = "assets/shaders/opengl/" + shaderName + ".frag";
    char* vertexCode = readFile(vertexPath);
    char* fragmentCode = readFile(fragmentPath);

    GLint shaderLength[2];
    shaderLength[0] = (GLint)strlen(vertexCode);
    shaderLength[1] = (GLint)strlen(fragmentCode);

    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader,1, &vertexCode, &shaderLength[0]);
    glShaderSource(fragmentShader,1, &fragmentCode, &shaderLength[1]);
    glCompileShader(vertexShader);      free(vertexCode);
    glCompileShader(fragmentShader);    free(fragmentCode);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    FATAL_ASSERT(success,"Failed to compile vertexShader");
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    FATAL_ASSERT(success,"Failed to compile fragmentShader");

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    FATAL_ASSERT(success,"Failed to link program");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const {
    glUseProgram(programID);
}

void Shader::setFloat(const std::string &name, const float value) const {
    glUniform1f(
        glGetUniformLocation(
            programID,
            name.c_str()
        ),
        value
    );
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
    glUniformMatrix4fv(
        glGetUniformLocation(
            programID,
            name.c_str()
        ),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}
