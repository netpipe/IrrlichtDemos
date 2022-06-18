#ifndef _Output_HPP_
#define _Output_HPP_

#include "Globals.hpp"

class Output
{
  public:

    static Output *Instance();

    virtual void w(const char *fmt, ...);
    void a();

  protected:

    static Output *m_output;
    static Output m_default_output_buffer;

    //Output();
    //virtual ~Output();

};

#endif
