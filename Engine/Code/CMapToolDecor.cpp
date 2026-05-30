#include "CMapToolDecor.h"
#include "CRendererManager.h"
#include "CMapObjectTex.h"
#include "CTransform.h"
#include "CResourceManager.h"

CMapToolDecor::CMapToolDecor(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMapToolMapObject(pGraphicDev)
{
    //맵툴용이기때문에 일단 하나의 타입으로 둔다
    m_tHandle.m_eType = OBJTYPE::OBJ_MAPDECOR;

    m_eCategory = MAPOBJECT_CATEGORY::DECOR;
    m_ePlacement = MAPOBJECT_PLACEMENT::_END;
}

CMapToolDecor::CMapToolDecor(const CMapToolDecor& rhs)
    : CMapToolMapObject(rhs)
{
}

CMapToolDecor::~CMapToolDecor()
{
}

_int CMapToolDecor::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);

    return 0;
}

void CMapToolDecor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapToolDecor::Render_GameObject()
{
    if (m_ePlacement == MAPOBJECT_PLACEMENT::GROUND)
        CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Decor")->Set_Texture();
    else if (m_ePlacement == MAPOBJECT_PLACEMENT::WALL)
        CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Decor")->Set_Texture();


    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}



HRESULT CMapToolDecor::Ready_GameObject()
{
    if (FAILED(CMapToolDecor::Add_Component()))
        return E_FAIL;

    return S_OK;
}


HRESULT CMapToolDecor::Add_Component()
{
    //트랜스폼 / 버퍼 생성 /콜라이더까즤
    if (FAILED(CMapToolMapObject::Add_Component()))
        return E_FAIL;

    return S_OK;
}


CMapToolDecor* CMapToolDecor::Create(LPDIRECT3DDEVICE9 Device)
{
    CMapToolDecor* pObj = new CMapToolDecor(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CMapToolDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}

void        CMapToolDecor::Free()
{
    CGameObject::Free();
}
