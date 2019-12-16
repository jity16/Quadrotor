#ifndef DRONE_OBSTACLE_H
#define DRONE_OBSTACLE_H


#include "../utils/Point.h"
#include "../object/Object.h"

class Obstacle
{
public:
    explicit Obstacle(Object *object);

    Obstacle(const Point &center_pos, const Point &size);

    const Point getSize() const;

    const Point &getMinCorner() const;

    const Point &getMaxCorner() const;

    Point getNode(bool min_x, bool min_y, bool min_z) const;

    const std::string &getName() const;

private:
    Point m_min_corner, m_max_corner;
    std::string m_name;
};


#endif //DRONE_OBSTACLE_H
