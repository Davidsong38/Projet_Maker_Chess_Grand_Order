//
// Created by sebas on 09/02/2025.
//

#ifndef SPRITEGROUP_H
#define SPRITEGROUP_H

#include <Displayable.h>
#include <rendering_cfg.h>
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
    float getDistanceToCamera() override { return -defaultLayer; }
protected:
    VAO* vao;
    Texture* texture;
    Shader* shader;
    spritePositioner_type* spritePositioners;
    int positionsSize, positionCount{0};
    float defaultLayer{LAYER_GROUND};
};



#endif //SPRITEGROUP_H
