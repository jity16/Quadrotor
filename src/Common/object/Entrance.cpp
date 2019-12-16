#include <iostream>
#include "Entrance.h"
#include "Door.h"

Entrance::Entrance(Object *door, bool is_front) : m_parent(door), m_is_front(is_front)
{
    m_pos = m_parent->getPosition() + (m_is_front? Point(0, Y_DISTANCE_FROM_OBJECT, 0) : Point(0, -Y_DISTANCE_FROM_OBJECT, 0));
    m_size = Point(0, 0, 0);
    m_name = (m_is_front ? "Front_Of_" : "Back_Of_") + (m_parent->getName());
    m_task = nullptr;
}

bool Entrance::isEntrance()
{
    return true;
}

Object *Entrance::getParent() const
{
    return m_parent;
}

bool Entrance::isFront() const
{
    return m_is_front;
}

Object *Entrance::getOtherEntrance() const
{
    return dynamic_cast<Door *>(m_parent)->getEntrance(!m_is_front);
}
