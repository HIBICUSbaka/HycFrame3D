#include "SPInput.h"
#include "RSRoot_DX11.h"
#include "RSPipelinesManager.h"
#include "RSMeshHelper.h"

void RegisterSPInput(ObjectFactory* _factory)
{
#ifdef _DEBUG
    assert(_factory);
#endif // _DEBUG
    _factory->GetAInputMapPtr()->insert(
        { FUNC_NAME(TestASpInput),TestASpInput });
    _factory->GetAInputMapPtr()->insert(
        { FUNC_NAME(TempToTitle),TempToTitle });
    _factory->GetAInputMapPtr()->insert(
        { FUNC_NAME(TempToSelect),TempToSelect });
    _factory->GetAInputMapPtr()->insert(
        { FUNC_NAME(TempToRun),TempToRun });
    _factory->GetAInputMapPtr()->insert(
        { FUNC_NAME(TempToResult),TempToResult });
    _factory->GetAInitMapPtr()->insert(
        { FUNC_NAME(TestASpInit),TestASpInit });
    _factory->GetAUpdateMapPtr()->insert(
        { FUNC_NAME(TestASpUpdate),TestASpUpdate });
    _factory->GetADestoryMapPtr()->insert(
        { FUNC_NAME(TestASpDestory),TestASpDestory });
    _factory->GetUInputMapPtr()->insert(
        { FUNC_NAME(TestUSpInput),TestUSpInput });
    _factory->GetUInputMapPtr()->insert(
        { FUNC_NAME(TestUSpBtnInput),TestUSpBtnInput });
    _factory->GetUInitMapPtr()->insert(
        { FUNC_NAME(TestUSpInit),TestUSpInit });
    _factory->GetUUpdateMapPtr()->insert(
        { FUNC_NAME(TestUSpUpdate),TestUSpUpdate });
    _factory->GetUDestoryMapPtr()->insert(
        { FUNC_NAME(TestUSpDestory),TestUSpDestory });

    _factory->GetAInitMapPtr()->insert({ FUNC_NAME(AniInit),AniInit });
    _factory->GetAUpdateMapPtr()->insert({ FUNC_NAME(AniUpdate),AniUpdate });
    _factory->GetADestoryMapPtr()->insert({ FUNC_NAME(AniDestory),AniDestory });
    _factory->GetAInitMapPtr()->insert({ FUNC_NAME(BBInit),BBInit });
    _factory->GetAUpdateMapPtr()->insert({ FUNC_NAME(BBUpdate),BBUpdate });
    _factory->GetADestoryMapPtr()->insert({ FUNC_NAME(BBDestory),BBDestory });
}

void TestASpInput(AInputComponent* _aic, Timer& _timer)
{
    if (input::isKeyDownInSingle(M_LEFTBTN))
    {
        auto mouseOffset = input::getMouseOffset();
        float horiR = -mouseOffset.x * _timer.FloatDeltaTime() / 800.f;
        _aic->GetSceneNode().GetMainCamera()->rotateRSCamera(0.f, horiR);
    }

    if (input::isKeyPushedInSingle(KB_RETURN))
    {
        P_LOG(LOG_DEBUG, "to test2\n");
        _aic->GetSceneNode().GetSceneManager()->
            LoadSceneNode("sample2-scene", "sample2-scene.json");
    }

    if (input::isKeyDownInSingle(KB_W))
    {
        _aic->GetActorObject("sp-point-light-actor")->
            GetComponent<ATransformComponent>()->
            TranslateZAsix(0.1f * _timer.FloatDeltaTime());
    }
    if (input::isKeyDownInSingle(KB_A))
    {
        _aic->GetActorObject("sp-point-light-actor")->
            GetComponent<ATransformComponent>()->
            TranslateXAsix(-0.1f * _timer.FloatDeltaTime());
    }
    if (input::isKeyDownInSingle(KB_S))
    {
        _aic->GetActorObject("sp-point-light-actor")->
            GetComponent<ATransformComponent>()->
            TranslateZAsix(-0.1f * _timer.FloatDeltaTime());
    }
    if (input::isKeyDownInSingle(KB_D))
    {
        _aic->GetActorObject("sp-point-light-actor")->
            GetComponent<ATransformComponent>()->
            TranslateXAsix(0.1f * _timer.FloatDeltaTime());
    }
    if (input::isKeyPushedInSingle(KB_P))
    {
        static bool simp = true;
        std::string basic = "light-pipeline";
        std::string simple = "simple-pipeline";
        if (simp)
        {
            getRSDX11RootInstance()->getPipelinesManager()->setPipeline(basic);
        }
        else
        {
            getRSDX11RootInstance()->getPipelinesManager()->setPipeline(simple);
        }
        simp = !simp;
    }
}

