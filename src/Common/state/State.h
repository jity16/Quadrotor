#ifndef LANDING_STATE_H
#define LANDING_STATE_H

#include "../config/Config.h"
template <class T>
class State
{
public:
    virtual void Enter(T*) = 0;
    virtual void Execute(T*) = 0;
    virtual void Exit(T*) = 0;
    virtual ~State(){};
};


#endif //LANDING_STATE_H
