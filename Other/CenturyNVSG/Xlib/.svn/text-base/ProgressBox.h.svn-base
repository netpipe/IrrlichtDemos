
//
// A simple XLib only Progress window replacement 
//

#ifndef _PROGRESSBOX_H_
#define _PROGRESSBOX_H_

#include <X11/Xlib.h>

class ProgressBox
{
public:
  ProgressBox();
  ~ProgressBox();

  void Create();
  void SetRange( int, int );
  void SetStep( int );
  void StepIt();
  void ShowWindow( bool tf );

private:
  void processMessages();

  Display * m_display;
  Window    m_window;
  GC        m_gc;
  int       m_rangeLow;
  int       m_rangeHigh;
  int       m_step;
  int       m_curStep;
  bool      m_displayed;
};

#endif /* _PROGRESSBOX_H_ */
