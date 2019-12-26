/*
  ardusim.h - Arduino Simulation Routines Header File

  Copyright (c) 2012 Stanley Huang

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

*/

#ifndef _ARDUSIM_H
#define _ARDUSIM_H

#ifdef WIN32
#include <windows.h>
#define usleep(us) Sleep(us / 1000)
#define utoa itoa
#define ultoa itoa
#define srandom

#else
#include   <unistd.h>

#define itoa(value, string, radix) sprintf(string, radix == 16 ? "%X" : "%d", value)
#define ltoa(value, string, radix) sprintf(string, radix == 16 ? "%X" : "%ld", value)
#define utoa(value, string, radix) sprintf(string, radix == 16 ? "%X" : "%u", value)
#define ultoa(value, string, radix) sprintf(string, radix == 16 ? "%X" : "%ld", value)

#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern uint8_t SIM_MEM[];
extern uint8_t SIM_SFR[];

// simulated AVR C stuff
//#define _BV(bit) (1 << (bit))

//extern uint8_t SREG;


void cli();
void sei();

#endif