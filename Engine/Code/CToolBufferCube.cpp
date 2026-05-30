#include "CToolBufferCube.h"
#include "CTransform.h"
#include "CProtoTypeManager.h"
#include "CCubeManager.h"
#include "CRendererManager.h"

CToolBufferCube::CToolBufferCube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev) , m_pBufferCom(nullptr)
{
}

CToolBufferCube::CToolBufferCube(const CToolBufferCube& rhs)
    : CGameObject(rhs) 
{
}

CToolBufferCube::~CToolBufferCube()
{
}

HRESULT CToolBufferCube::Ready_GameObject(const _vec3& vPos)
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
    m_pTransform->Set_Scale(CCubeManager::GetInstance()->GetCubeScale());

    return S_OK;
}

_int CToolBufferCube::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHABLEND, this);

    return 0;
}

void CToolBufferCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CToolBufferCube::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CToolBufferCube::ChangeColor(_vec4 Color)
{
    m_pBufferCom->Change_Color(Color);
}

CToolBufferCube* CToolBufferCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
    CToolBufferCube* pObj = new CToolBufferCube(pGraphicDev);

    if (FAILED(pObj->Ready_GameObject(vPos)))
    {
        MSG_BOX("ToolBufferRect Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}

HRESULT CToolBufferCube::Add_Component()
{
    CComponent* newCom = nullptr;

    newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
    if (newCom == nullptr)
        return E_FAIL;
    m_mapComponent->insert({ L"Com_Transform", newCom });

    newCom = m_pBufferCom = CLONE_PROTOTYPE(PROTO_CUBECOL, CCubeCol);
    if (newCom == nullptr)
        return E_FAIL;
    m_mapComponent->insert({ L"Com_Buffer", newCom });

    return S_OK;
}

void CToolBufferCube::Free()
{
    CGameObject::Free();
}
