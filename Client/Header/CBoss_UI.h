#pragma once
#include "CUI.h"

class CBoss_UI :
    public CUI
{
private:
    explicit CBoss_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBoss_UI(const CBoss_UI& rhs);
    virtual ~CBoss_UI();

public:
    static CBoss_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int    Update_GameObject(const _float& fTimeDelta) override;
    virtual void    LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual void    Render_GameObject() override;
    virtual void    Free() override;

private:
	float m_fBossHP = 0.f;
    float m_fMaxBossHP = 0.f;
};
