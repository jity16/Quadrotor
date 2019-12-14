#include <iostream>
#include <utility>
#include "GrabTask.h"
#include "../updater/PlanningPathUpdater.h"
#include "../../Common/utils/utils.h"
#include "../../Common/config/Handle.h"

void GrabTask::Execute(PlanningPathUpdater *t)
{
//    Point target_pos = t->getTargetPosition();
//    if ((cylinder_pos.x() - target_pos.x()) * (cylinder_pos.x() - target_pos.x()) +
//        (cylinder_pos.y() - target_pos.y()) * (cylinder_pos.y() - target_pos.y()) < 0.5 * 0.5)
//    {
//        std::cout << "adjust target position" << std::endl;
//        target_pos.setX(cylinder_pos.x());
//        target_pos.setY(cylinder_pos.y());
//        t->setTargetPosition(target_pos);
//    }

    if ((t->getDronePosition() - m_path_points[m_now_target_index]).norm() < CLOSE_THRESHOLD)
    {
        m_close_rounds++;
    } else
    {
        m_close_rounds = 0;
    }
    if (m_close_rounds >= CLOSE_ROUNDS_LIMIT)
    {
        m_now_target_index++;
        m_close_rounds = 0;
        //二次标定
        if(m_now_target_index == 1)
        {
            use_vision = true;
            utils::sleep(MICRO_SECONDS_FOR_DISCRIMINATE);
            use_vision = false;

            Point circle_pos = utils::getObjectPosition("CircleDummy", t->getCid());
            Point target_pos = utils::getObjectPosition("TargetDummy", t->getCid());
            std::cout << "second result : circle_pos = " << circle_pos << std::endl;
            std::cout << "second result : target_pos = " << target_pos << std::endl;
            std::cout << "second record result : circle_pos = " << recog_circle_pos << std::endl;
            std::cout << "second record result : target_pos = " << recog_target_pos << std::endl;
            std::cout << "second signal record result : target_pos = " << single_target_pos << std::endl;
            Point selected_pos = target_pos;
            selected_pos.setX(7.2f);
            selected_pos.setY(-10.4f);
            m_path_points.emplace_back(selected_pos.x(), selected_pos.y(), GRAB_START_HEIGHT);
            m_path_points.emplace_back(selected_pos.x(), selected_pos.y(), GRAB_START_HEIGHT - 0.3f);
            m_path_points.emplace_back(selected_pos.x(), selected_pos.y(), GRAB_START_HEIGHT - 0.325f);
            m_path_points.emplace_back(selected_pos.x(), selected_pos.y(), GRAB_START_HEIGHT);
            for(int i = 0; i < m_path_points.size(); i ++)
            {
                std::cout << i << ' ' << m_path_points[i] << std::endl;
            }
        }
        if (m_now_target_index == m_path_points.size() - 1)
        {
            std::cout << "TATQAQ" << std::endl;
            t->setHand();
            utils::sleep(3000);
        }
        if (m_now_target_index < m_path_points.size())
            t->setTargetPosition(m_path_points[m_now_target_index]);
    }
    if (m_now_target_index >= m_path_points.size())
    {
        t->finishCurrentTask();
    }
    if (m_close_rounds != 0)
    {
        std::cout << "close rounds is " << m_close_rounds << ' ' << m_now_target_index << std::endl;
    }
}

const std::string GrabTask::getName()
{
    return "Grab_On_" + m_object->getName();
}

GrabTask::GrabTask(Object *object) : m_object(object)
{

}

void GrabTask::Enter(PlanningPathUpdater *t)
{
    Task::Enter(t);
    std::cout << "Prepare to grab cylinder on platform " << m_object->getName() << std::endl;
    std::cout << "Open the eyes _(:з)∠)_" << std::endl;
    use_vision = true;
    utils::sleep(MICRO_SECONDS_FOR_DISCRIMINATE);
    use_vision = false;
    Point circle_pos = utils::getObjectPosition("CircleDummy", t->getCid());
    Point target_pos = utils::getObjectPosition("TargetDummy", t->getCid());
    std::cout << "first result : circle_pos = " << circle_pos << std::endl;
    std::cout << "first result : target_pos = " << target_pos << std::endl;
    std::cout << "first record result : circle_pos = " << recog_circle_pos << std::endl;
    std::cout << "first record result : target_pos = " << recog_target_pos << std::endl;
    std::cout << "first signal record result : target_pos = " << single_target_pos << std::endl;
    Point selected_pos = target_pos;
    Point now_position = t->getDronePosition();
    if(fabsf(now_position.x() - 7.2f) < fabsf(selected_pos.x() - 7.2f))
    {
        selected_pos.setX(now_position.x());
    }
    if(fabsf(now_position.y() + 10.4f) < fabsf(selected_pos.y() + 10.4f))
    {
        selected_pos.setY(now_position.y());
    }
    m_path_points.emplace_back(selected_pos.x(), selected_pos.y(), 2.5f);
    for(const auto &point: m_path_points)
    {
        std::cout << point << std::endl;
    }
    m_now_target_index = 0;
    if (m_now_target_index < m_path_points.size())
        t->setTargetPosition(m_path_points[m_now_target_index]);
}

void GrabTask::Exit(PlanningPathUpdater *t)
{
    std::cout << "Success to grab cylinder on platform " << m_object->getName() << "and go to next task" << std::endl;
    Task::Exit(t);
}
