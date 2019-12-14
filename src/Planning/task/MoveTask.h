#ifndef DRONE_MOVETASK_H
#define DRONE_MOVETASK_H


#include "Task.h"
#include "../../Common/object/Object.h"
#include "../../Common/route/Router.h"

class MoveTask : public Task
{
public:
    explicit MoveTask(Object *source, Object *destination);

    void Enter(PlanningPathUpdater *t) override;

    void Exit(PlanningPathUpdater *t) override;

    void Execute(PlanningPathUpdater *t) override;

    const std::string getName() override;

    float getDistance(Router *router) const;

private:
    int m_close_rounds;
    Object *m_source_object;
    Object *m_destination_object;
    Point m_start_pos, m_target_pos, m_previous_round_pos;
    std::vector<Point> m_path_points;
    int m_now_target_index;
    const simxFloat CLOSE_THRESHOLD = Config::Instance()->getFloatParam("MoveTask", "CLOSE_THRESHOLD");
    const simxInt CLOSE_ROUNDS_LIMIT = Config::Instance()->getIntParam("MoveTask", "CLOSE_ROUNDS_LIMIT");
    const simxFloat MIDDLE_CLOSE_THRESHOLD = Config::Instance()->getFloatParam("MoveTask", "MIDDLE_CLOSE_THRESHOLD");
    const simxInt MIDDLE_CLOSE_ROUNDS_LIMIT = Config::Instance()->getIntParam("MoveTask", "MIDDLE_CLOSE_ROUNDS_LIMIT");
};


#endif //DRONE_MOVETASK_H
