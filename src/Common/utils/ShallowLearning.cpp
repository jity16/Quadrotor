#include <cassert>
#include <fstream>
#include <iostream>
#include "ShallowLearning.h"
#include "utils.h"

float ShallowLearning::evalParam(const std::string &param, float default_value)
{
    std::string file_path = generateFileFromParamName(param);
    std::fstream file(file_path, std::ios::in);
    if (!file)
    {
        //std::cout << file_path + " not exist, so return default value" << std::endl;
        return default_value;
    } else
    {
        std::vector<float> history_data;
        float data;
        std::string now_time;
        while (file >> data >> now_time)
        {
            history_data.push_back(data);
        }
        file.close();
        return evalParamByHistory(history_data);
    }
}

float ShallowLearning::evalDistance(const std::string &source, const std::string &destination, float default_value)
{
    return evalParam(generateEventFromDistance(source, destination), default_value);
}

void ShallowLearning::updateParam(const std::string &param, float actual_value)
{
    std::string file_path = generateFileFromParamName(param);
    std::fstream file(file_path, std::ios::out | std::ios::app);
    file << actual_value << '\t' << utils::getNowTime() << std::endl;
    file.close();
}

void ShallowLearning::updateDistance(const std::string &source, const std::string &destination, float actual_value)
{
    updateParam(generateEventFromDistance(source, destination), actual_value);
}

float ShallowLearning::evalParamByHistory(const std::vector<float> &history_data)
{
    float rate = 1, result = 0;
    assert(!history_data.empty());
    for (int i = int(history_data.size()) - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            result += rate * history_data[i];
        } else
        {
            result += SHRINK_RATE * rate * history_data[i];
            rate *= 1 - SHRINK_RATE;
        }
    }
    return result;
}

const std::string ShallowLearning::generateEventFromDistance(const std::string &source, const std::string &destination)
{
    return "from " + source + " to " + destination;
}

const std::string ShallowLearning::generateFileFromParamName(const std::string &param_name)
{
    return "data/shallowLearning/" + param_name + ".txt";
}
