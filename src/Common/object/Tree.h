#ifndef DRONE_TREE_H
#define DRONE_TREE_H


#include "Object.h"

class Tree : public Object
{
    using Object::Object;
public:
    bool isTree() override;
};


#endif //DRONE_TREE_H
