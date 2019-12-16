#include <iostream>
#include "RRT.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#ifdef BAZEL_BUILD
#include "examples/protos/route_guide.grpc.pb.h"
#else
#include "route_guide.grpc.pb.h"
#endif
#include "RRTClient.h"
using namespace std;

const vector< double> obstacles={1.900000,-9.500062,0.000019,3.100000,-9.399932,0.899974,
                    1.675000,4.574937,0.000019,2.875001,4.675068,0.899974,
                    -8.099999,-8.050064,0.000019,-6.899999,-7.949934,0.899974,
                    6.175003,0.799998,0.000009,7.075003,1.299998,1.949987,
                    -0.550000,-6.275035,0.000009,0.350000,-6.224969,1.949987,
                    -3.450005,-1.150002,0.000009,-2.550004,-0.650002,1.949987,
                    -7.650001,3.249966,0.000009,-6.750000,3.300031,1.949987,
                    -1.330822,-0.260683,-0.091294,1.179628,2.339342,4.671947,
                    6.229515,-6.515163,-0.070539,8.169290,-4.506176,3.609922,
                    -7.950044,-1.299933,-0.000000,-7.150044,-0.499933,2.000000,
                    4.532753,-4.217207,0.000000,5.816997,-2.932963,1.284244,
                    -2.340466,-4.140426,0.000000,-1.059785,-2.859744,1.280681,
                    10.194996,-0.055002,-0.755000,11.154997,0.904999,0.205000,
                    -7.954000,7.696000,-0.129000,-6.945999,8.704000,0.879000,
                    6.719998,-10.880001,-0.755000,7.679998,-9.920000,0.205000
};

int main()
{
    RRTClient client;
    vector<Point> route;

    cout << client.setObstacles(obstacles) << endl;
    cout << client.getRoute(Point(10.7016, 0.472867, 0.813236),
                            Point(-5, 0, 1), route) << endl;
    std::cout << "size=" << route.size() << endl;
    for (const auto &p : route)
    {
        printf("%lf %lf %lf\n", p.x(), p.y(), p.z());
    }
}

int main_python_embedded()
{
//    std::cout<<"Hello world\n";
//    RRT rrt;
//    rrt.addObstacle(Point(1, 1, 1), Point(2, 2, 2));
//    std::vector<Point> route;
//    auto success = rrt.generateRoute(Point(0, 0, 0), Point(3, 3, 3), route);
//    std::cout << success;
//    for (auto p:route)
//    {
//        printf("%.3lf %.3lf %.3lf\n", p.x(), p.y(), p.z());
//    }

//    Py_Initialize();
//
//    char cwd[1000];
//    getcwd(cwd, 1000);
////    string path = string(cwd);
//    string path = "~/Work/XiaoXueQi/rrt-algorithms";
//
//    cerr << path << endl;
//    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
//    const char* cstr_cmd = chdir_cmd.c_str();
//    PyRun_SimpleString("import sys");
//    PyRun_SimpleString(cstr_cmd);
//
//    PyObject *use_int, *use_str, *use_byte, *use_tuple;
//    PyObject *use_list, *use_dict, *use_complex;
//    PyObject *pName, *pModule, *pFunc;
//    PyObject *pArgs, *pValue;
//    const char* module_name = "examples.rrt_bid_h.rrt_star_bid_h_3d";
//    const char* module_method = "solve_rrt";
//
//
//    use_int = Py_BuildValue("i", 123);
//    use_str = Py_BuildValue("s", "hello");
//    use_byte = Py_BuildValue("y", "hello2");
//    use_tuple = Py_BuildValue("(iis)", 1, 2, "three");
//    use_list = Py_BuildValue("[iis]", 1, 2, "three");
//    use_dict = Py_BuildValue("{s:i,s:i}", "abc", 123, "def", 456);
//    use_complex = Py_BuildValue("[ii{ii}(is){s:i}]", 1,2,3,4,5,"xcxcv","ff",1);
//    vector<PyObject *> py_double;
//    PyObject *my_list = PyList_New(0);
//    for (int i = 0; i < 90; i += 6)
//    {
//        auto my_tuple=Py_BuildValue("dddddd", obstacles[i + 0], obstacles[i + 1],
//                      obstacles[i + 2], obstacles[i + 3], obstacles[i + 4],
//                      obstacles[i + 5]);
//
//        if(PyList_Append(my_list, my_tuple) == -1) {
//            assert(0);
//        }
//    }
//
//    pName = PyUnicode_DecodeFSDefault(module_name);
//    pModule = PyImport_Import(pName);
//    Py_DECREF(pName);
//
//    if (pModule != NULL)
//    {
//        pFunc = PyObject_GetAttrString(pModule, module_method);
//        if (pFunc && PyCallable_Check(pFunc))
//        {
////            pArgs = PyTuple_New(7);
////            PyTuple_SetItem(pArgs, 0, use_int);
////            PyTuple_SetItem(pArgs, 1, use_str);
////            PyTuple_SetItem(pArgs, 2, use_byte);
////            PyTuple_SetItem(pArgs, 3, use_list);
////            PyTuple_SetItem(pArgs, 4, use_tuple);
////            PyTuple_SetItem(pArgs, 5, use_dict);
////            PyTuple_SetItem(pArgs, 6, use_complex);
//
//            pValue = PyObject_CallObject(pFunc, my_list);
//            Py_DECREF(pArgs);
//            if (pValue != NULL)
//            {
//                int ret_int;
//                char *ret_str, *ret_byte;
//                PyObject* ret_list, *ret_tuple, *ret_dict, *ret_complex;
//                //解析元组
//                PyArg_ParseTuple(pValue, "isyOOOO", &ret_int, &ret_str, &ret_byte, &ret_list,&ret_tuple,&ret_dict,&ret_complex);
//                Py_DECREF(pValue);
//            }
//            else {
//                Py_DECREF(pFunc);
//                Py_DECREF(pModule);
//                PyErr_Print();
//                fprintf(stderr, "Call failed\n");
//            }
//        }
//        else
//        {
//            if (PyErr_Occurred())
//                PyErr_Print();
//            fprintf(stderr, "Cannot find function \"%s\"\n", module_method);
//        }
//        Py_XDECREF(pFunc);
//        Py_DECREF(pModule);
//    }
//    else
//    {
//        PyErr_Print();
//        fprintf(stderr, "Failed to load \"%s\"\n", module_name);
//    }
//    Py_FinalizeEx();
}
