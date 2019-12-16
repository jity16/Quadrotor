#ifndef LANDING_RISINGSTATE_H
#define LANDING_RISINGSTATE_H


#include "../updater/LandingPathUpdater.h"

class RisingState : public State<LandingPathUpdater>
{
public:
    RisingState();

    void Enter(LandingPathUpdater *t) override;

    void Execute(LandingPathUpdater *t) override;

    void Exit(LandingPathUpdater *t) override;

private:

    Point m_target_pos;

    const simxFloat RISING_HEIGHT = Config::Instance()->getFloatParam("RisingState", "RISING_HEIGHT");

    const float RISING_STEP_LENGTH = Config::Instance()->getFloatParam("RisingState", "RISING_STEP_LENGTH");

    const simxInt WAITING_ROUNDS = Config::Instance()->getIntParam("RisingState", "WAITING_ROUNDS");
};


#endif //LANDING_RISINGSTATE_H
