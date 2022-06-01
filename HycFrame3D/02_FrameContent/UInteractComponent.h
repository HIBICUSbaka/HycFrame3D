#pragma once

#include "UiComponent.h"

using UiInteractInitFuncType = bool(*)(class UInteractComponent*);
using UiInteractUpdateFuncType = void(*)(class UInteractComponent*, Timer&);
using UiInteractDestoryFuncType = void(*)(class UInteractComponent*);

class UInteractComponent :public UiComponent
{
public:
    UInteractComponent(std::string&& _compName, class UiObject* _uiOwner);
    UInteractComponent(std::string& _compName, class UiObject* _uiOwner);
    virtual ~UInteractComponent();

    UInteractComponent& operator=(const UInteractComponent& _source)
    {
        if (this == &_source) { return *this; }
        mInitProcessFunctionPtr = _source.mInitProcessFunctionPtr;
        mUpdateProcessFunctionPtr = _source.mUpdateProcessFunctionPtr;
        mDestoryProcessFunctionPtr = _source.mDestoryProcessFunctionPtr;
        UiComponent::operator=(_source);
        return *this;
    }

public:
    virtual bool Init();
    virtual void Update(Timer& _timer);
    virtual void Destory();

public:
    void SetInitFunction(UiInteractInitFuncType _initFunc);
    void SetUpdateFunction(UiInteractUpdateFuncType _updateFunc);
    void SetDestoryFunction(UiInteractDestoryFuncType _destoryFunc);
    void ClearInitFunction();
    void ClearUpdateFunction();
    void ClearDestoryFunction();

private:
    UiInteractInitFuncType mInitProcessFunctionPtr;
    UiInteractUpdateFuncType mUpdateProcessFunctionPtr;
    UiInteractDestoryFuncType mDestoryProcessFunctionPtr;
};
