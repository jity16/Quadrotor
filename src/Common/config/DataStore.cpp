#include <fstream>
#include <iostream>
#include <cassert>
#include "DataStore.h"

const float DataStore::getData(const std::string &key)
{
    assert(m_datas.find(key) != m_datas.end());
    return m_datas[key];
}

DataStore *DataStore::Instance()
{
    static DataStore dataStore("data.ini");
    return &dataStore;
}

void DataStore::addNewData(const std::string &key, float value)
{
    //first cache
    m_datas.insert(std::make_pair(key, value));
    //then file
    std::ofstream file_handle(m_file_name, std::ios::app);
    file_handle << key << ' ' << value << std::endl;
    file_handle.close();
}

DataStore::DataStore(const std::string &file)
{
    m_file_name = file;
    std::ifstream file_handle(m_file_name);
    std::string key;
    float value;
    while(file_handle>>key>>value)
    {
        m_datas.insert(std::make_pair(key, value));
    }
    std::cout << "------------------Datas load completed------------------" << std::endl;
    for (auto i: m_datas)
    {
        std::cout << i.first << ' ' << i.second << std::endl;
    }
    file_handle.close();
}

bool DataStore::isStored(const std::string &key)
{
    return m_datas.find(key) != m_datas.end();
}
