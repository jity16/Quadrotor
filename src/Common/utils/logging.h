#ifndef LANDING_LOGGING_H
#define LANDING_LOGGING_H
#include <stack>
namespace log
{
    typedef std::pair<std::string, int> task;

    extern std::string indent;
    extern std::stack<std::pair<std::string, int> > tasks;

    int getCurrentTime();

    void start(const std::string &msg);

    void end();

    void info(const std::string &msg);
}

#endif //LANDING_LOGGING_H
