//
// Created by sebas on 29/12/2024.
//

#ifndef ENGINE_H
#define ENGINE_H



class Engine {
public:
    virtual ~Engine() = default;
    virtual void update(double deltaTime_ms) = 0;
};



#endif //ENGINE_H
