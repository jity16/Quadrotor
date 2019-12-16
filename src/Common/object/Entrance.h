#ifndef DRONE_ENTRANCE_H
#define DRONE_ENTRANCE_H


#include "Object.h"

class Entrance : public Object
{
public:
    Entrance(Object *door, bool is_front);

    bool isEntrance() override;

    Object *getParent() const;

    bool isFront() const;

    Object *getOtherEntrance() const;

private:
    Object *m_parent;
    bool m_is_front;
    const float Y_DISTANCE_FROM_OBJECT = Config::Instance()->getFloatParam("Entrance", "Y_DISTANCE_FROM_OBJECT");
};


#endif //DRONE_ENTRANCE_H
