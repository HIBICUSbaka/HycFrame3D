#pragma once

#include "System.h"
#include <vector>

class InstanceSystem :public System
{
public:
    InstanceSystem(class SystemExecutive* _sysExecutive);
    virtual ~InstanceSystem();

public:
    virtual bool Init();
    virtual void Run(Timer& _timer);
    virtual void Destory();

private:
    std::vector<class ATransformComponent>* mATransVecPtr;
    std::vector<class UTransformComponent>* mUTransVecPtr;
};
