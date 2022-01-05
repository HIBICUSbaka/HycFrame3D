#include "DestProcess.h"
#include "PlayerProcess.h"
#include "FadeProcess.h"

void RegisterDestProcess(ObjectFactory* _factory)
{
#ifdef _DEBUG
    assert(_factory);
#endif // _DEBUG
    _factory->GetAInitMapPtr()->insert(
        { FUNC_NAME(DestInit),DestInit });
    _factory->GetAUpdateMapPtr()->insert(
        { FUNC_NAME(DestUpdate),DestUpdate });
    _factory->GetADestoryMapPtr()->insert(
        { FUNC_NAME(DestDestory),DestDestory });
    _factory->GetAInitMapPtr()->insert(
        { FUNC_NAME(DestPtcInit),DestPtcInit });
    _factory->GetAUpdateMapPtr()->insert(
        { FUNC_NAME(DestPtcUpdate),DestPtcUpdate });
    _factory->GetADestoryMapPtr()->insert(
        { FUNC_NAME(DestPtcDestory),DestPtcDestory });
}

static ACollisionComponent* g_DestAcc = nullptr;
static ATransformComponent* g_DestAtc = nullptr;
static ACollisionComponent* g_DestPtcAcc = nullptr;
static AParticleComponent* g_DestPtcApc = nullptr;

static bool g_ToResultTigger = false;

bool DestInit(AInteractComponent* _aitc)
{
    g_DestAcc = _aitc->GetActorOwner()->
        GetAComponent<ACollisionComponent>(COMP_TYPE::A_COLLISION);
    if (!g_DestAcc) { return false; }

    g_DestAtc = _aitc->GetActorOwner()->
        GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM);
    if (!g_DestAtc) { return false; }

    g_ToResultTigger = false;

    return true;
}


void DestUpdate(AInteractComponent* _aitc, Timer& _timer)
{
    g_DestAtc->RotateYAsix(_timer.FloatDeltaTime() / 1500.f);

    if (g_DestAcc->CheckCollisionWith(PLAYER_NAME))
    {
        if (!g_ToResultTigger) { SetSceneOutFlg(true); }
        g_ToResultTigger = true;
    }

    if (g_ToResultTigger && !GetSceneOutFlg())
    {
        P_LOG(LOG_DEBUG, "to result\n");
        _aitc->GetActorOwner()->GetSceneNode().GetSceneManager()->
            LoadSceneNode("result-scene", "result-scene.json");
    }
}

void DestDestory(AInteractComponent* _aitc)
{
    g_DestAcc = nullptr;
    g_DestAtc = nullptr;
    g_ToResultTigger = false;
}

bool DestPtcInit(AInteractComponent* _aitc)
{
    g_DestPtcApc = _aitc->GetActorOwner()->
        GetAComponent<AParticleComponent>(COMP_TYPE::A_PARTICLE);
    if (!g_DestPtcApc) { return false; }

    g_DestPtcAcc = _aitc->GetActorOwner()->
        GetAComponent<ACollisionComponent>(COMP_TYPE::A_COLLISION);
    if (!g_DestPtcAcc) { return false; }

    return true;
}

void DestPtcUpdate(AInteractComponent* _aitc, Timer& _timer)
{
    if (g_DestPtcAcc->CheckCollisionWith(PLAYER_NAME))
    {
        g_DestPtcApc->GetEmitterInfo().mEmitNumPerSecond = 2400.f;
    }
    else
    {
        g_DestPtcApc->GetEmitterInfo().mEmitNumPerSecond = 800.f;
    }
}

void DestPtcDestory(AInteractComponent* _aitc)
{
    g_DestAcc = nullptr;
    g_DestPtcAcc = nullptr;
    g_DestPtcApc = nullptr;
}
