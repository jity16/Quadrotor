#ifndef DRONE_DOOR_H
#define DRONE_DOOR_H


#include "Object.h"

class Door: public Object
{
public:
    Door(const std::string &name, int client_id);

    bool isDoor() override;

    Object *getFrontEntrance() const;

    Object *getBackEntrance() const;

    Object *getEntrance(bool is_front) const;

private:
    Object *m_front_entrance;
    Object *m_back_entrance;
};


#endif //DRONE_DOOR_H
