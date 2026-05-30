#include "pch.h"
#include "CBossStage.h"
#include "CPlayer.h"
#include "CProtoTypeManager.h"
#include "CMonster.h"
#include "CPlane.h"
#include "CTerrain.h"
#include "CObjectCamera.h"
#include "CCameraManager.h"
#include "CResourceManager.h"
#include "CBossStage_EnvLayer.h"
#include "CBossStage_LogicLayer.h"
#include "CBossStage_UILayer.h"
#include "CCollisionManager.h"
#include "CDebugLine.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CLoading.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CSoundManager.h"

static const wchar_t* kStageTimerTag = L"StageTimer";

CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene()
{
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    Ready_Manager();

    auto* tm = CTimeManager::GetInstance();
    tm->Ready_Timer(kStageTimerTag);

    CSoundManager::GetInstance()->PlayBGM(L"stboss_loop_bgm.wav", CSoundManager::GetInstance()->GetVol().music);
    CSoundManager::GetInstance()->SoundPlay(L"ambient-rain-heavy.wav", EMBIENT_SOUND, CSoundManager::GetInstance()->GetVol().ambient, true);

    m_fTimer = 0.f;
    m_fClearTime = 0.f;
    m_bClear = false;
    m_fParTime = 120.f; //적정 클리어타임, 초단위로 입력해야됨

    return S_OK;
}

_int CBossStage::Update_Scene(const _float& fTimeDelta)
{
    CTimeManager::GetInstance()->Set_TimeDelta(kStageTimerTag);
    const float stageDt = CTimeManager::GetInstance()->Get_TimeDelta(kStageTimerTag);
    if (!m_bClear)
        m_fTimer += stageDt;

    Engine::CScene::Update_Scene(fTimeDelta);

    if (GetAsyncKeyState(VK_RETURN))
    {
        CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS2);
        return 0; // 전환 직후 남은 업데이트 스킵
    }

    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CObjectManager::GetInstance()->Object_Update(fTimeDelta);
    CObjectPoolingManager::GetInstance()->Object_Update(fTimeDelta);

    return 0;
}

void CBossStage::LateUpdate_Scene(const _float& fTimeDelta)
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

void CBossStage::Render_Scene()
{
    Engine::CScene::Render_Scene();

    // 포그 활성화
    m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
    // 포그 색상 지정 (ARGB)
    m_pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(10, 10, 25));
    // 또는 거리 기반 지수 포그
    m_pGraphicDev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP);
    float fDensity = 0.1f;
    m_pGraphicDev->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&fDensity));
}

HRESULT CBossStage::Ready_Environment_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CBossStage_EnvLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CBossStage::Ready_GameLogic_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CBossStage_LogicLayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CBossStage::Ready_UI_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CBossStage_UILayer::Create(m_pGraphicDev);
    if (pLayer == nullptr)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}


HRESULT CBossStage::Ready_Prototype()
{

    return S_OK;
}

HRESULT CBossStage::Ready_Manager()
{
    return S_OK;
}

CBossStage* CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossStage* pStage = new CBossStage(pGraphicDev);

    if (FAILED(pStage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pStage);
        return nullptr;
    }

    return pStage;
}

void CBossStage::Free() 
{
    Engine::CScene::Free();
}

int CBossStage::ComputeScore(float clear, float par, int treasure)
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
