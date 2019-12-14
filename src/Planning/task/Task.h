#ifndef DRONE_TASK_H
#define DRONE_TASK_H


#include "../../Common/state/State.h"
#include <ctime>


class PlanningPathUpdater;
class Task : public State<PlanningPathUpdater>
{
public:
    void Enter(PlanningPathUpdater *t) override;

    void Exit(PlanningPathUpdater *t) override;

    virtual float evalTimeCost(float default_value);

    virtual const std::string getName() = 0;

    void Fail();

private:
    clock_t m_start_time{0}, m_finish_time{0};
};


#endif //DRONE_TASK_H
