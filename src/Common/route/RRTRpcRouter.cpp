//
// Created by eric on 19-7-25.
//

#include "RRTRpcRouter.h"
#include "../RRT/RRTClient.h"

std::vector<Point> RRTRpcRouter::route(const Point &start_pos, const Point &end_pos)
{
    RRTClient rrt;
    std::vector<Point> route;
    std::vector<double> obstacle_points;

    for (const auto &obstacle: m_obstacles)
    {
        auto &a = obstacle.getMinCorner();
        auto &b = obstacle.getMaxCorner();
        obstacle_points.push_back(a.x());
        obstacle_points.push_back(a.y());
        obstacle_points.push_back(a.z());
        obstacle_points.push_back(b.x());
        obstacle_points.push_back(b.y());
        obstacle_points.push_back(b.z());
        // DEBUG: output obstacles
//        printf("%lf %lf %lf %lf %lf %lf\n", a.x(), a.y(), a.z(), b.x(), b.y(), b.z());
    }
    std::cout <<"DEBUG: start_pos "<< start_pos << " end_pos " << end_pos << std::endl;
    bool isOk = false;
    while (!isOk)
    {
        isOk = rrtClient.setObstacles(obstacle_points);
        if (!isOk)
        {
            std::cerr << "WARNING: rrtClient.setObstacles failed. "
                         "Start to retry."
                      << std::endl;
        }
    }

    isOk = false;
    while (!isOk)
    {
        isOk = rrtClient.getRoute(start_pos, end_pos, route);
        if (!isOk)
        {
            std::cerr << "WARNING: rrtClient.getRoute failed. "
                         "Start to retry."
                      << std::endl;
        }
    }

    printf("INFO: Route:\n{");
    for (const auto &point: route)
    {
        printf("%.4lf,%.4lf,%.4lf,\n", point.x(), point.y(), point.z());
    }
    printf("}\n");

    return route;
}

RRTRpcRouter::RRTRpcRouter()
{
    std::cout << "INFO: Use RRT RPC router" << std::endl;
}