bool TestASpInit(AInteractComponent* _aitc)
{
    P_LOG(LOG_DEBUG, "a sp init\n");

    _aitc->GetActorOwner()->
        GetComponent<ATimerComponent>()->
        StartTimer("timer1");

    return true;
}
void TestASpUpdate(AInteractComponent* _aitc, Timer&)
{
    //P_LOG(LOG_DEBUG, "a sp update\n");
    /*float time0 = _aitc->GetActorOwner()->
        GetComponent<ATimerComponent>(COMP_TYPE::A_TIMER)->
        GetTimer("timer0")->mTime;
    float time1 = _aitc->GetActorOwner()->
        GetComponent<ATimerComponent>(COMP_TYPE::A_TIMER)->
        GetTimer("timer1")->mTime;
    P_LOG(LOG_DEBUG, "timer0 : %f , timer1 : %f\n", time0, time1);*/

    CONTACT_PONT_PAIR contact = {};
    if (_aitc->GetActorObject("sp-point-light-actor")->
        GetComponent<ACollisionComponent>()->
        CheckCollisionWith("sp-actor", &contact))
    {
        _aitc->GetActorObject("sp-point-light-actor")->
            GetComponent<ATransformComponent>()->
            RollBackPosition();
        P_LOG(LOG_DEBUG, "a : %f, %f, %f ; b : %f, %f, %f\n",
            contact.first.x, contact.first.y, contact.first.z,
            contact.second.x, contact.second.y, contact.second.z);
        auto center = ACollisionComponent::CalcCenterOfContact(contact);
        P_LOG(LOG_DEBUG, "center of contact : %f, %f, %f\n",
            center.x, center.y, center.z);
    }

    _aitc->GetActorObject("sp-particle-actor")->
        GetComponent<ATransformComponent>()->
        SetPosition(_aitc->GetActorObject("sp-point-light-actor")->
            GetComponent<ATransformComponent>()->
            GetProcessingPosition());
    _aitc->GetActorObject("sp-particle-actor")->
        GetComponent<ATransformComponent>()->
        TranslateYAsix(5.f);
}

void TestASpDestory(AInteractComponent*)
{
    P_LOG(LOG_DEBUG, "a sp destory\n");
}

void TestUSpInput(UInputComponent* _uic, Timer& _timer)
{
    float delta = _timer.FloatDeltaTime();
    auto utc = _uic->GetUiOwner()->
        GetComponent<UTransformComponent>();

    if (input::isKeyDownInSingle(KB_W))
    {
        utc->TranslateYAsix(0.1f * delta);
    }
    if (input::isKeyDownInSingle(KB_A))
    {
        utc->TranslateXAsix(-0.1f * delta);
    }
    if (input::isKeyDownInSingle(KB_S))
    {
        utc->TranslateYAsix(-0.1f * delta);
    }
    if (input::isKeyDownInSingle(KB_D))
    {
        utc->TranslateXAsix(0.1f * delta);
    }

    if (input::isKeyPushedInSingle(KB_Z))
    {
        _uic->GetUiOwner()->
            GetComponent<USpriteComponent>()->
            ResetTexture();
    }
    if (input::isKeyPushedInSingle(KB_X))
    {
        _uic->GetUiOwner()->
            GetComponent<UAnimateComponent>()->
            ChangeAnimateTo("number");
    }
    if (input::isKeyPushedInSingle(KB_C))
    {
        _uic->GetUiOwner()->
            GetComponent<UAnimateComponent>()->
            ChangeAnimateTo("runman");
    }

    if (input::isKeyPushedInSingle(KB_N))
    {
        _uic->GetUiOwner()->
            GetComponent<UAudioComponent>()->
            PlayBgm("test", 0.8f);
    }
    if (input::isKeyPushedInSingle(KB_M))
    {
        _uic->GetUiOwner()->
            GetComponent<UAudioComponent>()->
            PlayBgm("test", 0.4f);
    }

    if (input::isKeyPushedInSingle(KB_RETURN))
    {
        P_LOG(LOG_DEBUG, "to test1\n");
        _uic->GetSceneNode().GetSceneManager()->
            LoadSceneNode("sample1-scene", "sample1-scene.json");
    }
}

void TestUSpBtnInput(UInputComponent* _uic, Timer& _timer)
{
    auto ubc = _uic->GetUiOwner()->
        GetComponent<UButtonComponent>();
    if (!ubc) { return; }

    if (input::isKeyPushedInSingle(KB_UP))
    {
        ubc->SelectUpBtn();
    }
    if (input::isKeyPushedInSingle(KB_LEFT))
    {
        ubc->SelectLeftBtn();
    }
    if (input::isKeyPushedInSingle(KB_DOWN))
    {
        ubc->SelectDownBtn();
    }
    if (input::isKeyPushedInSingle(KB_RIGHT))
    {
        ubc->SelectRightBtn();
    }

    if (ubc->IsCursorOnBtn() && input::isKeyPushedInSingle(M_LEFTBTN))
    {
        P_LOG(LOG_DEBUG, "this btn has been click : %s\n", ubc->GetCompName().c_str());
    }
}

