#ifndef LANDING_UTILS_H
#define LANDING_UTILS_H

#include <string>
#include <vector>
#include <regex>
#include <ctime>
#include "Point.h"
#include "../route/Obstacle.h"

#ifdef _WIN32
#include <windows.h>
#elif defined (__linux) || defined (__APPLE__)
#include <signal.h>
#include <unistd.h>
#endif
namespace utils
{
    const std::string getNowTime();
    const std::vector<std::string> regexSplit(const std::string &str, const std::string &delimiter);
    void sleep(int microseconds);
    void transformUTCtoBJC(int &year, int &month, int &day, int &hour, int &minute, int &second);
    Point getObjectPosition(simxInt handle, simxInt client_id);
    float getFloatSignal(const std::string &signal, simxInt client_id);
    const Obstacle getDroneBoxWithBasePos(const Point &base_pos);
    bool existCollision(const Point &p, const Obstacle &obstacle);
    bool existCollisionBetweenBaseAndObstacle(const Point &base_pos, const Obstacle &obstacle);
    bool existCollisionBetweenBaselineAndObstacle(const Point &start_pos, const Point &end_pos, const Obstacle &obstacle);
    bool existCollisionWithObstacles(const Point &p, const std::vector<Obstacle> &obstacles);
    bool existCollisionBetweenBaseAndObstacles(const Point &base_pos, const std::vector<Obstacle> &obstacles);
    bool existCollisionBetweenBaselineAndObstacles(const Point &start_pos, const Point &end_pos, const std::vector<Obstacle> &obstacles);
    const Point generateRandomPointBetween(const Point &start_pos, const Point &end_pos);
    Point getCoordinateInLeftCamera(float x_p_left, float x_p_right, float y_p);
    Point convertCoordinateSystem(simxInt handle, const Point &p);
};


#endif //LANDING_UTILS_H
