#include "pch.h"
#include "CBossStage_LogicLayer.h"
#include "CCameraManager.h"
#include "CTerrain.h"
#include "CPlayer.h"
#include "CObjectCamera.h"
#include "CMonster.h"
#include "CMonster_Goblin_Hammer.h"
#include "CMonster_Goblin_Fireball.h"
#include "CMonster_Orge_Axe.h"
#include "CObjectManager.h"
#include "CMonster_Boss.h"
#include "CBossBarrier.h"

CBossStage_LogicLayer::CBossStage_LogicLayer()
{

}


HRESULT CBossStage_LogicLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
{


    // ============ 오브젝트 카메라 생성 =============

    if (CCameraManager::GetInstance()->Get_Camera(L"Main_Camera") == NULL)
    {
        Engine::CGameObject* pCameraObject = nullptr;
        pCameraObject = CObjectCamera::Create(pGraphicDev);

        if (nullptr == pCameraObject)
            return E_FAIL;

        if (FAILED(CCameraManager::GetInstance()->
            Ready_Camera(L"Main_Camera", pCameraObject)))
            return E_FAIL;

        //============ 카메라 컴포넌트와 오브젝트 카메라는 Scene 단위에서 관리하지 않는다 ================
        CCameraManager::GetInstance()->Set_MainCamera(L"Main_Camera");
        CCameraManager::GetInstance()->Set_MainCameraCom(static_cast<CObjectCamera*>(pCameraObject)->Get_Camera());
    }

    CObjectManager::GetInstance()->RegisterFactory(OBJ_MONSTER_BOSS, [=]() { return CMonster_Boss::Create(pGraphicDev); });
    CObjectManager::GetInstance()->RegisterFactory(OBJ_BARRIER, [=]() { return CBossBarrier::Create(pGraphicDev); });
    CObjectManager::GetInstance()->RegisterFactory(OBJ_EFFECT_BOSS_APPEAR, [=]() { return CBossAppearEffect::Create(pGraphicDev); });
    CCameraManager::GetInstance()->Set_CameraTarget(CObjectManager::GetInstance()->Get(OBJ_PLAYER));

    return S_OK;
}

_int	CBossStage_LogicLayer::Update_Layer(const _float& fTimeDelta)
{
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	CBossStage_LogicLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	CBossStage_LogicLayer::Render_Layer()
{

}

CBossStage_LogicLayer* CBossStage_LogicLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossStage_LogicLayer* pPlayer = new CBossStage_LogicLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void CBossStage_LogicLayer::Free()
{
    CLayer::Free();
}