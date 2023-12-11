#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <fstream>
#include <iostream>

class Logger
{
    public:
        void initialize();
        void shutdown();

        Logger& operator<<(const std::string&);

	bool endl;

    private:
        std::ofstream logfile;
};

extern Logger log;

#endif
