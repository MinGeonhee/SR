#pragma once
#include "CUI.h"

class CUI_Number;
class CPlayer;

class CPauseUI :
    public CUI
{
private:
    explicit CPauseUI(const CPauseUI& rhs);
    explicit CPauseUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CPauseUI();

public:
    static CPauseUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void        Set_Toggle();

private:
    void Free() override;
    ID3DXLine* m_pLine= nullptr;
    bool bActive = false;

    int m_hoverIndex = -1;
};

