//
// Created by sebas on 19/02/2025.
//

#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include <Button.h>


class RadioButton : public SpriteTarget {
public:
    RadioButton(
        const std::vector<Texture *> &buttonTextures,
        glm::vec3 position,
        glm::vec2 button_size,
        glm::vec2 button_spacing,
        Texture * selection_texture,
        glm::vec2 selection_size,
        glm::vec3 selection_position,
        glm::vec3 button_filter_color
    );
    RadioButton(
        const std::vector<Texture *> &buttonTextures,
        glm::vec3 position,
        glm::vec2 button_size,
        glm::vec2 button_spacing,
        Texture * selection_texture,
        glm::vec2 selection_size,
        glm::vec3 selection_position,
        glm::vec3 button_filter_color,
        bool centered
    );
    void setIndex(int index);
    [[nodiscard]] int getIndex() const;
    void hide();
    void show();

    float getSpriteX() override;
    float getSpriteY() override;
    glm::vec3 getFilterColor() override;
    glm::vec4 getDefaultColor() override;
    float getSpriteRotation() override;
    bool isHidden() override;
protected:
    std::vector<Button *> radioButtonButtons;
    int selectedRadioButtonIndex;
    glm::vec3 selection_offset;
    Sprite* selection_sprite;
    bool hidden;
};



#endif //RADIOBUTTON_H
