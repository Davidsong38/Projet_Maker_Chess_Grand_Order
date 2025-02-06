//
// Created by sebas on 02/02/2025.
//

#include "mouseInputs.h"

#include <Chessboard.h>
#include <GameEngine.h>

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
    isLeftClickHeld = false;
}

void leftClickDown(glm::vec2 mousePos) {
    isLeftClickHeld = true;
    auto posInBoard = glm::ivec2();
    if (Chessboard::getInstance()->getPosInBoard(mousePos, posInBoard)) {
        //cout << posInBoard.x << ", " << posInBoard.y << endl;
        GameEngine::getInstance()->clickBoardAtPos(posInBoard.x, posInBoard.y);
    }
}

void rightClickUp(glm::vec2 mousePos) {
    isRightClickHeld = false;
}

void rightClickDown(glm::vec2 mousePos) {
    isRightClickHeld = true;
}

void middleClickUp(glm::vec2 mousePos) {
    isMiddleClickHeld = false;
}

void middleClickDown(glm::vec2 mousePos) {
    isMiddleClickHeld = true;
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


