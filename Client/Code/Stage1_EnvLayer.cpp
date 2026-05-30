#include "pch.h"
#include "Stage1_EnvLayer.h"
#include "CCameraManager.h"
#include "CTerrain.h"

Stage1_EnvLayer::Stage1_EnvLayer()
{
   
}

HRESULT Stage1_EnvLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
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

_int	Stage1_EnvLayer::Update_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);
    CLayer::Update_Layer(fTimeDelta);

    return 0;
}

void	Stage1_EnvLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
    CLayer::LateUpdate_Layer(fTimeDelta);
}

void	Stage1_EnvLayer::Render_Layer()
{
    
}

Stage1_EnvLayer* Stage1_EnvLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Stage1_EnvLayer* pPlayer = new Stage1_EnvLayer();

    if (FAILED(pPlayer->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pPlayer);
        return nullptr;
    }

    return pPlayer;
}

void Stage1_EnvLayer::Free()
{
    CLayer::Free();
}