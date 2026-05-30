#include "CMapToolObstacle.h"
#include "CRendererManager.h"
#include "CMapObjectTex.h"
#include "CTransform.h"
#include "CResourceManager.h"

CMapToolObstacle::CMapToolObstacle(LPDIRECT3DDEVICE9 pGraphicDev)
    :  CMapToolMapObject(pGraphicDev)
{
    //맵툴용임으로 하나의 타입으로 통일
    m_tHandle.m_eType = OBJ_MAPOBSTACLE;


    m_eCategory = MAPOBJECT_CATEGORY::OBSTACLE;
    m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CMapToolObstacle::CMapToolObstacle(const CMapToolObstacle& rhs)
    : CMapToolMapObject(rhs)
{
}

CMapToolObstacle::~CMapToolObstacle()
{
}

_int CMapToolObstacle::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::Update_GameObject(fTimeDelta);
    }

    return 0;
}


void  CMapToolObstacle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);
}


void    CMapToolObstacle::Render_GameObject()
{
    CResourceManager::GetInstance()->Get<CTexture>(L"Obstacle")->Set_Texture();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}



HRESULT     CMapToolObstacle::Ready_GameObject()
{
    if (FAILED(CMapToolObstacle::Add_Component()))
        return E_FAIL;

    return S_OK;
}



CMapToolObstacle* CMapToolObstacle::Create(LPDIRECT3DDEVICE9 Device)
{
    CMapToolObstacle* pObj = new CMapToolObstacle(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CMapToolObstacle Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}


HRESULT CMapToolObstacle::Add_Component()
{
    //트랜스폼 , 버퍼 , 콜라이더
    if (FAILED(CMapToolMapObject::Add_Component()))
        return E_FAIL;




    return S_OK;
}



void    CMapToolObstacle::Free()
{
    CGameObject::Free();
}
