#pragma once
#include "CGameObject.h"
#include "CPrototypeManager.h"

class CSkyBox :
    public CGameObject
{
private:
    explicit   CSkyBox(const CSkyBox& rhs);
    explicit   CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CSkyBox();

    virtual void  Free();

public:
    virtual     HRESULT     Ready_GameObject() override;
    virtual     _int        Update_GameObject(const _float& fTimeDelta)override;
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta)override;
    virtual     void        Render_GameObject()override;

private:
    HRESULT                 Add_Component();

public:
    static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

    Engine::CSkyCube*       m_pBufferCom = nullptr;
    Engine::CWallet*        m_pWalletCom = nullptr;
    Engine::CTransform*     m_pPlayerTransform = nullptr; // 한번만 캐싱해두면 국밥마냥 우려먹음
};

