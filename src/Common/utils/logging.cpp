#include <iostream>
#include <cassert>
#include "logging.h"

std::string log::indent;
std::stack<std::pair<std::string, int> > log::tasks;
int log::getCurrentTime()
{
    return 0;
}

void log::start(const std::string &msg)
{
    std::cout << indent + msg + " start"<< std::endl;
    indent += '\t';
    tasks.push(task(std::string(msg), getCurrentTime()));
}

void log::end()
{
    assert(!tasks.empty());
    indent.pop_back();
    std::cout << indent + tasks.top().first + " finished in " << (getCurrentTime() - tasks.top().second) << "ms"
              << std::endl;
    tasks.pop();
}

void log::info(const std::string &msg)
{
    std::cout << indent + msg << std::endl;
}
