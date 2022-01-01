#include "SPInput.h"

void RegisterSPInput(ObjectFactory* _factory)
{
#ifdef _DEBUG
    assert(_factory);
#endif // _DEBUG
    _factory->GetAInputMapPtr()->insert(
        { FUNC_NAME(TestASpInput),TestASpInput });
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
}

void TestASpInput(AInputComponent* _aic, Timer& _timer)
{
    if (InputInterface::IsKeyPushedInSingle(KB_RETURN))
    {
        P_LOG(LOG_DEBUG, "to test2\n");
        _aic->GetActorOwner()->GetSceneNode().GetSceneManager()->
            LoadSceneNode("sample2-scene", "sample2-scene.json");
    }

    if (InputInterface::IsKeyDownInSingle(KB_W))
    {
        _aic->GetActorOwner()->GetSceneNode().
            GetActorObject("sp-point-light-actor")->
            GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
            TranslateZAsix(0.1f * _timer.FloatDeltaTime());
    }
    if (InputInterface::IsKeyDownInSingle(KB_A))
    {
        _aic->GetActorOwner()->GetSceneNode().
            GetActorObject("sp-point-light-actor")->
            GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
            TranslateXAsix(-0.1f * _timer.FloatDeltaTime());
    }
    if (InputInterface::IsKeyDownInSingle(KB_S))
    {
        _aic->GetActorOwner()->GetSceneNode().
            GetActorObject("sp-point-light-actor")->
            GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
            TranslateZAsix(-0.1f * _timer.FloatDeltaTime());
    }
    if (InputInterface::IsKeyDownInSingle(KB_D))
    {
        _aic->GetActorOwner()->GetSceneNode().
            GetActorObject("sp-point-light-actor")->
            GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
            TranslateXAsix(0.1f * _timer.FloatDeltaTime());
    }
}

bool TestASpInit(AInteractComponent* _aitc)
{
    P_LOG(LOG_DEBUG, "a sp init\n");

    _aitc->GetActorOwner()->
        GetAComponent<ATimerComponent>(COMP_TYPE::A_TIMER)->
        StartTimer("timer1");

    return true;
}
void TestASpUpdate(AInteractComponent* _aitc, Timer&)
{
    //P_LOG(LOG_DEBUG, "a sp update\n");
    /*float time0 = _aitc->GetActorOwner()->
        GetAComponent<ATimerComponent>(COMP_TYPE::A_TIMER)->
        GetTimer("timer0")->mTime;
    float time1 = _aitc->GetActorOwner()->
        GetAComponent<ATimerComponent>(COMP_TYPE::A_TIMER)->
        GetTimer("timer1")->mTime;
    P_LOG(LOG_DEBUG, "timer0 : %f , timer1 : %f\n", time0, time1);*/

    CONTACT_PONT_PAIR contact = {};
    if (_aitc->GetActorOwner()->GetSceneNode().
        GetActorObject("sp-point-light-actor")->
        GetAComponent<ACollisionComponent>(COMP_TYPE::A_COLLISION)->
        CheckCollisionWith("sp-actor", &contact))
    {
        _aitc->GetActorOwner()->GetSceneNode().
            GetActorObject("sp-point-light-actor")->
            GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
            RollBackPosition();
        P_LOG(LOG_DEBUG, "a : %f, %f, %f ; b : %f, %f, %f\n",
            contact.first.x, contact.first.y, contact.first.z,
            contact.second.x, contact.second.y, contact.second.z);
        auto center = ACollisionComponent::CalcCenterOfContact(contact);
        P_LOG(LOG_DEBUG, "center of contact : %f, %f, %f\n",
            center.x, center.y, center.z);
    }

    _aitc->GetActorOwner()->GetSceneNode().
        GetActorObject("sp-particle-actor")->
        GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
        SetPosition(_aitc->GetActorOwner()->GetSceneNode().
            GetActorObject("sp-point-light-actor")->
            GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
            GetProcessingPosition());
    _aitc->GetActorOwner()->GetSceneNode().
        GetActorObject("sp-particle-actor")->
        GetAComponent<ATransformComponent>(COMP_TYPE::A_TRANSFORM)->
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
        GetUComponent<UTransformComponent>(COMP_TYPE::U_TRANSFORM);

    if (InputInterface::IsKeyDownInSingle(KB_W))
    {
        utc->TranslateYAsix(0.1f * delta);
    }
    if (InputInterface::IsKeyDownInSingle(KB_A))
    {
        utc->TranslateXAsix(-0.1f * delta);
    }
    if (InputInterface::IsKeyDownInSingle(KB_S))
    {
        utc->TranslateYAsix(-0.1f * delta);
    }
    if (InputInterface::IsKeyDownInSingle(KB_D))
    {
        utc->TranslateXAsix(0.1f * delta);
    }

    if (InputInterface::IsKeyPushedInSingle(KB_Z))
    {
        _uic->GetUiOwner()->
            GetUComponent<USpriteComponent>(COMP_TYPE::U_SPRITE)->
            ResetTexture();
    }
    if (InputInterface::IsKeyPushedInSingle(KB_X))
    {
        _uic->GetUiOwner()->
            GetUComponent<UAnimateComponent>(COMP_TYPE::U_ANIMATE)->
            ChangeAnimateTo("number");
    }
    if (InputInterface::IsKeyPushedInSingle(KB_C))
    {
        _uic->GetUiOwner()->
            GetUComponent<UAnimateComponent>(COMP_TYPE::U_ANIMATE)->
            ChangeAnimateTo("runman");
    }

    if (InputInterface::IsKeyPushedInSingle(KB_N))
    {
        _uic->GetUiOwner()->
            GetUComponent<UAudioComponent>(COMP_TYPE::U_AUDIO)->
            PlayBgm("test", 0.8f);
    }
    if (InputInterface::IsKeyPushedInSingle(KB_M))
    {
        _uic->GetUiOwner()->
            GetUComponent<UAudioComponent>(COMP_TYPE::U_AUDIO)->
            PlayBgm("test", 0.4f);
    }

    if (InputInterface::IsKeyPushedInSingle(KB_RETURN))
    {
        P_LOG(LOG_DEBUG, "to test1\n");
        _uic->GetUiOwner()->GetSceneNode().GetSceneManager()->
            LoadSceneNode("sample1-scene", "sample1-scene.json");
    }
}

void TestUSpBtnInput(UInputComponent* _uic, Timer& _timer)
{
    auto ubc = _uic->GetUiOwner()->
        GetUComponent<UButtonComponent>(COMP_TYPE::U_BUTTON);
    if (!ubc) { return; }

    if (InputInterface::IsKeyPushedInSingle(KB_UP))
    {
        ubc->SelectUpBtn();
    }
    if (InputInterface::IsKeyPushedInSingle(KB_LEFT))
    {
        ubc->SelectLeftBtn();
    }
    if (InputInterface::IsKeyPushedInSingle(KB_DOWN))
    {
        ubc->SelectDownBtn();
    }
    if (InputInterface::IsKeyPushedInSingle(KB_RIGHT))
    {
        ubc->SelectRightBtn();
    }

    if (ubc->IsCursorOnBtn() && InputInterface::IsKeyPushedInSingle(M_LEFTBTN))
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

