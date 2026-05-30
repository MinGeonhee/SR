#pragma once
#include "CUI.h"
#include "CProtoTypeManager.h"

class CGameOver :
    public CUI
{
private:
    explicit CGameOver(const CGameOver& rhs);
    explicit CGameOver(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CGameOver();

    virtual void            Free();

public:
    static CGameOver* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    void Set_Vanish(bool bVanish) { m_bVanish = bVanish; }
    bool Get_Vanish() const { return m_bVanish; }

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Set_Toggle(const _float& fTimeDelta);
    void Set_Time();
private:
    bool m_bMove = false;
    bool m_bVanish = false;
    bool m_bDown = false;
    bool m_playedDeathSfx = false;

    bool m_bRestartUI = false;
    int m_hoverIndex = -1;
};

