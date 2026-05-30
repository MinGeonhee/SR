#include "CMapToolCube.h"
#include "CCubeManager.h"
#include "CTransform.h"
#include "CRendererManager.h"
#include "CColliderBase.h"
#include "CMapToolMapObject.h"



CMapToolCube::CMapToolCube(LPDIRECT3DDEVICE9 device)
    : CCube(device)  , m_iTargetMakeIndex(-1)
{
    m_eCubeType = CUBE_TYPE::CUBE_MAPTOOL;
}

CMapToolCube::CMapToolCube(const CMapToolCube& cpy)
    : CCube(cpy)  , m_iTargetMakeIndex(cpy.m_iTargetMakeIndex)
{
    m_eCubeType = cpy.m_eCubeType;
}



HRESULT CMapToolCube::Ready_GameObject(const FLOOR& eFloor, const _ushort& sRoomNum, const _vec3& vPos)
{
    if (FAILED(Add_Component()))
        return E_FAIL;
    
    m_tHandle.m_eFloor = eFloor;
    m_tHandle.m_sRoomNum = sRoomNum;
    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

    m_pTransform->Set_Scale(CCubeManager::GetInstance()->GetCubeScale());
    m_pColliderCom->Set_ColliderScale(CCubeManager::GetInstance()->GetCubeScale());

    // 콜라이더 y의 값을 y스케일값의 반만큼 올려준다.
    m_pColliderCom->Set_Offset(_vec3({  0.f,
        CCubeManager::GetInstance()->GetCubeScale().y  / 2 , 0.f
        }));

    CCube::SemiUpdate();

    return S_OK;
}


HRESULT CMapToolCube::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMapToolCube::Add_Component()
{
    //부모 클래스에 있는 컴포넌트 추가 함수를 이용하여 컴포넌트를 추가 해준다.
    if (FAILED(CCube::Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CMapToolCube::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == false)
    {
        return 0;
    }

    CGameObject::Update_GameObject(fTimeDelta);

    return 0;
}

void CMapToolCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == false)
        return;
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    m_IsEvent = false;
}

void CMapToolCube::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

void CMapToolCube::SetCubeTransparent()
{
    for (int i = 0 ; i < CUBE_FACE::CUBE_FACE_END; ++i)
        m_pBuffer->ChangeAllUI( _vec2 (TRANSPARENT_TILE_UV_X , TRANSPARENT_TILE_UV_Y));
}

void CMapToolCube::SetCubeDefault()
{
    for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
        m_pBuffer->ChangeAllUI(_vec2(DEFUALT_CUBE_UV_CENTERX, DEFUALT_CUBE_UV_CENTERY));
}


CMapToolCube* CMapToolCube::Create(LPDIRECT3DDEVICE9 device, const FLOOR& eFloor, const _ushort& sRoomNum, const _vec3& vPos)
{
    CMapToolCube* pMapToolCube = new CMapToolCube(device);

    if (FAILED(pMapToolCube->Ready_GameObject(eFloor, sRoomNum, vPos)))
    {
        MSG_BOX("CTile Create Is Failed");
        Safe_Release(pMapToolCube);
        return nullptr;
    }

    return pMapToolCube;
}

CMapToolCube* CMapToolCube::Create(LPDIRECT3DDEVICE9 device)
{
    CMapToolCube* pMapToolCube = new CMapToolCube(device);

    if (FAILED(pMapToolCube->Ready_GameObject()))
    {
        MSG_BOX("CTile Create Is Failed");
        Safe_Release(pMapToolCube);
        return nullptr;
    }

    return pMapToolCube;
}
