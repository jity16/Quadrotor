//
// Created by eric on 19-7-25.
//

#ifndef DRONE_RRTRPCROUTER_H
#define DRONE_RRTRPCROUTER_H

#include "Router.h"
#include "../RRT/RRTClient.h"

class RRTRpcRouter: public Router
{
public:
    RRTRpcRouter();
    std::vector<Point> route(const Point &start_pos, const Point &end_pos) override;

private:
    RRTClient rrtClient;
};


#endif //DRONE_RRTRPCROUTER_H
