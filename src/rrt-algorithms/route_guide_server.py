# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""The Python implementation of the gRPC route guide server."""

from concurrent import futures
import time
import math
import logging

import grpc

import route_guide_pb2
import route_guide_pb2_grpc
import rrt_star_bid_h_3d
# import route_guide_resources

_ONE_DAY_IN_SECONDS = 60 * 60 * 24


def addObstacles(points):
    """Returns Feature at given location or None."""
    assert(len(points)%6==0)
    obs=[]
    for i in range(0,len(points),6):
        obs.append([points[i+j] for j in range(6)])
    rrt_star_bid_h_3d.set_obstacles(obs)
    logging.info('# Points of obstacles= %d. Success!'%(len(points), ))
    return route_guide_pb2.Obstacles(isSuccess=True)

def getRrtRoute(points):
    assert(len(points)==6)
    route=rrt_star_bid_h_3d.sovle_rrt((points[0],points[1],points[2]),(points[3],points[4],points[5]))
    ret=route_guide_pb2.Points()
    print('route=', route)
    for p in route:
        ret.coordinate.extend(p)
    print(route, 'return len= ', len(ret.coordinate))
    return ret


class RouteGuideServicer(route_guide_pb2_grpc.RouteGuideServicer):
    """Provides methods that implement functionality of route guide server."""

    def __init__(self):
        None
        # self.db = route_guide_resources.read_route_guide_database()

    def AddObstacles(self, request, context):
        logging.info('RPC AddObstacles')
        ret = addObstacles(request.coordinate)
        return ret

    def GetRrtRoute(self, request, context):
        logging.info('RPC GetRrtRoute')
        ret=getRrtRoute(request.coordinate)
        return ret


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=2))
    route_guide_pb2_grpc.add_RouteGuideServicer_to_server(
        RouteGuideServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    logging.info('Server start. Listening on port %d'%(50051,))
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO )
    serve()
