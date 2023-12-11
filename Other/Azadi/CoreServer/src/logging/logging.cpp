#include "logging.h"

void Logger::initialize()
{
    logfile.open("./log.txt");

    logfile << "Started logging" << std::endl;
}

void Logger::shutdown()
{
    logfile.close();
}

Logger& Logger::operator<<(const std::string &message)
{
    logfile << message << std::endl;
    std::cout << message << std::endl;

    return *this;
}

