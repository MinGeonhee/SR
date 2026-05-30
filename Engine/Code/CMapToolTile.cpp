#include "CMapToolTile.h"
#include "CTransform.h"
#include "CProtoTypeManager.h"
#include "CCollisionManager.h"
#include "CRendererManager.h"
#include "CTileManager.h"

CMapToolTile::CMapToolTile(const CMapToolTile& cpy)
    : CGameObject(cpy), m_IsEvent(false), m_tMapToolTile_Info(cpy.m_tMapToolTile_Info)
    , m_pBuffer(nullptr), m_eTileType(cpy.m_eTileType) 
{
    m_tHandle.m_eType = cpy.m_tHandle.m_eType;
    m_tHandle.m_iIndex = -1;
}

CMapToolTile::CMapToolTile(LPDIRECT3DDEVICE9 device)
    : CGameObject(device), m_pBuffer(nullptr), m_eTileType(TILETYPE::TILE_NONE), m_IsEvent(false)
{
    m_tHandle.m_eType = OBJ_TILE;
    m_tHandle.m_iIndex = -1;

    ZeroMemory(&m_tMapToolTile_Info, sizeof(m_tMapToolTile_Info));
}

CMapToolTile::~CMapToolTile()
{
}

HRESULT CMapToolTile::Ready_GameObject(const  FLOOR& eFloor, const _vec2& vCenterUV, const _ushort& sRoomNum, const _vec3& vPos)
{

    if (FAILED(Add_Component()))
        return E_FAIL;

    if (FAILED(Set_TilePos(vPos)))
        return E_FAIL;

    if (FAILED(Set_TileUV(vCenterUV)))
        return E_FAIL;

    m_tHandle.m_eFloor = eFloor;
    m_tHandle.m_sRoomNum = sRoomNum;


    m_pTransform->Set_Scale(CTileManager::GetInstance()->GetTileScale());
    m_pColliderCom->Set_ColliderScale(m_pTransform->Get_Scale());


    SemiUpdate();

    return S_OK;
}

HRESULT CMapToolTile::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;


    m_pTransform->Set_Scale(CTileManager::GetInstance()->GetTileScale());

    return S_OK;
}

_int CMapToolTile::Update_GameObject(const _float& fTimeDelta)
{

    if (m_IsEvent == true) // 어떤 이벤트가 발생했을때만 돌려준다 이미 월드좌표 Pos는 업데이트가 된 상태.
    {
        CGameObject::Update_GameObject(fTimeDelta);
    }

    return 0;
}

void CMapToolTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
        //한번 업데이트하고 나면 일단 true로 바꾼다
        m_IsEvent = false;
    }
}

void CMapToolTile::Render_GameObject()
{
    if (m_eTileType == TILE_NONE)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

HRESULT CMapToolTile::Add_Component()
{
    CComponent* newCom = m_pBuffer = CLONE_PROTOTYPE(PROTO_TILETEX, CTileTex);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", newCom });


    newCom = nullptr;
    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AABB, CAABB);

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_ZCol", newCom });



    newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", newCom });


    m_pColliderCom->Set_MyOwner(this);

    return S_OK;
}

HRESULT CMapToolTile::Set_TileUV(const _vec2& vCenterUV)
{
    if (m_pBuffer == nullptr)
        return E_FAIL;

    if (m_pBuffer->Change_CenterUV(vCenterUV))
        return E_FAIL;

    return S_OK;
}

HRESULT CMapToolTile::Set_TilePos(const _vec3& vPos)
{
    if (m_pTransform == nullptr)
        return E_FAIL;


    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);


    return S_OK;
}

void CMapToolTile::Change_TextureUV(const _vec2& vCenterUV)
{
    static_cast<CTileTex*>(m_pBuffer)->Change_CenterUV(vCenterUV);
}

void CMapToolTile::SemiUpdate()
{
    m_pTransform->Update_Component(0.f);
    m_pColliderCom->LateUpdate_Component(0.f);
}


//UV 좌표를 찍을 UV중점 좌표, Tile의 Pos값
CMapToolTile* CMapToolTile::Create(LPDIRECT3DDEVICE9 device, const  FLOOR& eFloor, const _ushort& sRoomNum, const _vec3& vPos, const _vec2& vCenterUV)
{
    CMapToolTile* pTile = new CMapToolTile(device);

    if (FAILED(pTile->Ready_GameObject(eFloor, vCenterUV, sRoomNum, vPos)))
    {
        MSG_BOX("CMapToolTile Create Is Failed");
        Safe_Release(pTile);
        return nullptr;
    }

    return pTile;
}

CMapToolTile* CMapToolTile::Create(LPDIRECT3DDEVICE9 device)
{
    CMapToolTile* pTile = new CMapToolTile(device);

    if (FAILED(pTile->Ready_GameObject()))
    {
        MSG_BOX("CMapToolTile Create Is Failed");
        Safe_Release(pTile);
        return nullptr;
    }

    return pTile;
}

void CMapToolTile::Free()
{
    CGameObject::Free();
}




