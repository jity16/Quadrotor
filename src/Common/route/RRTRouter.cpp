//
// Created by eric on 19-7-23.
//

#include <iostream>
#include "RRTRouter.h"
#include "../RRT/RRT.h"

std::vector<Point>
RRTRouter::route(const Point &start_pos, const Point &end_pos)
{
    RRT rrt;
    std::vector<Point> route, ret;

    puts("==== obs starts");
    for (const auto &obstacle: m_obstacles)
    {
        rrt.addObstacle(obstacle.getMinCorner(), obstacle.getMaxCorner());
        auto &a = obstacle.getMinCorner();
        auto &b = obstacle.getMaxCorner();
        printf("%lf %lf %lf %lf %lf %lf\n", a.x(), a.y(), a.z(), b.x(), b.y(), b.z());
    }
    puts("==== obs starts");
    puts("start_pos");
    std::cout << start_pos << " " << end_pos << std::endl;
    puts("end_pos");
    rrt.generateRoute(start_pos, end_pos, route, 0.5);

    printf("Route:\n{");
    for (const auto &point: route)
    {
        printf("%.4lf,%.4lf,%.4lf,\n", point.x(), point.y(), point.z());
    }
    printf("}\n");

    for (int i = 0; i < m_obstacles.size(); ++i)
    {
        if (i % 4 == 0)
        {
            ret.push_back(route[i]);
        }
    }

    return ret;
}
