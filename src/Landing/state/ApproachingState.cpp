#include <iostream>
#include "ApproachingState.h"
#include "math.h"
#include "LandingState.h"

void ApproachingState::Enter(LandingPathUpdater *t)
{
    std::cout << "now begin approaching to the car" << std::endl;
    Point plane_pos = t->getPlanePosition();
    t->setLandingSignal(plane_pos[2] + RELATIVE_LANDING_BEGIN_HEIGHT, plane_pos[2] + RELATIVE_LANDING_END_HEIGHT);
    std::cout << "adjust landing height to " << plane_pos[2] + RELATIVE_LANDING_BEGIN_HEIGHT << " and "
              << plane_pos[2] + RELATIVE_LANDING_END_HEIGHT << std::endl;
}

void ApproachingState::Execute(LandingPathUpdater *t)
{
    Point target_pos = t->getTargetPosition();
    Point plane_pos = t->getPlanePosition();
    plane_pos.setZ(plane_pos[2] + APPROACHING_OVER_HEIGHT);
    Point error = plane_pos - target_pos;
    std::cout << "plane_pos: " << plane_pos << std::endl;
    std::cout << "error: " << error << std::endl;
    std::cout << "target_pos: " << target_pos << std::endl;
    target_pos = target_pos + error.normalize() * std::min(APPROACHING_STEP_LENGTH, error.length());
    std::cout << "new_target_pos: " << target_pos << std::endl;
    t->setTargetPosition(target_pos);
    if(error.norm() < CLOSE_THRESHOLD)
    {
        t->changeState(new LandingState);
    }
}

void ApproachingState::Exit(LandingPathUpdater *t)
{
    std::cout << "finished approaching, now we are suspending above the car" << std::endl;
}

ApproachingState::ApproachingState()
{

}
