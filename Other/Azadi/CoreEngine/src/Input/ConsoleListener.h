#ifndef CONSOLE_LISTENER_071106
#define CONSOLE_LISTENER_071106

#include <string>
#include <vector>


class ConsoleListener {
    public:
        virtual std::string acceptConsoleCommand(std::string) = 0;
        virtual ~ConsoleListener(){};
        virtual std::string shortDescr() { return "No description given"; }

    protected:
        std::vector<std::string> splitArgs(std::string args,char splitter=' ')
        {
            std::vector<std::string> tmp;
            std::string tmpstring;
            for(unsigned int i=0;true;i++)
            {
                if(args[i]==splitter)
                {
                    tmp.push_back(tmpstring);
                    tmpstring="";
                }
                else
                    tmpstring+=args[i];

                if(i+1 == args.length())
                {
                    tmp.push_back(tmpstring);
                    break;
                }

            }

            return tmp;
        }


};

#endif

