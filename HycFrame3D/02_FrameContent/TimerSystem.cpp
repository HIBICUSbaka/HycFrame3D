#include "TimerSystem.h"
#include "SystemExecutive.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "ComponentContainer.h"
#include "ATimerComponent.h"
#include "UTimerComponent.h"

TimerSystem::TimerSystem(SystemExecutive* _sysExecutive) :
    System("timer-system", _sysExecutive),
    mATimerVecPtr(nullptr), mUTimerVecPtr(nullptr)
{

}

TimerSystem::~TimerSystem()
{

}

bool TimerSystem::Init()
{
#ifdef _DEBUG
    assert(GetSystemExecutive());
#endif // _DEBUG

    mATimerVecPtr = (std::vector<ATimerComponent>*)GetSystemExecutive()->
        GetSceneManager()->GetCurrentSceneNode()->
        GetComponentContainer()->GetCompVecPtr(COMP_TYPE::A_TIMER);
    mUTimerVecPtr = (std::vector<UTimerComponent>*)GetSystemExecutive()->
        GetSceneManager()->GetCurrentSceneNode()->
        GetComponentContainer()->GetCompVecPtr(COMP_TYPE::U_TIMER);

    if (!(mATimerVecPtr && mUTimerVecPtr)) { return false; }

    return true;
}

void TimerSystem::Run(Timer& _timer)
{
    for (auto& atmc : *mATimerVecPtr)
    {
        if (atmc.GetCompStatus() == STATUS::ACTIVE) { atmc.Update(_timer); }
    }

    for (auto& utmc : *mUTimerVecPtr)
    {
        if (utmc.GetCompStatus() == STATUS::ACTIVE) { utmc.Update(_timer); }
    }
}

void TimerSystem::Destory()
{

}