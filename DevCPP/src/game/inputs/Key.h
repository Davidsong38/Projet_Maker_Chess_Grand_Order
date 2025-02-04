//
// Created by sebas on 02/02/2025.
//

#ifndef KEY_H
#define KEY_H

class Key {
public:
    Key() = default;
    [[nodiscard]] bool didKeyGetPressed() const;
    [[nodiscard]] bool didKeyGetReleased() const;
    [[nodiscard]] bool isKeyDown() const;
    [[nodiscard]] bool isKeyUp() const;
    void setKey(bool isDown);
    void updateKey();
private:
    bool keyPressedThisFrame{}, keyReleasedThisFrame{}, keyDown{};
};



#endif //KEY_H
