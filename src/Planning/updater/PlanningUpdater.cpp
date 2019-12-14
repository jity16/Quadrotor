#include <iostream>
#include "PlanningUpdater.h"
#include "../../Common/utils/utils.h"
#include "../../Common/config/Handle.h"

bool use_vision = false;
Point cylinder_pos, recog_circle_pos, recog_target_pos, single_circle_pos, single_target_pos;
PlanningUpdater::PlanningUpdater(int time_step, int client_id) : Updater(time_step, client_id)
{
    std::cout << "Try to get necessary handle" << std::endl;
    m_handle_drone = Handle::Instance()->getObjectHandle("Quadricopter_base", m_cid);
    m_handle_target = Handle::Instance()->getObjectHandle("Quadricopter_target", m_cid);
    m_handle_hot_target = Handle::Instance()->getObjectHandle("Target", m_cid);
    std::cout << "Get drone's handle is " << m_handle_drone << " and target's handle is " << m_handle_target << std::endl;
}

const Point PlanningUpdater::getDronePosition()
{
    return utils::getObjectPosition(m_handle_drone, m_cid);
}

const Point PlanningUpdater::getTargetPosition()
{
    return utils::getObjectPosition(m_handle_target, m_cid);
}

void PlanningUpdater::setTargetPosition(Point p)
{
    simxFloat pos[3];
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
    simxSetObjectPosition(m_cid, m_handle_target, -1, pos, simx_opmode_oneshot);
}

void PlanningUpdater::setHand() const
{
    simxSetIntegerSignal(m_cid, "close_hand", 1, simx_opmode_blocking);
}

const Point PlanningUpdater::getHotTargetPosition()
{
    return utils::getObjectPosition(m_handle_hot_target, m_cid);
}

void PlanningUpdater::setPosition(const Point &pos, simxInt handle, simxInt relative_handle)
{
    simxFloat p[3];
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    simxSetObjectPosition(m_cid, handle, relative_handle, p, simx_opmode_oneshot);
}

