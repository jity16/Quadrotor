#ifndef LANDING_APPROCHINGSTATE_H
#define LANDING_APPROCHINGSTATE_H


#include "../updater/LandingPathUpdater.h"

class ApproachingState : public State<LandingPathUpdater>
{
public:
    ApproachingState();

    void Enter(LandingPathUpdater *t) override;

    void Execute(LandingPathUpdater *t) override;

    void Exit(LandingPathUpdater *t) override;

private:

    const simxFloat APPROACHING_OVER_HEIGHT = Config::Instance()->getFloatParam("ApproachingState", "APPROACHING_OVER_HEIGHT");
    const simxFloat APPROACHING_STEP_LENGTH = Config::Instance()->getFloatParam("ApproachingState", "APPROACHING_STEP_LENGTH");
    const simxFloat RELATIVE_LANDING_BEGIN_HEIGHT = Config::Instance()->getFloatParam("ApproachingState", "RELATIVE_LANDING_BEGIN_HEIGHT");
    const simxFloat RELATIVE_LANDING_END_HEIGHT = Config::Instance()->getFloatParam("ApproachingState", "RELATIVE_LANDING_END_HEIGHT");
    const simxFloat CLOSE_THRESHOLD = Config::Instance()->getFloatParam("ApproachingState", "CLOSE_THRESHOLD");
};


#endif //LANDING_APPROCHINGSTATE_H
