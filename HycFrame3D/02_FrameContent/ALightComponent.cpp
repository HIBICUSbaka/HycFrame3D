#include "ALightComponent.h"
#include "ActorObject.h"
#include "SceneNode.h"
#include "AssetsPool.h"
#include "ATransformComponent.h"
#include <vector>
#include "RSLight.h"
#include "RSRoot_DX11.h"
#include "RSLightsContainer.h"
#include "RSMeshHelper.h"

ALightComponent::ALightComponent(std::string&& _compName,
    ActorObject* _actorOwner) :
    ActorComponent(_compName, _actorOwner),
    mLightName(""),
    mRSLightPtr(nullptr),
    mCanCreateLight(false),
    mLightInfoForInit({}),
    mLightCamInfoForInit({}),
    mIsBloom(false),
    mIsCamera(false)
{

}

ALightComponent::ALightComponent(std::string& _compName,
    ActorObject* _actorOwner) :
    ActorComponent(_compName, _actorOwner),
    mLightName(""),
    mRSLightPtr(nullptr),
    mCanCreateLight(false),
    mLightInfoForInit({}),
    mLightCamInfoForInit({}),
    mIsBloom(false),
    mIsCamera(false)
{

}

ALightComponent::~ALightComponent()
{

}

bool ALightComponent::Init()
{
    if (!mCanCreateLight) { return false; }

    CreateLight();

    if (mRSLightPtr) { return true; }
    else { return false; }
}

void ALightComponent::Update(Timer& _timer)
{
    SyncDataFromTransform();
}

void ALightComponent::Destory()
{
    getRSDX11RootInstance()->getLightsContainer()->
        deleteRSLight(mLightName, true);
}

void ALightComponent::CreateLight()
{
    mLightName = GetCompName();
    mRSLightPtr = getRSDX11RootInstance()->getLightsContainer()->
        createRSLight(mLightName, &mLightInfoForInit);
#ifdef _DEBUG
    assert(mRSLightPtr);
#endif // _DEBUG

    if (mIsBloom)
    {
        SUBMESH_DATA* mesh = GetActorOwner()->GetSceneNode().GetAssetsPool()->
            GetSubMeshIfExisted(BOX_BLOOM_MESH_NAME);
        if (!mesh)
        {
            RS_SUBMESH_DATA boxBloom = getRSDX11RootInstance()->
                getMeshHelper()->getGeoGenerator()->
                createBox(1.f, 1.f, 1.f, 0, LAYOUT_TYPE::NORMAL_COLOR);
            GetActorOwner()->GetSceneNode().GetAssetsPool()->InsertNewSubMesh(
                BOX_BLOOM_MESH_NAME, boxBloom, MESH_TYPE::LIGHT);
            mesh = GetActorOwner()->GetSceneNode().GetAssetsPool()->
                GetSubMeshIfExisted(BOX_BLOOM_MESH_NAME);
        }
        mRSLightPtr->setLightBloom(mesh->mMeshData);
    }

    if (mIsCamera)
    {
        bool cam_create = getRSDX11RootInstance()->getLightsContainer()->
            createLightCameraFor(mLightName, &mLightCamInfoForInit);
#ifdef _DEBUG
        assert(cam_create);
#endif // _DEBUG
        (void)cam_create;
    }
}

void ALightComponent::ResetLight(LIGHT_INFO* _lightInfo)
{
    mRSLightPtr->resetRSLight(_lightInfo);
}

RSLight* ALightComponent::GetLightInfo()
{
    return mRSLightPtr;
}

void ALightComponent::SyncDataFromTransform()
{
    ATransformComponent* atc = GetActorOwner()->
        GetComponent<ATransformComponent>();
#ifdef _DEBUG
    assert(atc);
#endif // _DEBUG

    DirectX::XMFLOAT3 world = atc->GetPosition();
    DirectX::XMFLOAT3 angle = atc->GetRotation();
    DirectX::XMFLOAT3 scale = atc->GetScaling();

    mRSLightPtr->setRSLightPosition(world);

    if (mIsBloom)
    {
        DirectX::XMMATRIX mat = {};
        mat = DirectX::XMMatrixMultiply(
            DirectX::XMMatrixScaling(scale.x, scale.y, scale.z),
            DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z));
        mat = DirectX::XMMatrixMultiply(mat,
            DirectX::XMMatrixTranslation(world.x, world.y, world.z));
        DirectX::XMStoreFloat4x4(mRSLightPtr->getLightWorldMat(), mat);
    }
}

void ALightComponent::AddLight(LIGHT_INFO& _lightInfo,
    bool _setBloom, bool _setCamera, CAM_INFO& _camInfo)
{
    mLightInfoForInit = _lightInfo;
    mLightCamInfoForInit = _camInfo;
    mIsBloom = _setBloom;
    mIsCamera = _setCamera;
    mCanCreateLight = true;
}

void ALightComponent::AddLight(LIGHT_INFO& _lightInfo,
    bool _setBloom, bool _setCamera, CAM_INFO&& _camInfo)
{
    mLightInfoForInit = _lightInfo;
    mLightCamInfoForInit = _camInfo;
    mIsBloom = _setBloom;
    mIsCamera = _setCamera;
    mCanCreateLight = true;
}
