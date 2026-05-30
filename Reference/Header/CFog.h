#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class CBillboard;

class ENGINE_DLL CFog :
    public CGameObject
{
protected:
    //explicit    CFog();
    explicit    CFog(const CFog& rhs);
    explicit    CFog(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CFog();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

protected:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT     Add_Component();

protected:
    Engine::CBillboard* m_pBillboardCom;
    Engine::CRecTex* m_pBufferCom;

public:
    static  CFog* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
    virtual void    Free();
public:
    void Make_Random_Position(_vec3 _vRange, _vec3 _vPos);
};

END

