#include "Config.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <regex>
#include <cassert>
#include "../utils/utils.h"

setting_type Config::readConfig(const std::string &file)
{
    std::ifstream file_handle(file);
    std::string line;
    setting_type settings;
    std::string class_name;
    std::map<std::string, std::string> params;
    while (std::getline(file_handle, line))
    {
        std::smatch match_result;
        if (std::regex_match(line, match_result, std::regex("\\s*?\\[(.*?)\\]\\s*?")))
        {
            if (!class_name.empty())
            {
                settings.insert(std::make_pair(class_name, params));
            }
            class_name = match_result[1];
            params.clear();
        }
        if (std::regex_match(line, match_result, std::regex("\\s*?(\\S*?)\\s*?=\\s*?(\\S*?)\\s*?")))
        {
            params.insert(std::make_pair(match_result[1], match_result[2]));
        }
    }
    if (!class_name.empty())
    {
        settings.insert(std::make_pair(class_name, params));
    }
    std::cout << "------------------Configuration load completed------------------" << std::endl;
    for (auto i: settings)
    {
        std::cout << i.first << std::endl;
        for (auto j: i.second)
        {
            std::cout << '\t' << j.first << " : " << j.second << std::endl;
        }
    }
    return settings;
}

Config::Config(const std::string &file) : settings(readConfig(file))
{

}

const std::string Config::getStringParam(const std::string &class_name, const std::string &param_name)
{
    //std::cout << "Query " << class_name << " -> " << param_name;
    assert(settings.find(class_name) != settings.end());
    assert(settings[class_name].find(param_name) != settings[class_name].end());
    //std::cout << " and get " << settings[class_name][param_name] << std::endl;
    return settings[class_name][param_name];
}

const float Config::getFloatParam(const std::string &class_name, const std::string &param_name)
{
    return float(atof(getStringParam(class_name, param_name).c_str()));
}

const int Config::getIntParam(const std::string &class_name, const std::string &param_name)
{
    return atoi(getStringParam(class_name, param_name).c_str());
}

Config* Config::Instance()
{
    static Config config("config.ini");
    return &config;
}

const std::vector<std::string>
Config::getStringVectorParam(const std::string &class_name, const std::string &param_name)
{
    return utils::regexSplit(getStringParam(class_name, param_name), ",");
}

template<class Type>
Type convertString(const std::string &str)
{
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

