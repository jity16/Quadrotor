#include "Door.h"
#include "../../Planning/task/ThroughDoorTask.h"
#include "Entrance.h"

bool Door::isDoor()
{
    return true;
}

Door::Door(const std::string &name, int client_id) : Object(name, client_id)
{
    m_task = new ThroughDoorTask(this);
    m_front_entrance = new Entrance(this, true);
    m_back_entrance = new Entrance(this, false);
}

Object *Door::getFrontEntrance() const
{
    return m_front_entrance;
}

Object *Door::getBackEntrance() const
{
    return m_back_entrance;
}

Object *Door::getEntrance(bool is_front) const
{
    if (is_front)return m_front_entrance;
    else return m_back_entrance;
}
