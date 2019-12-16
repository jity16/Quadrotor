//
// Created by eric on 19-7-22.
//

#include "RRT.h"
#include "TreeNode.h"

RRT::RRT() : minCoordinate(-100, -100, 0), maxCoordinate(100, 100, 100)
{

}

void RRT::addObstacle(Point p1, Point p2)
{
    obstacles.emplace_back(p1, p2);
}

bool
RRT::generateRoute(const Point source, const Point destination,
                   std::vector<Point> &route, double stepLength)
{

    std::vector<TreeNode> tree{};
    tree.emplace_back(source, 0);
    for (int i = 0; i < 1000; ++i)
    {
        auto randomPoint = (i % 5 == 0) ? destination : getRandomPoint();
        int nearestPointId = getNearestPoint(randomPoint, tree);
        auto extendedPoint = extendPoint(tree[nearestPointId].pos, randomPoint,
                                         stepLength);
        if (existCollision(tree[nearestPointId].pos, extendedPoint))
        {
            continue;
        }

        printf("%d %lf %lf %lf\n", i, extendedPoint.x(), extendedPoint.y(),
               extendedPoint.z());

        tree.emplace_back(extendedPoint, nearestPointId);
        tree[nearestPointId].child.emplace_back(tree.size() - 1);
        if ((extendedPoint - destination).length() < maxError)
        {
            route = tracebackRoute(tree.size() - 1, tree);
            route.push_back(destination);
            return true;
        }
    }
    return false;
}

Point RRT::getRandomPoint()
{
    return Point(randomDouble(minCoordinate.x(), maxCoordinate.x()),
                 randomDouble(minCoordinate.x(), maxCoordinate.y()),
                 randomDouble(minCoordinate.x(), maxCoordinate.z()));
}

double RRT::randomDouble(double l, double r)
{
    static long randomNum = 0;
    randomNum = 25214903917 * randomNum + 11;
    return l + (r - l) / RAND_MAX * rand();
}

Point
RRT::extendPoint(Point formalPoint, Point directionPoint, double stepLength)
{
    auto vec = (directionPoint - formalPoint);
    return formalPoint + (vec) / (vec.length() / stepLength);
}

int RRT::getNearestPoint(Point point, const std::vector<TreeNode> &tree)
{
    double minDistance = 1e100;
    int nearestId = -1;
    for (int i = 0; i < tree.size(); ++i)
    {
        auto distance=(tree[i].pos - point).length();
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestId = i;
        }
    }
    return nearestId;
}

bool RRT::existCollision(Point p1, Point p2)
{
    for (const auto &obstacle:obstacles)
    {

        if (lessOrEqualForPoints(obstacle.first, p2) &&
            lessOrEqualForPoints(p2, obstacle.second))
        {
            return true;
        }
    }
    return false;
}

bool RRT::lessOrEqualForPoints(Point p1, Point p2)
{
    return p1.x() <= p2.x() && p1.y() <= p2.y() && p1.z() <= p2.z();
}

std::vector<Point> RRT::tracebackRoute(int pointId, const std::vector<TreeNode> &tree)
{
    auto ret = std::vector<Point>();
    ret.push_back(tree[pointId].pos);
    while (pointId)
    {
        pointId = tree[pointId].parent;
        ret.push_back(tree[pointId].pos);
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}
