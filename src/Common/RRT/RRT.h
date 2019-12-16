//
// Created by eric on 19-7-22.
//

#ifndef DRONE_RRT_H
#define DRONE_RRT_H
#include "../utils/utils.h"
#include "TreeNode.h"


class RRT
{
public:
    RRT();

    /**
     * 障碍物为与坐标轴平行的长方体，通过两个相对的定点确定长方体的位置。
     * @param p1 体对角线的一个点
     * @param p2 体对角线的另一个点
     */
    void addObstacle(Point p1, Point p2);

    /**
     * 生成一条无碰撞的路径。
     * @param source IN
     * @param destination IN
     * @param route OUT, the generated route
     * @param stepLength IN
     * @return
     */
    bool
    generateRoute(const Point source, const Point destination,
                  std::vector<Point> &route, double stepLength = 0.2);

private:
    Point minCoordinate, maxCoordinate;
    const double maxError = 0.5;

    std::vector<std::pair<Point, Point>> obstacles;

    double randomDouble(double l, double r);

    Point getRandomPoint();

    Point
    extendPoint(Point formalPoint, Point directionPoint,
                double stepLength);

    int getNearestPoint(Point point, const std::vector<TreeNode> &tree);

    bool existCollision(Point p1, Point p2);

    std::vector<Point> tracebackRoute(int pointId, const std::vector<TreeNode> &tree);

    bool lessOrEqualForPoints(Point p1, Point p2);
};


#endif //DRONE_RRT_H
