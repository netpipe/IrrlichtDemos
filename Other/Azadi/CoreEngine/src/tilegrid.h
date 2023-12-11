#ifndef TILEGRID_H
#define TILEGRID_H

#include <deque>
#include <vector>

template <class T>
class tilegrid
{
    public:

        tilegrid(const unsigned int &width, const unsigned int &height, const T &initializer)
        {
            for (unsigned int j = 0; j < height; ++j)
            {
                std::deque<T> temp;
                data.push_back(temp);
                for (unsigned int i = 0; i < width; ++i)
                {
                    data[j].push_back(initializer);
                }
            }
        };
        ~tilegrid(){}

        tilegrid<T>& push_left(const std::vector<T> &lhs)
        {
            for (unsigned int i = 0; i < data.size(); ++i)
                data[i].push_front(lhs[i]);

            return *this;
        }

        tilegrid<T>& push_right(const std::vector<T> &rhs)
        {
            for (unsigned int i = 0; i < data.size(); ++i)
                data[i].push_back(rhs[i]);

            return *this;
        }

        tilegrid<T>& push_top(const std::vector<T> &top)
        {
            std::list<T> temp;
            for (unsigned int i=0; i < top.size(); ++i)
                temp.push_back(top[i]);

            data.push_front(temp);

            return *this;
        }

        tilegrid<T>& push_bottom(const std::vector<T> &bottom)
        {
            std::list<T> temp;
            for (unsigned int i=0; i < bottom.size(); ++i)
                temp.push_back(bottom[i]);

            data.push_back(temp);

            return *this;
        }

        tilegrid<T>& pop_left()
        {
            for (unsigned int i = 0; i < data.size(); ++i)
                data[i].pop_front();
            return *this;
        }

        tilegrid<T>& pop_right()
        {
            for (unsigned int i = 0; i < data.size(); ++i)
                data[i].pop_back();
            return *this;
        }

        tilegrid<T>& pop_top()
        {
            data.pop_front();
            return *this;
        }

        tilegrid<T>& pop_bottom()
        {
            data.pop_back();
            return *this;
        }

        std::deque<T> operator[](const unsigned int &x)
        {
            return data.at(x);
        }

        std::vector<unsigned int> size()
        {
            std::vector<unsigned int> temp;
            temp.push_back(data.size());
            if (data.size() > 0)
                temp.push_back(data[0].size);

            return temp;
        }

        std::deque<std::deque<T> > data;

};


#endif
