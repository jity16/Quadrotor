#ifndef LANDING_VISIONPLANNER_H
#define LANDING_VISIONPLANNER_H


#include "LandingUpdater.h"
#include "../../Common/config/Config.h"

class LandingVisionUpdater : public LandingUpdater
{
public:
    LandingVisionUpdater(int client_id);

private:

    void update() override;
};


#endif //LANDING_VISIONPLANNER_H
