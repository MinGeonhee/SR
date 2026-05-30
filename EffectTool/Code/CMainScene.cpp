#include "CMainScene.h"
#include "CGrid.h"
#include "CParticleSystem.h"
#include "CProtoTypeManager.h"
#include "CResourceManager.h"
#include "CCameraManager.h"
#include "CCameraMan.h"

CMainScene::CMainScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Resource()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CMainScene::Update_Scene(const _float& fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    
    return iExit;
}

void CMainScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
}

void CMainScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
    
}

HRESULT CMainScene::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (pLayer == nullptr)
        return E_FAIL;
    
    Add_Cameraman(pLayer);

    
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CMainScene::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();

    if (pLayer == nullptr)
        return E_FAIL;

    // 바닥
    Engine::CGameObject* pGrid = nullptr;
    pGrid = CGrid::Create(m_pGraphicDev);

    pGrid->Set_OBJTYPE(OBJ_TERRAIN);

    if (nullptr == pGrid)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"Grid", pGrid)))
        return E_FAIL;


    // 파티클 시스템
    Engine::CGameObject* pPS = nullptr;
    pPS = CParticleSystem::Create(m_pGraphicDev);

    pPS->Set_OBJTYPE(OBJ_NONE);

    if (pPS == NULL)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"Particle System", pPS)))
        return E_FAIL;


    
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CMainScene::Ready_UI_Layer(const _tchar* pLayerTag)
{
    
    return S_OK;
}

HRESULT CMainScene::Ready_Prototype()
{
    
    REGISTER_PROTOTYPE(PROTO_RECTCOL, CRectCol, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_TERRAINCOL, CTerrainCol, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_CUBECOL, CCubeCol, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_PARTICLERECTEX, CParticleRecTex, m_pGraphicDev);

    REGISTER_PROTOTYPE(PROTO_TRANS, CTransform, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_CAMERA, CCamera, m_pGraphicDev);

    return S_OK;
}

HRESULT CMainScene::Ready_Resource()
{
    CResourceManager::GetInstance()->Load<CTexture>(L"ParticleTex", L"../../Client/Bin/Resource/Texture/Particle/particles.png", m_pGraphicDev);
    return S_OK;
}

HRESULT CMainScene::Add_Cameraman(CLayer* pLayer)
{
    //카메라맨 추가
    CCameraMan* newObj = nullptr;
    newObj = CCameraMan::Create(m_pGraphicDev);

    if (newObj == nullptr)
        return E_FAIL;

    CCameraManager::GetInstance()->Ready_Camera(L"ToolCam", newObj);
    CCameraManager::GetInstance()->Set_MainCamera(L"ToolCam");
    CCameraManager::GetInstance()->Set_MainCameraCom(newObj->Get_Camera());


    return S_OK;
}

CMainScene* CMainScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMainScene* pScene = new CMainScene(pGraphicDev);

    if (FAILED(pScene->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pScene);
        return nullptr;
    }

    return pScene;
}

void CMainScene::Free()
{
    Engine::CScene::Free();
}
