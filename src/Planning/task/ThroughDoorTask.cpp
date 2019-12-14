#include <iostream>
#include "ThroughDoorTask.h"
#include "../updater/PlanningPathUpdater.h"

void ThroughDoorTask::Execute(PlanningPathUpdater *t)
{
    Point now_pos = t->getTargetPosition();
    //根据在门的哪边来确定将哪儿设为终点

    //std::cout << error << ' ' << error.norm() << std::endl;
    //std::cout << (t->getDronePosition() - m_target_pos).norm() << std::endl;
    if ((t->getDronePosition() - m_target_pos).norm() < CLOSE_THRESHOLD)
    {
        m_close_rounds++;
    } else
    {
        m_close_rounds = 0;
    }
    if (m_close_rounds != 0)
    {
        std::cout << "close rounds is " << m_close_rounds << std::endl;
    }
    if (m_close_rounds >= CLOSE_ROUNDS_LIMIT)
    {
        t->finishCurrentTask();
    }
    //m_previous_round_pos = t->getDronePosition();
}

const std::string ThroughDoorTask::getName()
{
    return "Through_Door_Of_" + m_object->getName();
}

void ThroughDoorTask::Enter(PlanningPathUpdater *t)
{
    Task::Enter(t);
    std::cout << "Prepare to through the door " << m_object->getName() << std::endl;
    m_previous_round_pos = m_start_pos = t->getDronePosition();
    if (m_start_pos.y() > m_object->getPosition().y())
    {
        m_target_pos = m_object->getPosition() - Point(0, Y_DISTANCE_FROM_DOOR, 0);
    } else
    {
        m_target_pos = m_object->getPosition() + Point(0, Y_DISTANCE_FROM_DOOR, 0);
    }
    t->setTargetPosition(m_target_pos);
}

void ThroughDoorTask::Exit(PlanningPathUpdater *t)
{
    Task::Exit(t);
    std::cout << "Success to through the door " << m_object->getName() << " and go to next task" << std::endl;
}

ThroughDoorTask::ThroughDoorTask(Object *object) : m_object(object), m_close_rounds(0)
{

}
