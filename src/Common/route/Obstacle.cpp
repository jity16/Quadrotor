#include "Obstacle.h"

Obstacle::Obstacle(const Point &center_pos, const Point &size)
{
    m_min_corner = center_pos - size * 0.5;
    m_max_corner = center_pos + size * 0.5;
}

const Point Obstacle::getSize() const
{
    return m_max_corner - m_min_corner;
}

Obstacle::Obstacle(Object *object) : Obstacle::Obstacle(object->getPosition(), object->getSize())
{
    m_name = object->getName();
}

const Point &Obstacle::getMinCorner() const
{
    return m_min_corner;
}

const Point &Obstacle::getMaxCorner() const
{
    return m_max_corner;
}

Point Obstacle::getNode(bool min_x, bool min_y, bool min_z) const
{
    return Point(min_x * m_min_corner.x() + (!min_x) * m_max_corner.x(),
                 min_y * m_min_corner.y() + (!min_y) * m_max_corner.y(),
                 min_z * m_min_corner.z() + (!min_z) * m_max_corner.z());
}

const std::string &Obstacle::getName() const
{
    return m_name;
}
