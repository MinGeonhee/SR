#include "pch.h"
#include "CTutorialStage.h"
#include "CCollisionManager.h"
#include "CCameraManager.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CTutorialStage_EvnLayer.h"
#include "CTutorialStage_LogicLayer.h"
#include "CTutorialStage_UILayer.h"
#include "CLoading.h"
#include "CTimeManager.h"

static const wchar_t* kStageTimerTag = L"StageTimer";

CTutorialStage::CTutorialStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CTutorialStage::~CTutorialStage()
{
}



HRESULT CTutorialStage::Ready_Scene()
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

    auto* tm = CTimeManager::GetInstance();
    tm->Ready_Timer(kStageTimerTag);

    m_fTimer = 0.f;
    m_fClearTime = 0.f;
    m_bClear = false;
    m_fParTime = 120.f; //적정 클리어타임, 초단위로 입력해야됨

    return S_OK;
}

_int CTutorialStage::Update_Scene(const _float& fTimeDelta)
{
    CTimeManager::GetInstance()->Set_TimeDelta(kStageTimerTag);
    const float stageDt = CTimeManager::GetInstance()->Get_TimeDelta(kStageTimerTag);
    if (!m_bClear)
        m_fTimer += stageDt;

    Engine::CScene::Update_Scene(fTimeDelta);

    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CObjectManager::GetInstance()->Object_Update(fTimeDelta);
    CObjectPoolingManager::GetInstance()->Object_Update(fTimeDelta);


    return 0;
}

void CTutorialStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);

    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CObjectManager::GetInstance()->Object_LateUpdate(fTimeDelta);
    CObjectPoolingManager::GetInstance()->Object_LateUpdate(fTimeDelta);

    for (int i = 0; i < OBJ_END; ++i)
    {
        CCollisionManager::GetInstance()->Update_AllObjectList((OBJTYPE)i,
            CObjectManager::GetInstance()->Get_StaticObjectList((OBJTYPE)i),
            CObjectPoolingManager::GetInstance()->Get_DynamicObjectList((OBJTYPE)i));
    }

    CCollisionManager::GetInstance()->Update_Collision();
}

void CTutorialStage::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CTutorialStage::Ready_Environment_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CTutorialStage_EvnLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CTutorialStage::Ready_GameLogic_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CTutorialStage_LogicLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CTutorialStage::Ready_UI_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CTutorialStage_UILayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}


HRESULT CTutorialStage::Ready_Prototype()
{

    return S_OK;
}

HRESULT CTutorialStage::Ready_Manager()
{
    return S_OK;
}

CTutorialStage* CTutorialStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTutorialStage* pStage = new CTutorialStage(pGraphicDev);

    if (FAILED(pStage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pStage);
        return nullptr;
    }

    return pStage;
}

void CTutorialStage::Free()
{
    Engine::CScene::Free();
}

int CTutorialStage::ComputeScore(float clear, float par, int treasure)
{
    if (clear <= 0.f || par <= 0.f) return treasure;

    float ratio = clamp(par / clear, 0.25f, 2.5f);

    constexpr float BASE = 30000.f;
    constexpr float TIME_SCALE = 30000.f;

    float scoref = BASE + TIME_SCALE * ratio + (float)treasure;
    if (clear <= par) scoref += 3000.f;

    int score = (int)(scoref + 0.5f);
    return (score < 0) ? 0 : score;
}
