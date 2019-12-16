#include <iostream>
#include <set>
#include "utils.h"
#include "../vrep/extApiPlatform.h"
#include <random>
#include <cmath>

std::default_random_engine e;
std::uniform_real_distribution<double> u(0, 1);

const float B = 0.14;
const float PI = acosf(-1.0f);
const float Alpha = 85 * PI / 180;
const float Beta = atanf(tan(Alpha) * 9 / 16);
//const float Beta = 85.0f * PI / 180;
//const float Alpha = atanf(tanf(Beta) * 16 / 9);
const float Px = 1280;
const float Py = 720;
const Point DRONE_SIZE = Point(0.4f, 0.4f, 0.3f);

const std::string utils::getNowTime()
{
    time_t sys_time;
    time(&sys_time);
    struct tm *p;
    p = gmtime(&sys_time);
    int year = 1900 + p->tm_year;
    int month = 1 + p->tm_mon;
    int day = p->tm_mday;
    int hour = p->tm_hour;
    int minute = p->tm_min;
    int second = p->tm_sec;
    transformUTCtoBJC(year, month, day, hour, minute, second);
    char time_str[1024];
    sprintf(time_str, "%4d/%02d/%02d/%02d:%02d:%02d", year, month, day, hour, minute, second);
    return std::string(time_str);
}

const std::vector<std::string> utils::regexSplit(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> res;
    std::string::size_type pos1, pos2;
    size_t len = str.length();
    pos2 = str.find(delimiter);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        res.emplace_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + delimiter.size();
        pos2 = str.find(delimiter, pos1);
    }
    if (pos1 != len)
        res.emplace_back(str.substr(pos1));
    return res;
}

void utils::sleep(int microseconds)
{
#ifdef __WIN32
    Sleep(DWORD(microseconds));
#elif __linux
    usleep(microseconds);
#endif
}

void utils::transformUTCtoBJC(int &year, int &month, int &day, int &hour, int &minute, int &second)
{
    int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 30, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) mon[2] = 29;
    else mon[2] = 28;

    hour += 8;

    if (hour == 24 && minute != 0)
    {
        day += 1;
        hour = 0;
    } else if (hour > 24)
    {
        day += 1;
        hour -= 24;
    }

    if (day > mon[month])
    {
        day -= mon[month];
        month += 1;
    }

    if (month > 12)
    {
        year += 1;
        month -= 12;
    }
}

Point utils::getObjectPosition(simxInt handle, simxInt client_id)
{
    static std::set<simxInt> streamed_position_handle;
    simxFloat pos[3];
    //first call with streaming and following with buffer
    if (streamed_position_handle.find(handle) == streamed_position_handle.end())
    {
        streamed_position_handle.insert(handle);
        simxGetObjectPosition(client_id, handle, -1, pos, simx_opmode_streaming);
    }
    while (simxGetObjectPosition(client_id, handle, -1, pos, simx_opmode_buffer) == simx_return_novalue_flag);
    return Point(pos[0], pos[1], pos[2]);
}

float utils::getFloatSignal(const std::string &signal, simxInt client_id)
{
    static std::set<std::string> streamed_float_signal;
    float value = -1;
    if (streamed_float_signal.find(signal) == streamed_float_signal.end())
    {
        streamed_float_signal.insert(signal);
        simxGetFloatSignal(client_id, signal.c_str(), &value, simx_opmode_streaming);
    }
    while (simxGetFloatSignal(client_id, signal.c_str(), &value, simx_opmode_buffer) == simx_return_novalue_flag);
    return value;
}

bool utils::existCollision(const Point &p, const Obstacle &obstacle)
{
    const Point &min_p = obstacle.getMinCorner();
    const Point &max_p = obstacle.getMaxCorner();
    return min_p.x() <= p.x() && p.x() <= max_p.x() &&
           min_p.y() <= p.y() && p.y() <= max_p.y() &&
           min_p.z() <= p.z() && p.z() <= max_p.z();
}

bool utils::existCollisionBetweenBaseAndObstacle(const Point &base_pos, const Obstacle &obstacle)
{
    const Obstacle drone = getDroneBoxWithBasePos(base_pos);
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                Point node = drone.getNode(i, j, k);
                if (existCollision(node, obstacle))return true;
                node = obstacle.getNode(i, j, k);
                if (existCollision(node, drone))return true;
            }
    return false;
}

bool
utils::existCollisionBetweenBaselineAndObstacle(const Point &start_pos, const Point &end_pos, const Obstacle &obstacle)
{
    Point error = end_pos - start_pos;
    for (int i = 0; i <= 100; i++)
    {
        Point base_pos = start_pos + error * i / 100;
        if (existCollisionBetweenBaseAndObstacle(base_pos, obstacle))
        {
            //std::cout << "collide with " << obstacle.getName() << std::endl;
            return true;
        }
    }
    return false;
}

const Obstacle utils::getDroneBoxWithBasePos(const Point &base_pos)
{
    return Obstacle(Point(base_pos.x(), base_pos.y(), base_pos.z() - DRONE_SIZE.z() / 2), DRONE_SIZE);
}

bool utils::existCollisionWithObstacles(const Point &p, const std::vector<Obstacle> &obstacles)
{
    for (const auto &obstacle: obstacles)
    {
        if (existCollision(p, obstacle))
        {
            return true;
        }
    }
    return false;
}

bool utils::existCollisionBetweenBaseAndObstacles(const Point &base_pos, const std::vector<Obstacle> &obstacles)
{
    for (const auto &obstacle: obstacles)
    {
        if (existCollisionBetweenBaseAndObstacle(base_pos, obstacle))
        {
            return true;
        }
    }
    return false;
}

bool utils::existCollisionBetweenBaselineAndObstacles(const Point &start_pos, const Point &end_pos,
                                                      const std::vector<Obstacle> &obstacles)
{
    for (const auto &obstacle: obstacles)
    {
        if (existCollisionBetweenBaselineAndObstacle(start_pos, end_pos, obstacle))
        {
            return true;
        }
    }
    return false;
}

const Point utils::generateRandomPointBetween(const Point &start_pos, const Point &end_pos)
{
    float r_x = u(e), r_y = u(e), r_z = u(e);
    return Point(r_x * start_pos.x() + (1.0f - r_x) * end_pos.x(),
                 r_y * start_pos.y() + (1.0f - r_y) * end_pos.y(),
                 r_z * start_pos.z() + (1.0f - r_z) * end_pos.z());
}

Point utils::getCoordinateInLeftCamera(float x_p_left, float x_p_right, float y_p)
{
    float xc = B * x_p_left / (x_p_left - x_p_right);
    float yc = B * Px * y_p * tanf(Beta / 2) / ((x_p_left - x_p_right) * Py * tanf(Alpha / 2));
    float zc = B * Px / (2 * (x_p_left - x_p_right) * tanf(Alpha / 2));
    return Point(xc, yc, zc);
}

Point utils::convertCoordinateSystem(simxInt handle, const Point &p)
{
    return Point(p.x(), p.y(), p.z());
}

