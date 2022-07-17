#include "StaticGround.h"
#include "PlayerProcess.h"
#include "BulletProcess.h"
#include <unordered_set>

using namespace DirectX;

static std::unordered_set<AInteractComponent*> g_GroundSet = {};

void RegisterStaticGround(ObjectFactory* _factory)
{
#ifdef _DEBUG
    assert(_factory);
#endif // _DEBUG
    _factory->getAInitMapPtr().insert(
        { FUNC_NAME(GoundInit),GoundInit });
    _factory->getAUpdateMapPtr().insert(
        { FUNC_NAME(GoundUpdate),GoundUpdate });
    _factory->getADestoryMapPtr().insert(
        { FUNC_NAME(GoundDestory),GoundDestory });
}

bool GoundInit(AInteractComponent* _aitc)
{
    g_GroundSet.insert(_aitc);

    return true;
}

void GoundUpdate(AInteractComponent* _aitc, const Timer& _timer)
{
    auto acc = _aitc->getActorOwner()->
        getComponent<ACollisionComponent>();
    static CONTACT_PONT_PAIR contact = {};
    if (acc->checkCollisionWith(PLAYER_NAME, &contact))
    {
        auto playerAtc = _aitc->getActorOwner()->getSceneNode().
            getActorObject(PLAYER_NAME)->
            getComponent<ATransformComponent>();
        auto contactPoint =
            ACollisionComponent::calcCenterOfContact(contact);
        float xOffset = fabsf(contactPoint.x -
            playerAtc->getProcessingPosition().x);
        float zOffset = fabsf(contactPoint.z -
            playerAtc->getProcessingPosition().z);
        if (GetPlayerIsDashingFlg())
        {
            playerAtc->rollBackPosition();
            SetPlayerDashToObstacle();
        }
        else if (xOffset < 3.1f && zOffset < 3.1f &&
            (contactPoint.y - playerAtc->getProcessingPosition().y) < -0.1f)
        {
            playerAtc->rollBackPositionY();

            XMFLOAT3 playerPnt = playerAtc->getProcessingPosition();
            XMVECTOR contactVec = XMLoadFloat3(&contact.first) -
                XMLoadFloat3(&playerPnt);
            float lengthFirSq = XMVectorGetX(XMVector3LengthSq(contactVec));
            contactVec = XMLoadFloat3(&contact.second) -
                XMLoadFloat3(&playerPnt);
            float lengthSecSq = XMVectorGetX(XMVector3LengthSq(contactVec));
            float xzSq = xOffset * xOffset + zOffset * zOffset;
            float yFir = sqrtf(fabsf(lengthFirSq - xzSq));
            float ySec = sqrtf(fabsf(lengthSecSq - xzSq));
            float offset = fabsf(yFir - ySec);
            playerAtc->translateYAsix(offset);

            SetPlayerContactGround();
            auto atc = _aitc->getActorOwner()->
                getComponent<ATransformComponent>();
            SetPlayerLastReachGround(atc);
        }
        else if (xOffset < 3.1f && zOffset < 3.1f &&
            (contactPoint.y - playerAtc->getProcessingPosition().y) > 2.5f)
        {
            playerAtc->rollBackPositionY();

            XMFLOAT3 playerPnt = playerAtc->getProcessingPosition();
            XMVECTOR contactVec = XMLoadFloat3(&contact.first) -
                XMLoadFloat3(&playerPnt);
            float lengthFirSq = XMVectorGetX(XMVector3LengthSq(contactVec));
            contactVec = XMLoadFloat3(&contact.second) -
                XMLoadFloat3(&playerPnt);
            float lengthSecSq = XMVectorGetX(XMVector3LengthSq(contactVec));
            float xzSq = xOffset * xOffset + zOffset * zOffset;
            float yFir = sqrtf(fabsf(lengthFirSq - xzSq));
            float ySec = sqrtf(fabsf(lengthSecSq - xzSq));
            float offset = fabsf(yFir - ySec);
            playerAtc->translateYAsix(-offset);

            SetPlayerBrokeHead();
        }
        else
        {
            static const DirectX::XMFLOAT3 xUnit = { 1.f,0.f,0.f };
            static const DirectX::XMFLOAT3 zUnit = { 0.f,0.f,1.f };
            static const DirectX::XMVECTOR xVec = DirectX::XMLoadFloat3(&xUnit);
            static const DirectX::XMVECTOR zVec = DirectX::XMLoadFloat3(&zUnit);
            DirectX::XMFLOAT3 playerMove = GetPlayerMoveDirection();
            DirectX::XMFLOAT3 playerPnt = playerAtc->getProcessingPosition();
            DirectX::XMVECTOR plyMoveVec = DirectX::XMLoadFloat3(&playerMove);
            DirectX::XMVECTOR contactVec = DirectX::XMLoadFloat3(&contactPoint) -
                DirectX::XMLoadFloat3(&playerPnt);
            contactVec = DirectX::XMVector3Normalize(contactVec);
            contactVec = DirectX::XMVectorGetX(DirectX::XMVector3Dot(
                contactVec, plyMoveVec)) * contactVec;
            DirectX::XMVECTOR moveVec = plyMoveVec - contactVec;
            float xAsix = DirectX::XMVectorGetX(DirectX::XMVector3Dot(
                moveVec, xVec));
            float zAsix = DirectX::XMVectorGetX(DirectX::XMVector3Dot(
                moveVec, zVec));
            float aimSlow = (GetPlayerAimingFlg() && GetPlayerCanAimFlg()) ?
                0.1f : 1.f;
            float deltatime = _timer.floatDeltaTime() * aimSlow;
            playerAtc->rollBackPositionX();
            playerAtc->rollBackPositionZ();
            playerAtc->translateZAsix(0.02f * deltatime * zAsix);
            playerAtc->translateXAsix(0.02f * deltatime * xAsix);
            DirectX::XMStoreFloat3(&playerMove, moveVec);
            SetPlayerMoveDirection(playerMove);
        }
    }
}

void GoundDestory(AInteractComponent* _aitc)
{
    g_GroundSet.erase(_aitc);
}