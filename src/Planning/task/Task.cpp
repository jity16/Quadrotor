#include "Task.h"
#include "../../Common/utils/ShallowLearning.h"

void Task::Enter(PlanningPathUpdater *t)
{
    m_start_time = clock();
}

void Task::Exit(PlanningPathUpdater *t)
{
    m_finish_time = clock();
    ShallowLearning::updateParam(getName(), m_finish_time - m_start_time);
}

float Task::evalTimeCost(float default_value)
{
    return ShallowLearning::evalParam(getName(), default_value);
}

void Task::Fail()
{
    ShallowLearning::updateParam(getName(), 1000000000);
}
