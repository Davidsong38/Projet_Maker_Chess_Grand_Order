//
// Created by sebas on 09/02/2025.
//

#ifndef SPRITEGROUP_H
#define SPRITEGROUP_H

#include <Displayable.h>
#include <Texture.h>
#include <VAO.h>
#include <Shader.h>
#include <spritePositioner.h>


class SpriteGroup : public Displayable {
public:
    SpriteGroup(Texture* texture, int positionsSize);
    ~SpriteGroup() override;
    void draw() override;
    void load() override;
    void unload() override;
    void addPosition(spritePositioner_type spritePositioner);
    void clearPositions();
protected:
    VAO* vao;
    Texture* texture;
    Shader* shader;
    spritePositioner_type* spritePositioners;
    int positionsSize, positionCount{0};
};



#endif //SPRITEGROUP_H
