#include <iostream>
#include "FindTask.h"
#include "../updater/PlanningPathUpdater.h"
#include "../../Common/utils/utils.h"

FindTask::FindTask(Object *object):m_object(object)
{

}

void FindTask::Enter(PlanningPathUpdater *t)
{
    Task::Enter(t);
    std::cout << "Prepare to find the end " << m_object->getName() << std::endl;
    std::cout << "Open the eyes _(:з)∠)_" << std::endl;
    use_vision = true;
    utils::sleep(MICRO_SECONDS_FOR_DISCRIMINATE);
    use_vision = false;
    m_target = utils::getObjectPosition("CircleDummy", t->getCid());
    std::cout << "circle_pos = " << m_target << std::endl;
    m_target.setX(-7.45f + utils::getRandom(0.1f));
    m_target.setY(8.2f + utils::getRandom(0.1f));
    m_target.setZ(HOVER_HEIGHT);
    std::cout << "end position is " << m_target << std::endl;
    t->setTargetPosition(m_target);
    m_close_rounds = 0;
}

void FindTask::Exit(PlanningPathUpdater *t)
{
    Task::Exit(t);
    std::cout << "Success to find position of " << m_object->getName() << std::endl;
}

void FindTask::Execute(PlanningPathUpdater *t)
{
    if ((t->getDronePosition() -m_target).norm() < CLOSE_THRESHOLD)
    {
        m_close_rounds++;
    } else
    {
        m_close_rounds = 0;
    }
    if (m_close_rounds >= CLOSE_ROUNDS_LIMIT)
    {
        t->finishCurrentTask();
    }

    if (m_close_rounds != 0)
    {
        std::cout << "close rounds is " << m_close_rounds << std::endl;
    }
}

const std::string FindTask::getName()
{
    return "Find_Circle_On_" + m_object->getName();
}
