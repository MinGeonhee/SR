#include "pch.h"
#include "CBoundingBox.h"
#include "CRendererManager.h"
#include "CParticleMgr.h"

CBoundingBox::CBoundingBox(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CBoundingBox::CBoundingBox(const CBoundingBox& rhs)
    :CGameObject(rhs)
{
}

CBoundingBox::~CBoundingBox()
{
}


HRESULT CBoundingBox::Ready_GameObject()
{
    // 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

    return S_OK;
}

_int CBoundingBox::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

	_vec3 vScale = CParticleMgr::GetInstance()->Get_BoundingBoxScale();
    m_pTransformCom->Set_Scale(vScale);

    return 0;
}

void CBoundingBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBoundingBox::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pBufferCom->Render_Buffer();
    // 다시 켜줌.
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CBoundingBox::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_CUBECOL, CCubeCol);

    if (nullptr == pComponent)
        return E_FAIL;

    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

    // transform 컴포넌트.

    pComponent = m_pTransformCom = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

    if (pComponent == NULL)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

  

    return S_OK;
}

CBoundingBox* CBoundingBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoundingBox* pBoundingBox = new CBoundingBox(pGraphicDev);

    if (FAILED(pBoundingBox->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pBoundingBox);
        MSG_BOX("BoundingBox Create Failed");
        return nullptr;
    }

    return pBoundingBox;
}

void CBoundingBox::Free()
{
    Engine::CGameObject::Free();
}
