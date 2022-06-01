#pragma once

#include "UiComponent.h"
#include <DirectXMath.h>
#include <unordered_map>

struct ANIMATE_INFO
{
    std::string mTexName = "";
    DirectX::XMFLOAT2 mStride = { 0.f, 0.f };
    unsigned int mMaxCut = 0;
    bool mRepeatFlg = false;
    float mSwitchTime = 0.f;
};

class UAnimateComponent :public UiComponent
{
public:
    UAnimateComponent(std::string&& _compName, class UiObject* _uiOwner);
    UAnimateComponent(std::string& _compName, class UiObject* _uiOwner);
    virtual ~UAnimateComponent();

    UAnimateComponent& operator=(const UAnimateComponent& _source)
    {
        if (this == &_source) { return *this; }
        mAnimateMap = _source.mAnimateMap;
        mCurrentAnimateCut = _source.mCurrentAnimateCut;
        mCurrentAnimate = _source.mCurrentAnimate;
        mAnimateChangedFlg = _source.mAnimateChangedFlg;
        mTimeCounter = _source.mTimeCounter;
        UiComponent::operator=(_source);
        return *this;
    }

public:
    virtual bool Init();
    virtual void Update(Timer& _timer);
    virtual void Destory();

public:
    bool LoadAnimate(std::string _aniName, std::string _aniPath,
        DirectX::XMFLOAT2 _stride, UINT _maxCount,
        bool _repeatFlg, float _switchTime);
    void DeleteAnimate(std::string&& _aniName);
    void DeleteAnimate(std::string& _aniName);

    void ResetCurrentAnimate();
    void ClearCurrentAnimate();

    void ChangeAnimateTo(std::string&& _aniName);
    void ChangeAnimateTo(std::string& _aniName);

private:
    void SyncAniInfoToSprite();

private:
    std::unordered_map<std::string, ANIMATE_INFO*> mAnimateMap;
    UINT mCurrentAnimateCut;
    ANIMATE_INFO* mCurrentAnimate;
    bool mAnimateChangedFlg;
    float mTimeCounter;
};
