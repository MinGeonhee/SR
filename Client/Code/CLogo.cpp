#include "pch.h"
#include "CLogo.h"
#include "CBackGround.h"
#include "CProtoTypeManager.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CTerrain.h"
#include "CStage.h"
#include "CResourceManager.h"
#include "CManagement.h"
#include "CInputManager.h"
#include "CSceneManager.h"
#include "Logo_UiLayer.h"
#include "CSoundManager.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    CSoundManager::GetInstance()->PlayBGM(L"Main Menu Theme.wav", CSoundManager::GetInstance()->GetVol().music);

    return S_OK;

}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    if (GetAsyncKeyState(VK_RETURN))
    {
        CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE);
    }

    return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
    // debug¿ë ·»´õ
}

HRESULT CLogo::Ready_Environment_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag , pLayer });

    return S_OK;
}

HRESULT CLogo::Ready_UI_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = Logo_UiLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogo* pLogo = new CLogo(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("Logo Create Failed");
        Safe_Release(pLogo);
        return nullptr;
    }

    return pLogo;
}

void CLogo::Free()
{
    Engine::CScene::Free();
}
