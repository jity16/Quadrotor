#include <iostream>
#include <math.h>
#include "RisingState.h"
#include "ApproachingState.h"
#include "WaitState.h"

RisingState::RisingState() {}

void RisingState::Enter(LandingPathUpdater *t)
{
    std::cout << "now begin rising to high" << std::endl;
    m_target_pos = t->getDronePosition();
}

void RisingState::Execute(LandingPathUpdater *t)
{
    //Point drone_pos = m_target_pos;
    //Point drone_pos = t->getTargetPosition();
    //std::cout << drone_pos[0] << " " << drone_pos[1] << " " << drone_pos[2] << std::endl;
    //std::cout << t->getTargetPosition() << std::endl;
    m_target_pos.setZ(m_target_pos[2] + RISING_STEP_LENGTH);
    std::cout << "Set target position: " << m_target_pos << std::endl;
    t->setTargetPosition(m_target_pos);
    Point qrcode_pos = t->getQRcodePosition();
    std::cout << "QRcode position: " << qrcode_pos << std::endl;
    // check leave
    if(fabsf(qrcode_pos.z()) > 0.01 && fabsf(qrcode_pos.z() + 1) > 0.01)
    {
        std::cout << "find QRcode !!!" << std::endl;
        t->changeState(new WaitState(WAITING_ROUNDS, new ApproachingState));
    }

}

void RisingState::Exit(LandingPathUpdater *t)
{
    std::cout << "rising to height " << RISING_HEIGHT << " and prepare to leave" << std::endl;
}
