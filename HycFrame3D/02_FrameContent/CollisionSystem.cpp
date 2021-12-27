#include "CollisionSystem.h"
#include "SystemExecutive.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "ComponentContainer.h"
#include "PhysicsWorld.h"
#include "ACollisionComponent.h"

CollisionSystem::CollisionSystem(class SystemExecutive* _sysExecutive) :
    System("collision-system", _sysExecutive),
    mACollisionVecPtr(nullptr)
{

}

CollisionSystem::~CollisionSystem()
{

}

bool CollisionSystem::Init()
{
#ifdef _DEBUG
    assert(GetSystemExecutive());
#endif // _DEBUG

    mACollisionVecPtr = (std::vector<ACollisionComponent>*)GetSystemExecutive()->
        GetSceneManager()->GetCurrentSceneNode()->
        GetComponentContainer()->GetCompVecPtr(COMP_TYPE::A_COLLISION);

    if (!(mACollisionVecPtr)) { return false; }

    return true;
}

void CollisionSystem::Run(Timer& _timer)
{
    for (auto& acc : *mACollisionVecPtr)
    {
        acc.Update(_timer);
    }

    GetSystemExecutive()->GetSceneManager()->GetCurrentSceneNode()->
        GetPhysicsWorld()->DetectCollision();
}

void CollisionSystem::Destory()
{

}