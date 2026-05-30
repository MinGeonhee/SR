#include "pch.h"
#include "CStage2_EnvLayer.h"
#include "CCameraManager.h"
#include "CTerrain.h"

CStage2_EnvLayer::CStage2_EnvLayer()
{

}

HRESULT CStage2_EnvLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
{
    //Engine::CGameObject* pGameObject = nullptr;
    //pGameObject = CTerrain::Create(pGraphicDev);
    //
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //
    //if (FAILED(Add_GameObject(L"Terrain", pGameObject)))
    //    return E_FAIL;

    return S_OK;
}

_int	CStage2_EnvLayer::Update_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	CStage2_EnvLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	CStage2_EnvLayer::Render_Layer()
{

}

CStage2_EnvLayer* CStage2_EnvLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage2_EnvLayer* pPlayer = new CStage2_EnvLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void CStage2_EnvLayer::Free()
{
    CLayer::Free();
}