#ifndef LANDING_LANDINGSTATE_H
#define LANDING_LANDINGSTATE_H

#include "../updater/LandingPathUpdater.h"

class LandingState : public State<LandingPathUpdater>
{
public:
    LandingState();

    void Enter(LandingPathUpdater *t) override;

    void Execute(LandingPathUpdater *t) override;

    void Exit(LandingPathUpdater *t) override;

private:
    const simxFloat RELATIVE_LANDING_HEIGHT = Config::Instance()->getFloatParam("LandingState", "RELATIVE_LANDING_HEIGHT");
    const simxFloat LANDING_STEP_LENGTH = Config::Instance()->getFloatParam("LandingState", "LANDING_STEP_LENGTH");
    const simxFloat CLOSE_THRESHOLD = Config::Instance()->getFloatParam("LandingState", "CLOSE_THRESHOLD");
    const simxInt KEEP_CLOSE_ROUNDS = Config::Instance()->getIntParam("LandingState", "KEEP_CLOSE_ROUNDS");
    int close_rounds = 0;
};


#endif //LANDING_LANDINGSTATE_H
