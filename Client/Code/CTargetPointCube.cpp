#include "pch.h"
#include "CTargetPointCube.h"

CTargetPointCube::CTargetPointCube(LPDIRECT3DDEVICE9 Device)
    : CEventCube(Device)
{
    m_pColliderCom = nullptr;
    m_pStatusCom = nullptr;
    m_pOwner = nullptr;
    m_pTarget = nullptr;
    m_pTextureCom = nullptr;
}

CTargetPointCube::CTargetPointCube(const CTargetPointCube& cpy)
    : CEventCube(cpy)
{
}

CTargetPointCube::~CTargetPointCube()
{
}

//업데이트 필요없다 임마는
_int        CTargetPointCube::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

//LateUpdate도 필요없다..
void        CTargetPointCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    return;
}

//렌더도 필요없다 애초에 랜더그룹에 넣어두지도않을거임
void        CTargetPointCube::Render_GameObject()
{

}

void CTargetPointCube::PlayEvent()
{
    int a = 0;
}




HRESULT     CTargetPointCube::Ready_GameObject()
{
    if (FAILED(CTargetPointCube::Add_Component()))
        return E_FAIL;



    return S_OK;
}

HRESULT	    CTargetPointCube::Add_Component()
{
    if (FAILED(CEventCube::Add_Component()))
        return E_FAIL;


    //트렌스폼 설정이 되고난뒤 추가적으로 설정할 컴포넌트

    return S_OK;
}



CTargetPointCube* CTargetPointCube::Create(LPDIRECT3DDEVICE9 Device)
{
    CTargetPointCube* pTargetPointCube = new CTargetPointCube(Device);

    if (FAILED(pTargetPointCube->Ready_GameObject()))
    {
        Safe_Release(pTargetPointCube);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pTargetPointCube;
}


