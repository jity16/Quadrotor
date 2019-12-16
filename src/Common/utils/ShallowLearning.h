#ifndef LANDING_SHALLOWLEARNING_H
#define LANDING_SHALLOWLEARNING_H


#include <mutex>
#include <vector>
#include "../config/Config.h"

namespace ShallowLearning
{
    //std::mutex mutex;
    const float SHRINK_RATE = Config::Instance()->getFloatParam("ShallowLearning", "SHRINK_RATE");
    float evalParam(const std::string &param, float default_value);
    float evalDistance(const std::string &source, const std::string &destination, float default_value);
    void updateParam(const std::string &param, float actual_value);
    void updateDistance(const std::string &source, const std::string &destination, float actual_value);
    float evalParamByHistory(const std::vector<float> &history_data);
    const std::string generateEventFromDistance(const std::string &source, const std::string &destination);
    const std::string generateFileFromParamName(const std::string &param_name);
};


#endif //LANDING_SHALLOWLEARNING_H
