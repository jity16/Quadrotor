#ifndef LANDING_FINISHSTATE_H
#define LANDING_FINISHSTATE_H


#include "../updater/LandingPathUpdater.h"

class FinishState: public State<LandingPathUpdater>
{
public:
    FinishState();

    void Enter(LandingPathUpdater *t) override;

    void Execute(LandingPathUpdater *t) override;

    void Exit(LandingPathUpdater *t) override;
};


#endif //LANDING_FINISHSTATE_H
