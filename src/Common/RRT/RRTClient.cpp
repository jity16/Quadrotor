//
// Created by eric on 19-7-24.
//

#include "route_guide.pb.h"
#include "RRTClient.h"

RRTClient::RRTClient():
        m_stub ( RouteGuide::NewStub(grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials())))
{
}

bool RRTClient::setObstacles(std::vector<double> obstacles)
{
    Points points;
    ClientContext context;
    Obstacles response;

    for  (auto &v:obstacles)
    {
        points.add_coordinate(v);
    }
    auto status = m_stub->AddObstacles(&context, points, &response);
    return status.ok();
}

bool
RRTClient::getRoute(Point source, Point destination, std::vector<Point> &route)
{
    Points points;
    ClientContext context;
    Points response;

    points.add_coordinate(source.x());
    points.add_coordinate(source.y());
    points.add_coordinate(source.z());
    points.add_coordinate(destination.x());
    points.add_coordinate(destination.y());
    points.add_coordinate(destination.z());
    auto status = m_stub->GetRrtRoute(&context, points, &response);
//    printf("DEBUG: response size(double number)=%d\n", response.coordinate_size());
    for (int i = 0; i < response.coordinate_size(); i += 3)
    {
        route.emplace_back(response.coordinate(i), response.coordinate(i + 1),
                           response.coordinate(i + 2));
    }
    return status.ok();
}
