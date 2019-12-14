#ifndef DRONE_GRABTASK_H
#define DRONE_GRABTASK_H


#include "Task.h"
#include "../../Common/object/Object.h"

class GrabTask : public Task
{
public:
    //not a good implement, lead to shared ptr

    explicit GrabTask(Object *object);

    void Enter(PlanningPathUpdater *t) override;

    void Exit(PlanningPathUpdater *t) override;

    void Execute(PlanningPathUpdater *t) override;

    const std::string getName() override;

private:
    Object *m_object;
    std::vector<Point> m_path_points;
    int m_now_target_index;
    int m_close_rounds;
    const simxFloat CLOSE_THRESHOLD = Config::Instance()->getFloatParam("GrabTask", "CLOSE_THRESHOLD");
    const simxInt CLOSE_ROUNDS_LIMIT = Config::Instance()->getIntParam("GrabTask", "CLOSE_ROUNDS_LIMIT");
    const simxFloat GRAB_START_HEIGHT = Config::Instance()->getFloatParam("GrabTask", "GRAB_START_HEIGHT");
    const simxInt MICRO_SECONDS_FOR_DISCRIMINATE = Config::Instance()->getIntParam("GrabTask",
                                                                                   "MICRO_SECONDS_FOR_DISCRIMINATE");
};


#endif //DRONE_GRABTASK_H
