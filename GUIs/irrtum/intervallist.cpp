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

#include "common.h"
#include "intervallist.h"

IntervalList::IntervalList():
    m_intervals()
{
}

bool IntervalList::isEmpty() const
{
    return m_intervals.empty();
}

s32 IntervalList::getMin() const
{
    if (isEmpty())
        return 0;
    else
        return m_intervals.front().first;
}

s32 IntervalList::getMax() const
{
    if (isEmpty())
        return 0;
    else
        return m_intervals.back().second;
}

bool IntervalList::contains(s32 value) const
{
    int a = 0;
    int b = m_intervals.size() - 1;
    while (a <= b)
    {
        int pivot = (a + b) / 2;
        if (value < m_intervals[pivot].first)
            b = pivot - 1;
        else if (value > m_intervals[pivot].second)
            a = pivot + 1;
        else
            return true;
    }
    return false;
}

void IntervalList::addInterval(s32 from, s32 to)
{
    if (from > to)
        return;

    int i = 0;
    while (i < m_intervals.size())
    {
        if (m_intervals[i].first > to + 1)
        {
            break;
        }
        else if (m_intervals[i].second < from - 1)
        {
            ++i;
        }
        else
        {
            from = my_min(from, m_intervals[i].first);
            to = my_max(to, m_intervals[i].second);
            m_intervals.erase(m_intervals.begin() + i);
        }
    }
    m_intervals.insert(m_intervals.begin() + i, make_pair(from, to));
}

void IntervalList::removeBelow(s32 threshold)
{
    int size = m_intervals.size();
    int i;
    for (i = 0; i < size && m_intervals[i].second < threshold; ++i)
        ;
    m_intervals.erase(m_intervals.begin(), m_intervals.begin() + i);
    size = m_intervals.size();
    if (size != 0)
    {
        m_intervals[0].first = my_max(m_intervals[0].first, threshold);
    }
}

void IntervalList::removeAbove(s32 threshold)
{
    int size = m_intervals.size();
    int i = 0;
    for (i = 0; i < size && m_intervals[size - i - 1].first > threshold; ++i)
        ;
    m_intervals.erase(m_intervals.begin() + size - i, m_intervals.begin() + size);
    size = m_intervals.size();
    if (size != 0)
    {
        m_intervals[size - 1].second = my_min(m_intervals[size - 1].second, threshold);
    }
}

void IntervalList::debug()
{
    for (int i = 0; i < m_intervals.size(); ++i)
    {
        cout << " " << m_intervals[i].first << ".." << m_intervals[i].second;
    }
    cout << endl;
}

bool IntervalList::parseInterval(string s, s32& from, s32& to)
{
    if (s.empty())
        return false;

    from = to = 0;

    size_t dashpos = s.find_first_of('-', 1);
    if (dashpos == string::npos)
    {
        istringstream is(s);
        is >> from;
        to = from;
        return !is.fail() && !is.bad() && is.eof();
    }
    else
    {
        istringstream is(s.substr(0, dashpos));
        is >> from;
        istringstream is2(s.substr(dashpos + 1));
        is2 >> to;
        return !is.fail() && !is.bad() && is.eof() &&
            !is2.fail() && !is2.bad() && is2.eof();
    }
}
