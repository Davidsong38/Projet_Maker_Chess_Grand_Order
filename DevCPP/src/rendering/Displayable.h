//
// Created by sebas on 03/01/2025.
//

#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H



class Displayable {
public:
    virtual ~Displayable() = default;
    virtual void draw() = 0;
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual float getDistanceToCamera() = 0;
};



#endif //DISPLAYABLE_H
