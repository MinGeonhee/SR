#pragma once
#include "CUI.h"

class CLoading;
class CLoadingBar_UI :
    public CUI
{
private:
    explicit CLoadingBar_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CLoadingBar_UI(const CLoadingBar_UI& rhs);
    virtual ~CLoadingBar_UI();

public:
    static CLoadingBar_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int    Update_GameObject(const _float& fTimeDelta) override;
    virtual void    LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual void    Render_GameObject() override;
    virtual void    Free() override;

    void SetLoading(CLoading* p) { m_pLoading = p; }

private:
    float  m_fProgress = 0.f;
	CLoading* m_pLoading = nullptr;

};