bool TestUSpInit(UInteractComponent* _uitc)
{
    P_LOG(LOG_DEBUG, "u sp init\n");
    return true;
}

void TestUSpUpdate(UInteractComponent* _uitc, Timer& _timer)
{

}

void TestUSpDestory(UInteractComponent* _uitc)
{
    P_LOG(LOG_DEBUG, "u sp destory\n");
}

void TempToTitle(AInputComponent* _aic, Timer&)
{
    if (input::isKeyPushedInSingle(KB_RCONTROL))
    {
        P_LOG(LOG_DEBUG, "to title\n");
        _aic->GetSceneNode().GetSceneManager()->
            LoadSceneNode("title-scene", "title-scene.json");
    }
}

void TempToSelect(AInputComponent* _aic, Timer&)
{
    if (input::isKeyPushedInSingle(KB_RCONTROL))
    {
        P_LOG(LOG_DEBUG, "to select\n");
        _aic->GetSceneNode().GetSceneManager()->
            LoadSceneNode("select-scene", "select-scene.json");
    }
}

void TempToRun(AInputComponent* _aic, Timer&)
{
    if (input::isKeyPushedInSingle(KB_RCONTROL))
    {
        P_LOG(LOG_DEBUG, "to run\n");
        _aic->GetSceneNode().GetSceneManager()->
            LoadSceneNode("run-scene", "run-scene.json");
    }
}

void TempToResult(AInputComponent* _aic, Timer&)
{
    if (input::isKeyPushedInSingle(KB_RCONTROL))
    {
        P_LOG(LOG_DEBUG, "to result\n");
        _aic->GetSceneNode().GetSceneManager()->
            LoadSceneNode("result-scene", "result-scene.json");
    }
}

static float g_AniSpdFactor = 50.f;
static AAnimateComponent* g_Aanc = nullptr;

bool AniInit(AInteractComponent* _aitc)
{
    P_LOG(LOG_DEBUG, "animate init\n");

    g_Aanc = _aitc->GetActorOwner()->
        GetComponent<AAnimateComponent>();
    if (!g_Aanc) { return false; }

    return true;
}

void AniUpdate(AInteractComponent* _aitc, Timer& _timer)
{
    _aitc->GetActorOwner()->
        GetComponent<ATransformComponent>()->
        RotateYAsix(_timer.FloatDeltaTime() / 1000.f);

    if (input::isKeyPushedInSingle(KB_1))
    {
        g_Aanc->ChangeAnimationTo("run");
    }
    else if (input::isKeyPushedInSingle(KB_2))
    {
        g_Aanc->ChangeAnimationTo("bite");
    }
    else if (input::isKeyPushedInSingle(KB_3))
    {
        g_Aanc->ChangeAnimationTo("roar");
    }
    else if (input::isKeyPushedInSingle(KB_4))
    {
        g_Aanc->ChangeAnimationTo("attack_tail");
    }
    else if (input::isKeyPushedInSingle(KB_5))
    {
        g_Aanc->ChangeAnimationTo("idle");
    }
    else if (input::isKeyPushedInSingle(KB_UP))
    {
        g_AniSpdFactor += 10.f;
        g_Aanc->SetSpeedFactor(g_AniSpdFactor);
    }
    else if (input::isKeyPushedInSingle(KB_DOWN))
    {
        g_AniSpdFactor -= 10.f;
        g_Aanc->SetSpeedFactor(g_AniSpdFactor);
    }
}

void AniDestory(AInteractComponent* _aitc)
{
    P_LOG(LOG_DEBUG, "animate destory\n");
    g_Aanc = nullptr;
}

static ATransformComponent* g_BBAtc = nullptr;
static float g_XFactor = 0.f;

bool BBInit(AInteractComponent* _aitc)
{
    g_XFactor = -1.f;
    g_BBAtc = _aitc->GetActorOwner()->
        GetComponent<ATransformComponent>();
    if (!g_BBAtc) { return false; }

    return true;
}

void BBUpdate(AInteractComponent* _aitc, Timer& _timer)
{
    g_BBAtc->TranslateXAsix(_timer.FloatDeltaTime() * g_XFactor * 0.01f);
    if (fabsf(g_BBAtc->GetProcessingPosition().x) > 18.f)
    {
        g_BBAtc->RollBackPositionX();
        g_XFactor *= -1.f;
    }
}

void BBDestory(AInteractComponent* _aitc)
{
    g_BBAtc = nullptr;
}
