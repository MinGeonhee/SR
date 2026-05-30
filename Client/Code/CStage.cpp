#include "pch.h"
#include "CStage.h"
#include "Cplayer.h"
#include "CProtoTypeManager.h"
#include "CMonster.h"
#include "CPlane.h"
#include "CTerrain.h"
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
#include "CLoading.h"
#include "CSceneManager.h"
#include "CEventbus.h"
#include "CInputManager.h"
#include "CSoundManager.h"
#include "CLightManager.h"
#include "CUIManager.h"
#include "CTimeManager.h"
#include "CScoreBoard.h"

static const wchar_t* kStageTimerTag = L"StageTimer";

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{

}

HRESULT CStage::Ready_Scene()
{
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    CSoundManager::GetInstance()->PlayBGM(L"st1_loop_bgm.wav", CSoundManager::GetInstance()->GetVol().music);

    Ready_Manager();
    Ready_Prototype();

    auto* tm = CTimeManager::GetInstance();
    tm->Ready_Timer(kStageTimerTag);

    m_fTimer = 0.f;
    m_fClearTime = 0.f;
    m_bClear = false;
    m_fParTime = 120.f; //적정 클리어타임, 초단위로 입력해야됨

    return S_OK;
}

_int CStage::Update_Scene(const _float& dt)
{
    CTimeManager::GetInstance()->Set_TimeDelta(kStageTimerTag);
    const float stageDt = CTimeManager::GetInstance()->Get_TimeDelta(kStageTimerTag);
    if (!m_bClear)
        m_fTimer += stageDt;

    Engine::CScene::Update_Scene(dt);

    if (CUIManager::GetInstance()->IsCleared())
        Clear();

    if (GetAsyncKeyState(VK_RETURN))
    {
        CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE2);
        return 0; // 전환 직후 남은 업데이트 스킵
    }

    CCameraManager::GetInstance()->Update_MainCamera(dt);
    CObjectManager::GetInstance()->Object_Update(dt);
    CObjectPoolingManager::GetInstance()->Object_Update(dt);

    Engine::CScene::Update_Scene(dt);
    return 0;
    
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);

    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CObjectManager::GetInstance()->Object_LateUpdate(fTimeDelta);
    CObjectPoolingManager::GetInstance()->Object_LateUpdate(fTimeDelta);

    for (int i = 0; i < OBJ_END; ++i)
    {
        CCollisionManager::GetInstance()->Update_AllObjectList((OBJTYPE)i,
        CObjectManager::GetInstance()->Get_VisibleObjectList((OBJTYPE)i),
        CObjectPoolingManager::GetInstance()->Get_DynamicObjectList((OBJTYPE)i));
    }

    CCollisionManager::GetInstance()->Update_Collision();
}

void CStage::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CStage::Ready_Environment_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = Stage1_EnvLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = Stage1_LogicLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = Stage1_UILayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    D3DLIGHT9       tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

    tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    tLightInfo.Direction = { 1.f, -1.f, 1.f };

    if (FAILED(CLightManager::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
        return E_FAIL;

    return S_OK;
}

HRESULT CStage::Ready_Manager()
{
 /*   CSoundManager::GetInstance()->PlayBGM(L"-Last-Engage-.wav", 1.f);*/
    return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage* pStage = new CStage(pGraphicDev);

    if (FAILED(pStage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pStage);
        return nullptr;
    }

    return pStage;
}

void CStage::Free()
{
    Engine::CScene::Free();
}

//지피티가 역산해줌 스코어 공식
int CStage::ComputeScore(float clear, float par, int treasure)
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

bool CStage::Clear()
{
    if (m_bClear) return false;

    m_bClear = true;
    m_fClearTime = m_fTimer;
    m_iClearScore = ComputeScore(m_fClearTime, m_fParTime, m_iTreasure);

    const SCENETYPE stage = STAGE1;
    CScoreBoard::GetInstance()->CommitStageResult(
        stage, m_iClearScore, m_iTreasure, m_fParTime, m_fClearTime);

    CUIManager::GetInstance()->ActivateUI(L"StageClearUI");
    return true;
}