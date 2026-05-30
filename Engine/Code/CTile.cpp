#include "CTile.h"
#include "CTransform.h"
#include "CProtoTypeManager.h"
#include "CCollisionManager.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CTileManager.h"



CTile::CTile(const CTile& cpy)
    : CGameObject(cpy) , m_IsEvent(false)
    , m_pBuffer(nullptr) ,  m_eTileType(cpy.m_eTileType) 
{
    m_tHandle.m_eType = cpy.m_tHandle.m_eType;
    m_tHandle.m_iIndex = -1;
}

CTile::CTile(LPDIRECT3DDEVICE9 device)
    : CGameObject(device)  , m_pBuffer(nullptr) , m_eTileType(TILETYPE::TILE_NONE) , m_IsEvent(false) 
{
    m_tHandle.m_eType = OBJ_TILE;
    m_tHandle.m_iIndex = -1;
}

CTile::~CTile()
{
}

HRESULT CTile::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Tile");  

    return S_OK;
}

_int CTile::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true) // 어떤 이벤트가 발생했을때만 돌려준다 이미 월드좌표 Pos는 업데이트가 된 상태.
        CGameObject::Update_GameObject(fTimeDelta);
 

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX,this);

    //_vec3 vPos;
    //m_pTransform->Get_Info(INFO_POS, &vPos);

    //CGameObject::Compute_ViewZ(&vPos);
    return 0;
}

void CTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
        //한번 업데이트하고 나면 일단 true로 바꾼다
        m_IsEvent = false;
    }
}

void CTile::Render_GameObject()
{
    if (m_eTileType == TILE_NONE)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pTextureCom->Set_Texture();
    // 이거 캐싱해두고 그걸 가져다 쓰는게 나을거 같은데
    m_pBuffer->Render_Buffer();
}

HRESULT CTile::Add_Component()
{
    CComponent* newCom = m_pBuffer = CLONE_PROTOTYPE( PROTO_TILETEX, CTileTex);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", newCom });


    newCom = nullptr;
    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AABB , CAABB );

    m_mapComponent[ID_DYNAMIC].insert({L"Com_ZCol", newCom});



    newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS,CTransform);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", newCom });


    m_pColliderCom->Set_MyOwner(this);

    return S_OK;
}

HRESULT CTile::Set_TileUV(const _vec2& vCenterUV)
{
    if (m_pBuffer == nullptr)
        return E_FAIL;

    if (m_pBuffer->Change_CenterUV(vCenterUV))
        return E_FAIL;

    return S_OK;
}

HRESULT CTile::Set_TilePos(const _vec3& vPos)
{
    if (m_pTransform == nullptr)
        return E_FAIL;

    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
    return S_OK;
}

void CTile::Set_BufferIndexCw()
{
    if (m_eTileType != TILE_CEILING)
        return;


}

void CTile::Change_IndexBuffer_To_CCW()
{
    if (this->m_eTileType != TILETYPE::TILE_CEILING)
        return;

    this->m_pBuffer->Change_IndexCCW();
}

void CTile::Change_TextureUV(const _vec2& vCenterUV)
{
    static_cast<CTileTex*>(m_pBuffer)->Change_CenterUV(vCenterUV);
}

void CTile::SemiUpdate()
{
    m_pTransform->Update_Component(0.f);
    m_pColliderCom->LateUpdate_Component(0.f);
}



CTile* CTile::Create(LPDIRECT3DDEVICE9 device)
{
    CTile* pTile = new CTile(device);

    if (FAILED(pTile->Ready_GameObject()))
    {
        MSG_BOX("CTile Create Is Failed");
        Safe_Release(pTile);
        return nullptr;
    }

    return pTile;
}

void CTile::Free()
{
    CGameObject::Free();
}
