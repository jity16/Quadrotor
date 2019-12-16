#ifndef DRONE_JOINT_H
#define DRONE_JOINT_H


#include "Object.h"

class Joint : public Object
{
    using Object::Object;
public:
    bool isJoint() override;
};


#endif //DRONE_JOINT_H
