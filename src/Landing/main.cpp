#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "updater/LandingPathUpdater.h"
#include "updater/LandingVisionUpdater.h"
#include <vector>
#include "../Common/utils/ShallowLearning.h"
#include "../Common/utils/utils.h"

int client_id;
std::vector<std::string> float_signals = Config::Instance()->getStringVectorParam("Main", "CLEAN_FLOAT_SIGNALS");

void clearSignals()
{
    for (auto float_signal: float_signals)
    {
        std::cout << float_signal << std::endl;
        simxClearFloatSignal(client_id, float_signal.c_str(), simx_opmode_blocking);
    }
}

void doSomethingBeforeSimulation()
{
    simxFinish(-1);
    client_id = simxStart("127.0.0.1", 19997, 1, 1, 2000, 5);
    if (client_id == -1)
    {
        std::cout << "Failed to connect to remote API server" << std::endl;
        exit(0);
    }
    std::cout << "Connect to remote API server and cid is " << client_id << std::endl;
    simxStartSimulation(client_id, simx_opmode_blocking);
    clearSignals();
}

void doSomethingAfterSimulation()
{
    clearSignals();
    // stop simulation
    while(simxStopSimulation(client_id, simx_opmode_blocking) != simx_return_ok);
    // close connection
    simxFinish(client_id);
    std::cout << "Close connection to V-REP" << std::endl;
    exit(0);
}

void doSomethingAfterSimulationForLinux(int sig)
{
    doSomethingAfterSimulation();
}

#ifdef __WIN32

bool ctrlHandler(DWORD fdwctrltype)
{
    switch (fdwctrltype)
    {
        case CTRL_C_EVENT:
        {
            doSomethingAfterSimulation();
            return false;
        }
        default:
        {
            return false;
        }
    }
}

#endif


int main(int argc, char const *argv[])
{
#ifdef __WIN32
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) ctrlHandler, true))
    {
        std::cout << "Capture ctrl-c event failed" << std::endl;
        exit(0);
    };
#else
    signal(SIGINT, doSomethingAfterSimulationForLinux);
#endif
    doSomethingBeforeSimulation();

//    std::cout << ShallowLearning::evalParam("test", 100) << std::endl;
//    ShallowLearning::updateParam("test", 1);
//    std::cout << ShallowLearning::evalParam("test", 100) << std::endl;
//    ShallowLearning::updateParam("test", 2);
//    std::cout << ShallowLearning::evalParam("test", 100) << std::endl;
//    ShallowLearning::updateParam("test", 3);
//    std::cout << ShallowLearning::evalParam("test", 100) << std::endl;

    //LandingPathUpdater path_updater(client_id);
    LandingVisionUpdater vision_updater(client_id);
//    std::thread vision([&vision_updater]() {
//        vision_updater.run();
//    });
//    std::thread path([&path_updater]() {
//        path_updater.run();
//    });
//    path.join();
    //vision.join();
    vision_updater.run();
    return 0;
}

