//
// Created by sebas on 12/01/2025.
//

#ifndef VAO_H
#define VAO_H

#include "glFunctions.h"

#define VAO_2D_TEXTURE 0
#define VAO_3D_TEXTURE 1
#define VAO_3D_NORMAL 2
#define VAO_3D_TEXTURE_NORMAL 3
#define VAO_3D 4

class VAO {
public:
  VAO(GLuint vaoID, const float buffer[], GLsizeiptr buffer_size, int attribConfig);
  VAO(GLuint vaoID, const double buffer[], GLsizeiptr buffer_size, int attribConfig);
  void bind() const;
private:
  GLuint vaoID, vboID{0};
  static void initAttributes(int attribConfig, GLuint dataType);
  static void initAttributes(int attribCount, const int attribSize[], GLuint dataType);
};



#endif //VAO_H
