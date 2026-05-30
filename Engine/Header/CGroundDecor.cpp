#include "CGroundDecor.h"
#include "CTransform.h"
#include "CMapObjectTex.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CTexture.h"

CGroundDecor::CGroundDecor(LPDIRECT3DDEVICE9 Device)
	: CDecor(Device) , m_pBillboardCom(nullptr)
{
}

CGroundDecor::CGroundDecor(const CGroundDecor& rhs)
	: CDecor(rhs) , m_pBillboardCom(nullptr)
{
}

CGroundDecor::~CGroundDecor()
{
}



_int  CGroundDecor::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup( RENDER_ALPHATESTING , this );

    return 0;
}

void        CGroundDecor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    else
        if( m_pBillboardCom !=  nullptr)
            m_pBillboardCom->LateUpdate_Component(fTimeDelta);
}


void        CGroundDecor::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD , m_pTransform->Get_World());
    CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Decor")->Set_Texture();
    m_pBuffer->Render_Buffer();
}


HRESULT   CGroundDecor::Ready_GameObject()
{
    if (FAILED(CGroundDecor::Add_Component()))
        return E_FAIL;



    return S_OK;
}


HRESULT    CGroundDecor::Add_Component()
{
    if (FAILED(CDecor::Add_Component()))
        return E_FAIL;


    CComponent* newCom = nullptr;

    newCom = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard" , newCom });
    m_pBillboardCom->Set_MyOwner(this);


    return S_OK;
}


CGroundDecor* CGroundDecor::Create(LPDIRECT3DDEVICE9 device)
{
    CGroundDecor* pObj = new CGroundDecor(device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CGroundDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}

void CGroundDecor::Free()
{
    CGameObject::Free();
}
