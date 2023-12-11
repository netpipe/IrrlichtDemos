/*
Irrtum
Copyright (C) 2011-2015 kahrl <kahrl@gmx.net>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef INTERVALLIST_HEADER
#define INTERVALLIST_HEADER

#include "common.h"

class IntervalList
{
public:
    IntervalList();

    bool isEmpty() const;
    s32 getMin() const;
    s32 getMax() const;
    bool contains(s32 value) const;

    void addInterval(s32 from, s32 to);
    void removeBelow(s32 threshold);
    void removeAbove(s32 threshold);
    void debug();

    bool parseInterval(string s, s32& from, s32& to);

private:
    vector<pair<s32, s32> > m_intervals;
};

#endif

