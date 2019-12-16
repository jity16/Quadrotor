#ifndef LANDING_LANDINGUPDATER_H
#define LANDING_LANDINGUPDATER_H

#include "../../Common/updater/Updater.h"
extern Point QRCode_pos;
class LandingUpdater: public Updater
{
public:

    explicit LandingUpdater(int time_step=0, int client_id=-1);

    const Point getDronePosition();

    const Point getCarPosition();

    const Point getTargetPosition();

    const Point getPlanePosition();

    void setTargetPosition(Point p);

    void setLandingSignal(simxFloat landing_begin_height, simxFloat landing_end_height);

protected:
    simxInt m_handle_drone, m_handle_target, m_handle_car, m_handle_plane;

};


#endif //LANDING_LANDINGUPDATER_H
