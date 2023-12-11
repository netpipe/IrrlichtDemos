Cloud scene node V0.2 by gaz davidson - gaz@bitplane.net


Clouds! Well what more can I say? run the demo to see them in action.

Fixed for Irrlicht 1.3 :)

Changes from 0.1:

Spent lots of time profiling, but it's still heavy on cpu
Shell sort now sorts lists of pointers instead of structs
culls distant parent nodes

todo:
  cull parent clouds that are outside camera->fustrum->bbox
  cull more clouds before they reach the the sort list, since it's the cpu killer
  more cloud generation functions
  maybe sort clouds in to regions

  rewrite! this was my 2nd ever irrlicht scene node, and it shows!