#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class ENGINE_DLL CBuff :
    public CGameObject
{
protected:
    explicit    CBuff(const CBuff& rhs);
    explicit    CBuff(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CBuff();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta) override;
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual     void        Render_GameObject() override;

    static  CBuff* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT     Add_Component();

protected:
    virtual void    Free() override;

private:
    Engine::CAnimation* m_pAnimationCom;
    Engine::CBillboard* m_pBillboardCom;

private:
    float m_fOffset_Y = 1.f;
public:
    void Set_Offset_Y(float _f) { m_fOffset_Y = _f; }

};

END