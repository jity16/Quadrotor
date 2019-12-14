#ifndef DRONE_FINDTASK_H
#define DRONE_FINDTASK_H


#include "../../Common/object/Object.h"

class FindTask: public Task
{
public:
    //not a good implement, lead to shared ptr

    explicit FindTask(Object *object);

    void Enter(PlanningPathUpdater *t) override;

    void Exit(PlanningPathUpdater *t) override;

    void Execute(PlanningPathUpdater *t) override;

    const std::string getName() override;

private:
    Object *m_object;
    int m_close_rounds;
    Point m_target;
    const simxFloat CLOSE_THRESHOLD = Config::Instance()->getFloatParam("FindTask", "CLOSE_THRESHOLD");
    const simxInt CLOSE_ROUNDS_LIMIT = Config::Instance()->getIntParam("FindTask", "CLOSE_ROUNDS_LIMIT");
    const simxFloat HOVER_HEIGHT = Config::Instance()->getFloatParam("FindTask", "HOVER_HEIGHT");
    const simxInt MICRO_SECONDS_FOR_DISCRIMINATE = Config::Instance()->getIntParam("GrabTask",
                                                                                   "MICRO_SECONDS_FOR_DISCRIMINATE");
};


#endif //DRONE_FINDTASK_H
