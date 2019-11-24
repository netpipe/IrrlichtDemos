#ifndef AZSCMODELFACTORY_H
#define AZSCMODELFACTORY_H

#include <vector>

class azModelFactory
{
    public:
        virtual ~azModelFactory() {};

        virtual int LoadModel(const char * filename) = 0;
        virtual void Move(const unsigned int &iD, const float &x, const float &y, const float &z) = 0;
        virtual void Rotate(const unsigned int &iD, const float &x, const float &y, const float &z) = 0;
        virtual void Flush() = 0;
        virtual std::vector<std::vector<float> > GetBoundingBoxes(const unsigned int &ID) = 0;
        virtual void StartAction(const unsigned int &ID, const std::string &animName, const float &easeIn = 0.0f, const float &easeOut = 0.0f) = 0;
        virtual void StopAction(const unsigned int &ID, const std::string &animName) = 0;
        virtual void StartCycle(const unsigned int &ID, const std::string &animName, const float &weight = 1.0f, const float &delay = 0.0f) = 0;
        virtual void StopCycle(const unsigned int &ID, const std::string &animName, const float &delay = 0.0f) = 0;
        virtual std::vector<std::string> GetAnimationNames(const unsigned int &ID) = 0;
};

#endif
