#ifndef DRONE_PLANNINGVISIONUPDATER_H
#define DRONE_PLANNINGVISIONUPDATER_H


#include "PlanningUpdater.h"

class PlanningVisionUpdater: public PlanningUpdater
{
public:
    explicit PlanningVisionUpdater(int client_id);
    void update() override;
};


#endif //DRONE_PLANNINGVISIONUPDATER_H
