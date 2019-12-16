#ifndef DRONE_HANDLE_H
#define DRONE_HANDLE_H


#include <string>
#include <map>

class Handle
{
public:

    const int getObjectHandle(const std::string &object_name, int client_id);

    static Handle *Instance();

    void addNewHandle(const std::string &object_name, int handle);

private:
    explicit Handle(const std::string &file);

    std::map<std::string, int> m_handles;
    std::string m_file_name;
};


#endif //DRONE_HANDLE_H
