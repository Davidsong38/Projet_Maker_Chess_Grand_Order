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

bool isLeftClickBeingHeld();
bool isRightClickBeingHeld();
bool isMiddleClickBeingHeld();

glm::vec2 getMousePos();
glm::vec2 getScreenMousePos();
glm::vec2 getScreenMousePos(glm::vec2 mousePos);

#endif //MOUSEINPUTS_H
