#include "Platform.h"
#include "../../Planning/task/GrabTask.h"

bool Platform::isPlatform()
{
    return true;
}

Platform::Platform(const std::string &name, int client_id) : Object(name, client_id)
{
    if (getName() == "Target_platform")
        m_task = new GrabTask(this);
}

const Point Platform::getValidPosition() const
{
    Point ret = m_pos;
    if(getName() == "End")
    {
        ret.setZ(HOVER_HEIGHT_OVER_END);
    } else
    {
        ret.setZ(HOVER_HEIGHT_OVER_PLATFORM);
    }
    return ret;
}

