#include "pch.h"
#include "CMain.h"
#include "CProtoTypeManager.h"
#include "CObjectCamera.h"
#include "CCameraManager.h"
#include "CResourceManager.h"
#include "Stage1_EnvLayer.h"
#include "Stage1_LogicLayer.h"
#include "Stage1_UILayer.h"
#include "CCollisionManager.h"
#include "CDebugLine.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CBackGround.h"

CMain::CMain(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CMain::~CMain()
{
}

HRESULT CMain::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    Ready_Manager();

    return S_OK;
}

_int CMain::Update_Scene(const _float& fTimeDelta)
{
    Engine::CScene::Update_Scene(fTimeDelta);
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CObjectManager::GetInstance()->Object_Update(fTimeDelta);
    CObjectPoolingManager::GetInstance()->Object_Update(fTimeDelta);
    CCollisionManager::GetInstance()->Update_Collision();


    return 0;
}

void CMain::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CObjectManager::GetInstance()->Object_LateUpdate(fTimeDelta);
    CObjectPoolingManager::GetInstance()->Object_LateUpdate(fTimeDelta);
}

void CMain::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CMain::Ready_Environment_Layer(const wstring& pLayerTag)
{

    Engine::CLayer* pLayer = CLayer::Create();
    if (pLayer == nullptr)
        return E_FAIL;

    return S_OK;
}

HRESULT CMain::Ready_GameLogic_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (pLayer == nullptr)
        return E_FAIL;
    return S_OK;
}

HRESULT CMain::Ready_UI_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}


HRESULT CMain::Ready_Prototype()
{

    return S_OK;
}

HRESULT CMain::Ready_Manager()
{
    CCollisionManager::GetInstance()->InitCollisionTable();

    return S_OK;
}

CMain* CMain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMain* pStage = new CMain(pGraphicDev);

    if (FAILED(pStage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pStage);
        return nullptr;
    }

    return pStage;
}

void CMain::Free()
{
    Engine::CScene::Free();
}
