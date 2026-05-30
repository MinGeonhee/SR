#include "CWallDecor.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CTexture.h"

CWallDecor::CWallDecor(LPDIRECT3DDEVICE9 pGraphicDev)
    : CDecor(pGraphicDev)
{
    m_ePlacement = MAPOBJECT_PLACEMENT::WALL;
}

CWallDecor::CWallDecor(const CWallDecor& rhs)
    : CDecor(rhs)
{
    m_ePlacement = MAPOBJECT_PLACEMENT::WALL;
}

CWallDecor::~CWallDecor()
{
}



_int CWallDecor::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);


    CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);

    _vec3 vPos = *this->Get_Transform()->GetInfoPos();

    CGameObject::Compute_ViewZ(&vPos);

    return 0;
}


void  CWallDecor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);
}


void    CWallDecor::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Decor")->Set_Texture();
    m_pBuffer->Render_Buffer();
}



HRESULT CWallDecor::Ready_GameObject()
{
    if (FAILED(CWallDecor::Add_Component()))
        return E_FAIL;

    return S_OK;
}




HRESULT CWallDecor::Add_Component()
{
    //트랜스폼 / 버퍼 생성
    if (FAILED(CMapObject::Add_Component()))
        return E_FAIL;
}


CWallDecor* CWallDecor::Create(LPDIRECT3DDEVICE9 Device)
{
    CWallDecor* pObj = new CWallDecor(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CWallDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}


void CWallDecor::Free()
{
    CGameObject::Free();
}