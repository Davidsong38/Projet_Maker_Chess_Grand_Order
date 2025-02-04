//
// Created by sebas on 09/01/2025.
//

#ifndef FILEIO_H
#define FILEIO_H

#include <string>

long get_file_size(const std::string &filePath);
char* readFile(const std::string &filePath);
void readFile(const std::string &fileName, char* output);

#endif //FILEIO_H
