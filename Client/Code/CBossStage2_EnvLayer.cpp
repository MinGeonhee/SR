#include "pch.h"
#include "CBossStage2_EnvLayer.h"
#include "CCameraManager.h"
#include "CTerrain.h"

CBossStage2_EnvLayer::CBossStage2_EnvLayer()
{

}

HRESULT CBossStage2_EnvLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
{
    return S_OK;
}

_int	CBossStage2_EnvLayer::Update_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	CBossStage2_EnvLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	CBossStage2_EnvLayer::Render_Layer()
{

}

CBossStage2_EnvLayer* CBossStage2_EnvLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossStage2_EnvLayer* pPlayer = new CBossStage2_EnvLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void CBossStage2_EnvLayer::Free()
{
    CLayer::Free();
}