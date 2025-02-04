//
// Created by sebas on 12/01/2025.
//

#include "CubeMap.h"

#include <iostream>
#include <ostream>

#include "stb_image.h"
#include "log.h"

constexpr std::string faces[6] {
    "/right.jpg",
    "/left.jpg",
    "/top.jpg",
    "/bottom.jpg",
    "/front.jpg",
    "/back.jpg"
};

CubeMap::CubeMap(const GLuint cubeMapID, const std::string &folderName) {
    this->cubeMapID = cubeMapID;
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++) {
        std::string path = "assets/cubemaps/" + folderName + faces[i];
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(
            path.c_str(),
            &width,
            &height,
            &nrChannels,
            0
        );
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        } else {
            std::cerr << "ERROR : Failed to load cubemap " << path << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMap::~CubeMap() = default;

void CubeMap::use() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
}
