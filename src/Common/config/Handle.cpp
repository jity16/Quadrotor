#include <fstream>
#include <iostream>
#include "Handle.h"
extern "C" {
#include "../vrep/extApi.h"
}
const int Handle::getObjectHandle(const std::string &object_name, int client_id)
{
    if(m_handles.find(object_name) == m_handles.end())
    {
        std::cout << "appear new object_name " << object_name << std::endl;
        int ret;
        simxGetObjectHandle(client_id, object_name.c_str(), &ret, simx_opmode_blocking);
        std::cout << "get it's handles is " << ret << std::endl;
        addNewHandle(object_name, ret);
        return ret;
    }else
    {
        std::cout << "old object " << object_name << " and the handles is " << m_handles[object_name] << std::endl;
        return m_handles[object_name];
    }
    return 0;
}

Handle *Handle::Instance()
{
    static Handle handle("handle.ini");
    return &handle;
}

Handle::Handle(const std::string &file)
{
    m_file_name = file;
    std::ifstream file_handle(m_file_name);
    std::string name;
    int handle;
    while(file_handle>>name>>handle)
    {
        m_handles.insert(std::make_pair(name, handle));
    }
    std::cout << "------------------Handles load completed------------------" << std::endl;
    for (auto i: m_handles)
    {
        std::cout << i.first << ' ' << i.second << std::endl;
    }
    file_handle.close();
}

void Handle::addNewHandle(const std::string &object_name, int handle)
{
    //first cache
    m_handles.insert(std::make_pair(object_name, handle));
    //then file
    std::ofstream file_handle(m_file_name, std::ios::app);
    file_handle << object_name << ' ' << handle << std::endl;
    file_handle.close();
}
