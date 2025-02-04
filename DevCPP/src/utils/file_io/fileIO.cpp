//
// Created by sebas on 09/01/2025.
//

#include "fileIO.h"

#include <cstring>
#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

long get_file_size(const std::string &filePath) {
  long fileSize=0;
  FILE* file = fopen(filePath.c_str(),"rb");
  if (!file){
    std::cerr << "Error opening file " << filePath << std::endl;
    return 0;
  }
  fseek(file,0,SEEK_END);
  fileSize = ftell(file);
  fseek(file,0,SEEK_SET);
  fclose(file);
  return fileSize;
}

char* readFile(const std::string &filePath) {
  long fileSize = get_file_size(filePath);
  char* buffer = (char*)malloc(fileSize+1);
  memset(buffer,0,fileSize + 1);
  FILE* file = fopen(filePath.c_str(),"rb");
  if (!file) {
    std::cerr << "Error opening file " << filePath << std::endl;
  }
  fread(buffer,sizeof(char),fileSize,file);
  return buffer;
}