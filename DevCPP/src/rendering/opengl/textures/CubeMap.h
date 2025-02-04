//
// Created by sebas on 12/01/2025.
//

#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>

#include "glFunctions.h"

class CubeMap {
public:
  CubeMap(GLuint cubeMapID, const std::string &folderName);
  ~CubeMap();
  void use() const;
private:
  GLuint cubeMapID;
};



#endif //CUBEMAP_H
