//
// Created by sebas on 02/02/2025.
//

#include "mouseInputs.h"

#include <log.h>

#include "platform.h"
#include "clickables.h"

/*
 * clickDown is called upon pressing the button
 * clickUp is called upon releasing the button
 * When it comes to mousePos :
 * (0, 0) corresponds to the top left corner
 * (1, 0) corresponds to the top right corner
 * (0, 1) corresponds to the bottom left corner
 * (1, 1) corresponds to the bottom right corner
 * When it comes to screenMousePos :
 * (-1, -1) corresponds to the top left corner
 * (+1, -1) corresponds to the top right corner
 * (-1, +1) corresponds to the bottom left corner
 * (+1, +1) corresponds to the bottom right corner
 */

bool isLeftClickHeld = false;
bool isRightClickHeld = false;
bool isMiddleClickHeld = false;

void leftClickUp(const glm::vec2 mousePos) {
    isLeftClickHeld = false;
    const glm::vec2 screenMousePos = getScreenMousePos(mousePos);
    for (const auto & clickable_area_lmb : clickable_areas_lmb) {
        if (clickable_area_lmb == nullptr) continue;
        clickable_area_lmb->releaseClick();
    }
}

void leftClickDown(const glm::vec2 mousePos) {
    isLeftClickHeld = true;
    const glm::vec2 screenMousePos = getScreenMousePos(mousePos);
    for (const auto & clickable_area_lmb : clickable_areas_lmb) {
        if (clickable_area_lmb == nullptr) continue;
        if (clickable_area_lmb->clickAt(screenMousePos)) break;
    }
}

void rightClickUp(const glm::vec2 mousePos) {
    isRightClickHeld = false;
    const glm::vec2 screenMousePos = getScreenMousePos(mousePos);
    for (const auto & clickable_area_rmb : clickable_areas_rmb) {
        if (clickable_area_rmb == nullptr) continue;
        clickable_area_rmb->releaseClick();
    }
}

void rightClickDown(const glm::vec2 mousePos) {
    isRightClickHeld = true;
    const glm::vec2 screenMousePos = getScreenMousePos(mousePos);
    for (const auto & clickable_area_rmb : clickable_areas_rmb) {
        if (clickable_area_rmb == nullptr) continue;
        if (clickable_area_rmb->clickAt(screenMousePos)) break;
    }
}

void middleClickUp(const glm::vec2 mousePos) {
    isMiddleClickHeld = false;
    const glm::vec2 screenMousePos = getScreenMousePos(mousePos);
    for (const auto & clickable_area_mmb : clickable_areas_mmb) {
        if (clickable_area_mmb == nullptr) continue;
        clickable_area_mmb->releaseClick();
    }
}

void middleClickDown(const glm::vec2 mousePos) {
    isMiddleClickHeld = true;
    const glm::vec2 screenMousePos = getScreenMousePos(mousePos);
    for (const auto & clickable_area_mmb : clickable_areas_mmb) {
        if (clickable_area_mmb == nullptr) continue;
        if (clickable_area_mmb->clickAt(screenMousePos)) break;
    }
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

glm::vec2 getMousePos() {
    return platform_get_mouse_position();
}

glm::vec2 getScreenMousePos() {
    return getScreenMousePos(getMousePos());
}

glm::vec2 getScreenMousePos(const glm::vec2 mousePos) {
    return { 2 * mousePos.x - 1, 2 * mousePos.y - 1 };
}


