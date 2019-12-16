#include "Router.h"

Router::Router()
{

}

void Router::addObstacle(const Obstacle &obstacle)
{
    m_obstacles.emplace_back(obstacle);
}