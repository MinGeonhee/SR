#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CPlane :
    public CGameObject
{
private:

    explicit CPlane(const CPlane& rhs);
    explicit CPlane(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CPlane();

public:

    static CPlane* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
                void        Key_Input(const _float& fTimeDelta);

private:

    Engine::CUIRecTex* m_pBufferCom;       // 삼각형 버퍼 컴포넌트

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT     Add_Component();

private:
    virtual void            Free() override;
};

