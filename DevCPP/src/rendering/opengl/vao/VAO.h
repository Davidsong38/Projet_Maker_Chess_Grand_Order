//
// Created by sebas on 12/01/2025.
//

#ifndef VAO_H
#define VAO_H


#define VAO_2D_TEXTURE 0
#define VAO_3D_TEXTURE 1
#define VAO_3D_NORMAL 2
#define VAO_3D_TEXTURE_NORMAL 3
#define VAO_3D 4

class VAO {
public:
  VAO(unsigned int vaoID, const float buffer[], signed long long int buffer_size, int attribConfig);
  VAO(unsigned int vaoID, const double buffer[], signed long long int buffer_size, int attribConfig);
  void bind() const;
private:
  unsigned int vaoID, vboID{0};
  static void initAttributes(int attribConfig, unsigned int dataType);
  static void initAttributes(int attribCount, const int attribSize[], unsigned int dataType);
};



#endif //VAO_H
