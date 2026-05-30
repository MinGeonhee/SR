#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CBoundingBox :
    public CGameObject
{
private:
    explicit    CBoundingBox(const CBoundingBox& rhs);
    explicit    CBoundingBox(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CBoundingBox();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();
  
public:
    static CBoundingBox*    Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CTransform*     Get_Transform() { return m_pTransformCom; }

private:
    Engine::CCubeCol*       m_pBufferCom;
    Engine::CTransform*     m_pTransformCom;        // 트랜스폼 컴포넌트

private:
    virtual void  Free();
};

