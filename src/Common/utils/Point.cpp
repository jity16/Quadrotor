#include <cmath>
#include <cassert>
#include <iosfwd>
#include <ostream>
#include "Point.h"

Point::Point()
{
    pos[0] = pos[1] = pos[2] = 0;
}

Point::Point(const Point &p)
{
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
}

Point::Point(simxFloat x, simxFloat y, simxFloat z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

Point::Point(simxFloat *p)
{
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
}

simxFloat Point::operator[](int index) const
{
    return pos[index];
}

simxFloat Point::x() const
{
    return pos[0];
}

simxFloat Point::y() const
{
    return pos[1];
}

simxFloat Point::z() const
{
    return pos[2];
}

simxFloat Point::length() const
{
    return sqrtf(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
}

const Point Point::operator+(const Point &p) const
{
    return Point(pos[0] + p[0], pos[1] + p[1], pos[2] + p[2]);
}

const Point Point::operator-(const Point &p) const
{
    return Point(pos[0] - p[0], pos[1] - p[1], pos[2] - p[2]);
}

const Point Point::operator/(simxFloat p) const
{
    return Point(pos[0] / p, pos[1] / p, pos[2] / p);
}

simxFloat Point::operator*(const Point &p) const
{
    return pos[0] * p[0] + pos[1] * p[1] + pos[2] * p[2];
}

const Point operator*(const Point &lhs, simxFloat rhs)
{
    return Point(lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs);
}

const Point operator*(simxFloat lhs, const Point &rhs)
{
    return Point(lhs * rhs[0], lhs * rhs[1], lhs * rhs[2]);
}

const Point Point::normalize() const
{
    return *this / this->length();
}

void Point::setX(simxFloat x)
{
    pos[0] = x;
}

void Point::setY(simxFloat y)
{
    pos[1] = y;
}

void Point::setZ(simxFloat z)
{
    pos[2] = z;
}

void Point::set(int ind, simxFloat val)
{
    assert(ind < 3);
    assert(ind >= 0);
    pos[ind] = val;
}

simxFloat Point::norm() const
{
    return std::fmax(fabsf(pos[0]), std::fmax(fabsf(pos[1]), fabsf(pos[2])));
}

std::ostream &operator<<(std::ostream &out, const Point &ls)
{
    out << ls[0] << ' ' << ls[1] << ' ' << ls[2];
    return out;
}

float distance(const Point &lhs, const Point &rhs)
{
    return sqrtf((rhs[0] - lhs[0]) * (rhs[0] - lhs[0]) + (rhs[1] - lhs[1]) * (rhs[1] - lhs[1]) +
                 (rhs[2] - lhs[2]) * (rhs[2] - lhs[2]));
}

