#include "pch.h"
#include "CTutorialStage_EvnLayer.h"
#include "CCameraManager.h"

CTutorialStage_EvnLayer::CTutorialStage_EvnLayer()
{
}
CTutorialStage_EvnLayer::~CTutorialStage_EvnLayer()
{

}

HRESULT CTutorialStage_EvnLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
{

    return S_OK;
}

_int	CTutorialStage_EvnLayer::Update_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	CTutorialStage_EvnLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	CTutorialStage_EvnLayer::Render_Layer()
{

}

CTutorialStage_EvnLayer* CTutorialStage_EvnLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTutorialStage_EvnLayer* pPlayer = new CTutorialStage_EvnLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void CTutorialStage_EvnLayer::Free()
{
    CLayer::Free();
}
