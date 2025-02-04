//
// Created by sebas on 30/12/2024.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include "glm.hpp"

void* platform_load_gl_function(char* funName);

int platform_create_window(int width, int height, const char* title);
void platform_update_window();
void platform_swap_buffers();
glm::ivec2 platform_get_window_size();

void lockMouse();
void unlockMouse();
void hideMouse();
void showMouse();

void platform_fill_keycode_lookup_table();

#endif //PLATFORM_H
