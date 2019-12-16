#ifndef LANDING_WAITSTATE_H
#define LANDING_WAITSTATE_H

#include "../updater/LandingPathUpdater.h"

class WaitState : public State<LandingPathUpdater>
{
public:

    WaitState(int remain_rounds, State *next_state);

    void Enter(LandingPathUpdater *t) override;

    void Execute(LandingPathUpdater *t) override;

    void Exit(LandingPathUpdater *t) override;


private:
    int m_remain_rounds;
    State *m_next_state;
};


#endif //LANDING_WAITSTATE_H
