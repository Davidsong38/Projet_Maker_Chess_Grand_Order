//
// Created by sebas on 01/02/2025.
//

#ifndef SPRITETARGET_H
#define SPRITETARGET_H



class SpriteTarget {
public:
  virtual ~SpriteTarget() = default;
  virtual float getSpriteX() = 0;
  virtual float getSpriteY() = 0;
};



#endif //SPRITETARGET_H
