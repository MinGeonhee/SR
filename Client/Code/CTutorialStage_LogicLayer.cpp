#include "pch.h"
#include "CTutorialStage_LogicLayer.h"
#include "CCameraManager.h"

CTutorialStage_LogicLayer::CTutorialStage_LogicLayer()
{
}
CTutorialStage_LogicLayer::~CTutorialStage_LogicLayer()
{

}

HRESULT CTutorialStage_LogicLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
{

    return S_OK;
}

_int	CTutorialStage_LogicLayer::Update_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	CTutorialStage_LogicLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	CTutorialStage_LogicLayer::Render_Layer()
{

}

CTutorialStage_LogicLayer* CTutorialStage_LogicLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTutorialStage_LogicLayer* pPlayer = new CTutorialStage_LogicLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void CTutorialStage_LogicLayer::Free()
{
    CLayer::Free();
}