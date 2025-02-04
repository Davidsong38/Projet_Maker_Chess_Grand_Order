//
// Created by sebas on 02/02/2025.
//

#include "mouseInputs.h"

#include "log.h"
#include <iostream>
#include "platform.h"

/*
 * clickDown is called upon pressing the button
 * clickUp is called upon releasing the button
 * (0, 0) corresponds to the top left corner
 * (1, 0) corresponds to the top right corner
 * (0, 1) corresponds to the bottom left corner
 * (1, 1) corresponds to the bottom right corner
 */

bool isLeftClickHeld = false;
bool isRightClickHeld = false;
bool isMiddleClickHeld = false;

void leftClickUp(glm::vec2 mousePos) {
    isLeftClickHeld = true;
}

void leftClickDown(glm::vec2 mousePos) {
    isLeftClickHeld = false;
}

void rightClickUp(glm::vec2 mousePos) {
    isRightClickHeld = true;
}

void rightClickDown(glm::vec2 mousePos) {
    isRightClickHeld = false;
}

void middleClickUp(glm::vec2 mousePos) {
    isMiddleClickHeld = true;
}

void middleClickDown(glm::vec2 mousePos) {
    isMiddleClickHeld = false;
}

glm::vec2 getMousePos() {
    return platform_get_mouse_position();
}

bool isLeftClickBeingHeld() {
    return isLeftClickHeld;
}

bool isRightClickBeingHeld() {
    return isRightClickHeld;
}

bool isMiddleClickBeingHeld() {
    return isMiddleClickHeld;
}


