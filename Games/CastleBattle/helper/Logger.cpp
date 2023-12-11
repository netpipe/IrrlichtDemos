#include "Logger.h"
#include <iostream>
using namespace irr;
using namespace std;
Logger * Logger::logger  = NULL;
Logger* Logger::getLogger(){
  if(logger == NULL)
    logger = new Logger();
  return logger;
}
void Logger::print(LOG_TYPE type,  int level, core::stringc str){
  cout<<str.c_str()<<endl;
}
Logger::Logger(){

}
