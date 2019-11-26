#include "Output.hpp"
#include "stdarg.h"

Output *Output::m_output = NULL;
Output Output::m_default_output_buffer;

Output *Output::Instance()
{
  if(!m_output)
    m_output = &m_default_output_buffer;

  return m_output;
}

void Output::w(const char *fmt, ...)
{
#ifdef _DEBUG_MODE_
	va_list vlist;
  char buffer[255];
  va_start(vlist,fmt);
  vsprintf(buffer, fmt,vlist);
  va_end(vlist);
  printf("%s", buffer);
#endif
}

void Output::a()
{
  printf("test\n");
}
