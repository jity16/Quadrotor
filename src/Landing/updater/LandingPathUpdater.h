#ifndef LANDING_PATHPLANER_H
#define LANDING_PATHPLANER_H


#include "LandingUpdater.h"
#include "../../Common/state/State.h"

class LandingPathUpdater : public LandingUpdater
{
public:
    explicit LandingPathUpdater(simxInt);

    void changeState(State<LandingPathUpdater> *);

    void setLandingFinished();

    Point getQRcodePosition();

private:

    void update() override;

    State<LandingPathUpdater> *m_current_state;
};

#endif //LANDING_PATHPLANER_H
