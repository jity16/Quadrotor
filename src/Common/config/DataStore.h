#ifndef DRONE_PARAMETER_H
#define DRONE_PARAMETER_H


#include <string>
#include <map>

class DataStore
{
public:
    const float getData(const std::string &key);

    static DataStore *Instance();

    void addNewData(const std::string &key, float value);

    bool isStored(const std::string &key);

private:
    explicit DataStore(const std::string &file);

    std::map<std::string, float> m_datas;
    std::string m_file_name;
};


#endif //DRONE_PARAMETER_H
