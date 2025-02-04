//
// Created by sebas on 02/02/2025.
//

#ifndef MOUSEINPUTS_H
#define MOUSEINPUTS_H

#include <glm.hpp>

void leftClickUp(glm::vec2 mousePos);
void leftClickDown(glm::vec2 mousePos);
void rightClickUp(glm::vec2 mousePos);
void rightClickDown(glm::vec2 mousePos);
void middleClickUp(glm::vec2 mousePos);
void middleClickDown(glm::vec2 mousePos);

glm::vec2 getMousePos();
bool isLeftClickBeingHeld();
bool isRightClickBeingHeld();
bool isMiddleClickBeingHeld();

#endif //MOUSEINPUTS_H
