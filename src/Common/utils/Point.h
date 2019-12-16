#ifndef LANDING_POINT_H
#define LANDING_POINT_H
extern "C" {
#include "../vrep/extApi.h"
}
#include <ostream>

class Point
{
public:
    Point();
    Point(const Point &p);
    Point(simxFloat x, simxFloat y, simxFloat z);
    explicit Point(simxFloat p[3]);
    simxFloat operator[](int index) const;
    simxFloat x() const;
    simxFloat y() const;
    simxFloat z() const;
    simxFloat length() const;
    void setX(simxFloat x);
    void setY(simxFloat y);
    void setZ(simxFloat z);
    void set(int ind, simxFloat val);
    const Point operator +(const Point &p) const;
    const Point operator -(const Point &p) const;
    const Point operator /(simxFloat p) const;
    const Point normalize() const;
    simxFloat operator *(const Point &p) const;
    simxFloat norm() const;
    friend const Point operator *(const Point &lhs, simxFloat rhs);
    friend const Point operator *(simxFloat lhs, const Point &rhs);
    friend std::ostream &operator <<(std::ostream &out, const Point &ls);
    friend float distance(const Point &lhs, const Point &rhs);
private:
    simxFloat pos[3];
};


#endif //LANDING_POINT_H
