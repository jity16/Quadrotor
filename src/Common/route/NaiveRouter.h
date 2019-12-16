#ifndef DRONE_NAIVEROUTER_H
#define DRONE_NAIVEROUTER_H


#include "Router.h"

class NaiveRouter: public Router
{
public:
    std::vector<Point> route(const Point &start_pos, const Point &end_pos) override;
};


#endif //DRONE_NAIVEROUTER_H
