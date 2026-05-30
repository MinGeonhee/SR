#include "pch.h"
#include "CBossStage_EnvLayer.h"
#include "CCameraManager.h"
#include "CTerrain.h"

CBossStage_EnvLayer::CBossStage_EnvLayer()
{
   
}

HRESULT CBossStage_EnvLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
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

_int	CBossStage_EnvLayer::Update_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	CBossStage_EnvLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	CBossStage_EnvLayer::Render_Layer()
{
    
}

CBossStage_EnvLayer* CBossStage_EnvLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossStage_EnvLayer* pPlayer = new CBossStage_EnvLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void CBossStage_EnvLayer::Free()
{
    CLayer::Free();
}