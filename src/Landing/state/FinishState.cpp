#include <iostream>
#include "FinishState.h"

FinishState::FinishState()
{

}

void FinishState::Enter(LandingPathUpdater *t)
{
    std::cout << "now task finished, just keep like this and don't move" << std::endl;
    t->setLandingFinished();
}

void FinishState::Execute(LandingPathUpdater *t)
{

}

void FinishState::Exit(LandingPathUpdater *t)
{

}
