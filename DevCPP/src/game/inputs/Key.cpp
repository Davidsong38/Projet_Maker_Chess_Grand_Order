//
// Created by sebas on 02/02/2025.
//

#include "Key.h"

bool Key::didKeyGetPressed() const {
    return keyPressedThisFrame;
}

bool Key::didKeyGetReleased() const {
    return keyReleasedThisFrame;
}

bool Key::isKeyDown() const {
    return keyDown;
}

bool Key::isKeyUp() const {
    return !keyDown;
}

void Key::setKey(const bool isDown) {
    if (isDown == keyDown)
        return;
    if (isDown)
        keyPressedThisFrame = true;
    else
        keyReleasedThisFrame = true;
    keyDown = isDown;
}

void Key::updateKey() {
    keyPressedThisFrame = false;
    keyReleasedThisFrame = false;
}