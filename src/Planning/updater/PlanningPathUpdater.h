#ifndef DRONE_PLANNINGPATHUPDATER_H
#define DRONE_PLANNINGPATHUPDATER_H


#include <vector>
#include "PlanningUpdater.h"
#include "../../Common/config/Config.h"
#include "../../Common/route/Router.h"

class Object;
class Task;
class PlanningPathUpdater: public PlanningUpdater
{
public:
    explicit PlanningPathUpdater(int client_id);

    void update() override;

    virtual ~PlanningPathUpdater();

    void startTasks();

    void finishCurrentTask();

    void finishTasks();

    void failCurrentTask();

    std::vector<Point> getPathPoints(const Point &start_pos, const Point &end_pos);


private:
    std::vector<Object *> m_objects;
    std::vector<Task *> m_tasks;
    int m_running_task_index;
    int crash_rounds;
    const simxFloat CRASH_HEIGHT = Config::Instance()->getFloatParam("PlanningPathUpdater", "CRASH_HEIGHT");
    const int CRASH_ROUNDS_THRESHOLD = Config::Instance()->getIntParam("PlanningPathUpdater", "CRASH_ROUNDS_THRESHOLD");
    Router *m_router, *m_distance_evaluation_router;
    const std::string ROUTE_ALGORITHM = Config::Instance()->getStringParam("PlanningPathUpdater", "ROUTE_ALGORITHM");
    const std::string DISTANCE_EVALUATION_ALGORITHM = Config::Instance()->getStringParam("PlanningPathUpdater", "DISTANCE_EVALUATION_ALGORITHM");
};


#endif //DRONE_PLANNINGPATHUPDATER_H
