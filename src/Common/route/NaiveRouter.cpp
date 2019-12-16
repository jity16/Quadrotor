#include <iostream>
#include "NaiveRouter.h"
#include "../utils/utils.h"

std::vector<Point> NaiveRouter::route(const Point &start_pos, const Point &end_pos)
{
    std::vector<Point> ret;
    ret.emplace_back(start_pos);
    //ret.emplace_back((start_pos + end_pos) / 2);
    ret.emplace_back(end_pos);
/*    if (utils::existCollisionBetweenBaselineAndObstacles(start_pos, end_pos, m_obstacles))
    {
        return std::vector<Point>{};
    }*/
    return ret;
}
