#include "CMapToolEventObject.h"
#include "CRendererManager.h"
#include "CMapObjectTex.h"
#include "CTransform.h"
#include "CResourceManager.h"

CMapToolEventObject::CMapToolEventObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapToolMapObject(pGraphicDev) , m_iBufferInt(0)
{
    //맵툴용임으로 하나의 타입으로 통일
    m_tHandle.m_eType = OBJ_MAPEVENT;


    m_eCategory = MAPOBJECT_CATEGORY::EVENT;
    m_ePlacement = MAPOBJECT_PLACEMENT::_END;
}

CMapToolEventObject::CMapToolEventObject(const CMapToolEventObject& rhs)
	: CMapToolMapObject(rhs) , m_iBufferInt(0)
{
}

CMapToolEventObject::~CMapToolEventObject()
{
}

_int CMapToolEventObject::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::Update_GameObject(fTimeDelta);
    }

    return 0;
}


void  CMapToolEventObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
}


void    CMapToolEventObject::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());

    if( m_ePlacement == MAPOBJECT_PLACEMENT::GROUND)
        CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Event")->Set_Texture();
    else if( m_ePlacement == MAPOBJECT_PLACEMENT::WALL)
        CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Event")->Set_Texture();

    m_pBuffer->Render_Buffer();
}



HRESULT     CMapToolEventObject::Ready_GameObject()
{
    if (FAILED(CMapToolEventObject::Add_Component()))
        return E_FAIL;

    return S_OK;
}



CMapToolEventObject* CMapToolEventObject::Create(LPDIRECT3DDEVICE9 Device)
{
    CMapToolEventObject* pObj = new CMapToolEventObject(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CGroundDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}


HRESULT CMapToolEventObject::Add_Component()
{
    //트랜스폼 / 버퍼 생성
    if (FAILED(CMapToolMapObject::Add_Component()))
        return E_FAIL;

    


    return S_OK;
}



void    CMapToolEventObject::Free()
{
    CGameObject::Free();
}