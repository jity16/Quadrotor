#ifndef DRONE_PLANNINGUPDATER_H
#define DRONE_PLANNINGUPDATER_H


#include "../../Common/updater/Updater.h"
extern bool use_vision;
extern Point cylinder_pos;
extern Point recog_target_pos, recog_circle_pos;
extern Point single_target_pos, single_circle_pos;
class PlanningUpdater: public Updater
{
public:
    explicit PlanningUpdater(int time_step=0, int client_id=-1);

    const Point getDronePosition();

    const Point getTargetPosition();

    void setTargetPosition(Point p);

    void setPosition(const Point &pos, simxInt handle, simxInt relative_handle);

    void setHand() const;

    const Point getHotTargetPosition();

private:
    simxInt m_handle_drone, m_handle_target, m_handle_hot_target;
};


#endif //DRONE_PLANNINGUPDATER_H
