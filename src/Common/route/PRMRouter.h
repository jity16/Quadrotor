#ifndef DRONE_PRMROUTER_H
#define DRONE_PRMROUTER_H


#include "Router.h"

class PRMRouter: public Router
{
public:
    std::vector<Point> route(const Point &start_pos, const Point &end_pos) override;
};


#endif //DRONE_PRMROUTER_H
