//
// Created by eric on 19-7-24.
//

#ifndef DRONE_RRTCLIENT_H
#define DRONE_RRTCLIENT_H


#include <memory>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "route_guide.grpc.pb.h"
#include "route_guide.pb.h"
#include "../utils/Point.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using routeguide::Points;

using routeguide::Obstacles;
using routeguide::RouteSummary;
using routeguide::RouteGuide;

class RRTClient
{
public:
    RRTClient();
    bool setObstacles(std::vector<double> obstacles);
    bool getRoute(Point source, Point destination, std::vector<Point> &route);

private:
    std::unique_ptr<RouteGuide::Stub> m_stub;
};


#endif //DRONE_RRTCLIENT_H
