#include "RenderSystem.h"
#include "RSRoot_DX11.h"
#include "RSPipelinesManager.h"
#include "RSDevices.h"
#include "RSDrawCallsPool.h"
#include "RSCamera.h"
#include "RSCamerasContainer.h"
#include "RSResourceManager.h"
#include "RSLightsContainer.h"
#include "RSParticlesContainer.h"
#include "WM_Interface.h"
#include "BasicRSPipeline.h"
#include "SystemExecutive.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "AssetsPool.h"
#include "UButtonComponent.h"

RenderSystem::RenderSystem(SystemExecutive* _sysExecutive) :
    System("render-system", _sysExecutive),
    mRenderSystemRoot(nullptr), mAssetsPool(nullptr),
    mEnvTex(nullptr), mDiffTex(nullptr), mSpecTex(nullptr)
{

}

RenderSystem::~RenderSystem()
{

}

bool RenderSystem::Init()
{
    if (!mRenderSystemRoot)
    {
        mRenderSystemRoot = new RSRoot_DX11();
        if (!mRenderSystemRoot->
            startUp(window::getWindowPtr()->getWndHandle()))
        {
            return false;
        }

        std::string name = "temp-cam";
        CAM_INFO ci = {};
        ci.Type = LENS_TYPE::PERSPECTIVE;
        ci.Position = { 0.f,0.f,0.f };
        ci.LookAtVector = { 0.f,0.f,1.f };
        ci.UpVector = { 0.f,1.f,0.f };
        ci.NearFarZ = { 1.f,800.f };
        ci.PerspFovYRatio = { DirectX::XM_PIDIV4,16.f / 9.f };
        ci.OrthoWidthHeight = { 12.8f,7.2f };
        mRenderSystemRoot->getCamerasContainer()->createRSCamera(name, &ci);

        name = "temp-ui-cam";
        ci = {};
        ci.Type = LENS_TYPE::ORTHOGRAPHIC;
        ci.Position = { 0.f,0.f,0.f };
        ci.LookAtVector = { 0.f,0.f,1.f };
        ci.UpVector = { 0.f,1.f,0.f };
        ci.NearFarZ = { 1.f,100.f };
        ci.PerspFovYRatio = { DirectX::XM_PIDIV4,16.f / 9.f };
        ci.OrthoWidthHeight = { 1280.f,720.f };
        mRenderSystemRoot->getCamerasContainer()->createRSCamera(name, &ci);

        if (!createBasicPipeline()) { return false; }
    }

    mAssetsPool = nullptr;
    mAssetsPool = GetSystemExecutive()->GetSceneManager()->
        getCurrentSceneNode()->getAssetsPool();
    if (!mAssetsPool) { return false; }

    mEnvTex = GetSystemExecutive()->GetSceneManager()->
        getCurrentSceneNode()->getIBLEnvironment();
    mDiffTex = GetSystemExecutive()->GetSceneManager()->
        getCurrentSceneNode()->getIBLDiffuse();
    mSpecTex = GetSystemExecutive()->GetSceneManager()->
        getCurrentSceneNode()->getIBLSpecular();

    getRSDX11RootInstance()->getParticlesContainer()->resetRSParticleSystem();

    return true;
}

void RenderSystem::Run(Timer& _timer)
{
    mRenderSystemRoot = getRSDX11RootInstance();
#ifdef _DEBUG
    assert(mRenderSystemRoot);
#endif // _DEBUG

    static RS_DRAWCALL_DATA drawCall = {};
    auto& meshPool = mAssetsPool->SubMeshPool;
    RS_DRAWCALL_DATA btnSelectFlg = {};
    bool hasBtnSelect = false;
    for (auto& mesh : meshPool)
    {
        mesh.second.InstanceVector.clear();
        for (auto& instance : mesh.second.InstanceMap)
        {
            mesh.second.InstanceVector.emplace_back(instance.second);
        }
        if (!mesh.second.InstanceVector.size()) { continue; }

        mesh.second.BonesVector.clear();
        for (auto& bone : mesh.second.BonesMap)
        {
            mesh.second.BonesVector.emplace_back(bone.second);
        }

        auto drawCallPool = mRenderSystemRoot->getDrawCallsPool();
        DRAWCALL_TYPE dType = DRAWCALL_TYPE::MAX;
        MESH_TYPE mType = mesh.second.MeshType;
        switch (mType)
        {
        case MESH_TYPE::OPACITY: dType = DRAWCALL_TYPE::OPACITY; break;
        case MESH_TYPE::TRANSPARENCY: dType = DRAWCALL_TYPE::TRANSPARENCY; break;
        case MESH_TYPE::LIGHT: dType = DRAWCALL_TYPE::LIGHT; break;
        case MESH_TYPE::UI_SPRITE: dType = DRAWCALL_TYPE::UI_SPRITE; break;
        default: break;
        }

        drawCall = {};
        drawCall.MeshData.InputLayout = mesh.second.MeshData.InputLayout;
        drawCall.MeshData.TopologyType = mesh.second.MeshData.TopologyType;
        drawCall.MeshData.VertexBuffer = mesh.second.MeshData.VertexBuffer;
        drawCall.MeshData.IndexBuffer = mesh.second.MeshData.IndexBuffer;
        drawCall.MeshData.IndexSize = mesh.second.MeshData.IndexSize;
        drawCall.InstanceData.DataArrayPtr = &(mesh.second.InstanceVector);
        drawCall.InstanceData.BonesArrayPtr = &(mesh.second.BonesVector);
        auto texSize = mesh.second.MeshData.Textures.size();
        auto& texArray = mesh.second.MeshData.Textures;
        for (size_t i = 0; i < texSize; i++)
        {
            if (texArray[i] != "")
            {
                drawCall.TextureData[i].EnabledFlag = true;
                drawCall.TextureData[i].Srv = mRenderSystemRoot->
                    getResourceManager()->getMeshSrv(texArray[i]);
            }
        }

        drawCallPool->addDrawCallToPipe(dType, drawCall);
        if (mesh.first == SELECTED_BTN_SPRITE_NAME)
        {
            hasBtnSelect = true;
            btnSelectFlg = drawCall;
        }
    }
    if (hasBtnSelect)
    {
        mRenderSystemRoot->getDrawCallsPool()->addDrawCallToPipe(
            DRAWCALL_TYPE::UI_SPRITE, btnSelectFlg);
    }
    mRenderSystemRoot->getLightsContainer()->uploadLightBloomDrawCall();

    mRenderSystemRoot->getLightsContainer()->forceCurrentAmbientLight(
        GetSystemExecutive()->GetSceneManager()->
        getCurrentSceneNode()->getCurrentAmbientFactor());

    setPipelineIBLTextures(mEnvTex, mDiffTex, mSpecTex);
    setPipelineDeltaTime(_timer.floatDeltaTime());

    getRSDX11RootInstance()->getPipelinesManager()->changeToNextPipeline();
    mRenderSystemRoot->getPipelinesManager()->execuateCurrentPipeline();
    mRenderSystemRoot->getDevices()->presentSwapChain();
    mRenderSystemRoot->getDrawCallsPool()->clearAllDrawCalls();
}

void RenderSystem::Destory()
{
    mRenderSystemRoot->cleanAndStop();
    delete mRenderSystemRoot;
}
