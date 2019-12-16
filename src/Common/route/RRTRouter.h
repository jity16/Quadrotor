//
// Created by eric on 19-7-23.
//

#ifndef DRONE_RRTROUTER_H
#define DRONE_RRTROUTER_H

#include "Router.h"

class RRTRouter: public Router
{
public:
    std::vector<Point> route(const Point &start_pos, const Point &end_pos) override;
};


#endif //DRONE_RRTROUTER_H
