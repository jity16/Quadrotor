#ifndef DRONE_CYLINDER_H
#define DRONE_CYLINDER_H


#include "Object.h"

class Cylinder : public Object
{
    using Object::Object;
public:
    bool isCylinder() override;
};


#endif //DRONE_CYLINDER_H
