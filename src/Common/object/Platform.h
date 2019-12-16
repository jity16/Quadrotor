#ifndef DRONE_PLATFORM_H
#define DRONE_PLATFORM_H


#include "Object.h"

class Platform : public Object
{
public:
    Platform(const std::string &name, int client_id);

    bool isPlatform() override;

    const Point getValidPosition() const override;

private:
    const simxFloat HOVER_HEIGHT_OVER_PLATFORM = Config::Instance()->getFloatParam("MoveTask",
                                                                                   "HOVER_HEIGHT_OVER_PLATFORM");
    const simxFloat HOVER_HEIGHT_OVER_END = Config::Instance()->getFloatParam("MoveTask", "HOVER_HEIGHT_OVER_END");
};


#endif //DRONE_PLATFORM_H
