#ifndef DRONE_THROUGHDOORTASK_H
#define DRONE_THROUGHDOORTASK_H


#include "Task.h"
#include "../../Common/object/Object.h"

class ThroughDoorTask : public Task
{
public:
    explicit ThroughDoorTask(Object *object);

    void Execute(PlanningPathUpdater *t) override;

    const std::string getName() override;

    void Enter(PlanningPathUpdater *t) override;

    void Exit(PlanningPathUpdater *t) override;

private:
    int m_close_rounds;
    Point m_start_pos, m_target_pos, m_previous_round_pos;
    Object *m_object;
    const simxFloat Y_DISTANCE_FROM_DOOR = Config::Instance()->getFloatParam("ThroughDoorTask", "Y_DISTANCE_FROM_DOOR");
    const simxFloat MOVE_STEP_LENGTH = Config::Instance()->getFloatParam("ThroughDoorTask", "MOVE_STEP_LENGTH");
    const simxFloat CLOSE_THRESHOLD = Config::Instance()->getFloatParam("ThroughDoorTask", "CLOSE_THRESHOLD");
    const simxInt CLOSE_ROUNDS_LIMIT = Config::Instance()->getIntParam("ThroughDoorTask", "CLOSE_ROUNDS_LIMIT");
};


#endif //DRONE_THROUGHDOORTASK_H
