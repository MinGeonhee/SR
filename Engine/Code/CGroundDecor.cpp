#include "CGroundDecor.h"
#include "CMapObjectTex.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"



CGroundDecor::CGroundDecor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDecor(pGraphicDev) , m_pBillboardCom(nullptr)
{
    m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CGroundDecor::CGroundDecor(const CGroundDecor& rhs)
	: CDecor(rhs)	 , m_pBillboardCom(nullptr)
{
    m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CGroundDecor::~CGroundDecor()
{
}



_int CGroundDecor::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::Update_GameObject(fTimeDelta);
    }

    else
        m_pBillboardCom->Update_Component(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_MAPOBJECT_GROUND_DECOR, this);
}


void  CGroundDecor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }

    else
        m_pBillboardCom->LateUpdate_Component();
}


void    CGroundDecor::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}



HRESULT     CGroundDecor::Ready_GameObject()
{
    if (FAILED(CGroundDecor::Add_Compoenet()))
        return E_FAIL;

    return S_OK;
}



CGroundDecor* CGroundDecor::Create(LPDIRECT3DDEVICE9 Device)
{
    CGroundDecor* pObj = new CGroundDecor(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CGroundDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}


HRESULT CGroundDecor::Add_Component()
{
    //트랜스폼 / 버퍼 생성
    if (FAILED(CMapObject::Add_Compoenet()))
        return E_FAIL;


    // 빌보드 컴포넌트 생성   
    CComponent* newCom = nullptr;

    newCom = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard" , newCom });

}



void    CGroundDecor::Free()
{
    CGameObject::Free();
}