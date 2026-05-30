#include "CCube.h"
#include "CTransform.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CCollisionManager.h"
#include "CResourceManager.h"
#include "CTexture.h"

CCube::CCube(const CCube& cpy)
    : CGameObject(cpy) , m_IsEvent(true)  , m_eCubeType(CUBE_TYPE::CUBE_NONE)
{
    
    m_tHandle.m_eType = cpy.m_tHandle.m_eType;
}

CCube::CCube(LPDIRECT3DDEVICE9 device)
    : CGameObject(device) , m_IsEvent(false) ,  m_pBuffer(nullptr)
    , m_eCubeType(CUBE_TYPE::CUBE_NONE)
{
    m_tHandle.m_eType = OBJ_CUBE;
}

CCube::~CCube()
{
}


HRESULT CCube::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Tile");
    return S_OK;
}


_int CCube::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX, this);

   
    return 0;
}

void CCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if( m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CCube::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

_vec2* CCube::GetCenterUV()
{
    return m_pBuffer->GetCenterUV();
}

_bool* CCube::GetCubeHorizontalFilpArr()
{
    return m_pBuffer->GetIsFilpHorizontalArr();
}

_bool* CCube::GetCubeVerticalFilpArr()
{
    return m_pBuffer->GetIsFilpVerticalArr();
}

void CCube::SetCubeHorizontalFilpArr(_bool* arr)
{
    if (arr == nullptr)
        return;

    m_pBuffer->SetFilpHorizontalArr(arr);
}

void CCube::SetCubeVerticalFilpArr(_bool* arr)
{
    if (arr == nullptr)
        return;
    m_pBuffer->SetFilpVerticalArr(arr);
}


HRESULT CCube::Add_Component()
{

    CComponent* newCom = nullptr;
    newCom = m_pBuffer = CLONE_PROTOTYPE(PROTO_CUBETEX, CCubeTex);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer" , newCom });


    newCom = nullptr;
    newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_ATrans" , newCom });


    newCom = nullptr;
    newCom = m_pColliderCom = CLONE_PROTOTYPE( PROTO_COLLIDER_AAABBB , CAAABBB);
    m_mapComponent->insert({ L"Com_BCollider" , newCom });
    
    m_pColliderCom->Set_MyOwner(this);

	return S_OK;

}


void CCube::Change_CubeTextureFaceUV(const CUBE_FACE& eType, const _vec2& vCenterUV)
{
    m_pBuffer->ChangeTextureCubeFaceUV(eType,vCenterUV);
}

void CCube::Filp_Horizontal(const CUBE_FACE& etype)
{
    m_pBuffer->Filp_Horizontal(etype);
}

void CCube::Filp_Vertically(const CUBE_FACE& etype)
{
    m_pBuffer->Filp_Vertically(etype);
}

void CCube::SemiUpdate()
{
    m_pTransform->Update_Component(0.f);
    m_pColliderCom->LateUpdate_Component(0.f);
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 device)
{
    CCube* pCube = new CCube(device);

    if (FAILED(pCube->Ready_GameObject()))
    {        
        MSG_BOX("CTile Create Is Failed");
        Safe_Release(pCube);
        return nullptr;
    }

    return pCube;
}

void CCube::Free()
{
    CGameObject::Free();
}
