file(GLOB_RECURSE SRC *.cpp *.cc *.h *.hpp *.c)

message(AUTHOR_WARNING SRC ${SRC})
list(FILTER SRC EXCLUDE REGEX ".*RRT/main.cpp$")
if (NOT BUILD_RRTRPC)
    list(FILTER SRC EXCLUDE REGEX ".*RRT/route_guide.grpc.pb.cc$")
    list(FILTER SRC EXCLUDE REGEX ".*route_guide.pb.cc$")
    list(FILTER SRC EXCLUDE REGEX ".*RRT/route_guide.grpc.pb.h$")
    list(FILTER SRC EXCLUDE REGEX ".*RRT/route_guide.pb.h$")
    list(FILTER SRC EXCLUDE REGEX ".*RRT/RRTClient.cpp$")
    list(FILTER SRC EXCLUDE REGEX ".*RRT/RRTClient.h$")
    list(FILTER SRC EXCLUDE REGEX ".*route/RRTRpcRouter.cpp$")
    list(FILTER SRC EXCLUDE REGEX ".*route/RRTRpcRouter.h$")
#
#    list(REMOVE_ITEM SRC RRT/route_guide.grpc.pb.cc
#            RRT/route_guide.pb.cc
#            RRT/route_guide.grpc.pb.h
#            RRT/route_guide.pb.h
#            RRT/RRTClient.cpp
#            RRT/RRTClient.h
#            route/RRTRpcRouter.cpp
#            route/RRTRpcRouter.h
#            )
endif(NOT BUILD_RRTRPC)

add_library(common ${SRC})
if (NOT BUILD_RRTRPC)
    message(AUTHOR_WARNING NOT ${BUILD_RRTRPC})
else(NOT BUILD_RRTRPC)
    message(AUTHOR_WARNING ON ${BUILD_RRTRPC})
endif(NOT BUILD_RRTRPC)

message(AUTHOR_WARNING ${SRC})

#set(PYTHON_EXECUTABLE:FILEPATH /home/eric/App/anaconda3/bin/python)
#set(PYTHON_INCLUDE_DIR:PATH /home/eric/App/anaconda3/include)
#set(PYTHON_LIBRARY:FILEPATH /home/eric/App/anaconda3/lib/libpython3.6m.a)
#set(PYTHON_LIBRARIES /home/eric/App/anaconda3/lib/libpython3.6m.a)
#find_package(PythonLibs REQUIRED)
#find_package(PythonLibs 3.6 REQUIRED)
#find_package(PythonLibs 3.6 REQUIRED PATHS /home/eric/App/anaconda3)
#message(AUTHOR_WARNING ${PYTHON_INCLUDE_DIRS})
#message(AUTHOR_WARNING ${PYTHON_LIBRARIES})
#include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(/home/eric/.local/include)
file(GLOB RRT_source RRT/*)
add_executable(rrt ${RRT_source})

target_link_libraries(rrt common)
target_link_libraries(rrt -lprotobuf -pthread -lpthread)
target_link_libraries(rrt grpc++ grpc++_reflection)

if (BUILD_RRTRPC)
    link_libraries(common -lprotobuf -pthread -lpthread)
    link_libraries(common  grpc++ grpc++_reflection)
endif(BUILD_RRTRPC)
