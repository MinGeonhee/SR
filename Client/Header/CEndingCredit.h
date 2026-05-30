#pragma once
#include "CUI.h"
class CEndingCredit :
    public CUI
{
private:
    explicit CEndingCredit(const CEndingCredit& rhs);
    explicit CEndingCredit(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CEndingCredit();

    virtual void            Free();

public:
    static CEndingCredit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    bool m_bMove = true;
    float  m_scrollOfs = 0.f;
    float  m_scrollSpeed = 40.f;
    bool  m_prevActive = false;
    bool m_bTextUp = false;

    static const wchar_t* Texts[];
    static const int TextCount;
};

