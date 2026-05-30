#pragma once
#include "CUI.h"
#include "CProtoTypeManager.h"

class CSelect_UI :
    public CUI
{
private:
    explicit CSelect_UI(const CSelect_UI& rhs);
    explicit CSelect_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSelect_UI();

    virtual void            Free();

public:
    static CSelect_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void Set_Vanish(bool bVanish) { m_bVanish = bVanish; }
	bool Get_Vanish() const { return m_bVanish; }

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Key_Input();
    void Set_Ani();
    void Key_StageChange();
    void Select_Stage();
    void Set_Toggle(const _float& fTimeDelta);

private:
    Engine::CAnimation* m_pAnimationCom;
    vector<CUI*> m_vecUIObjects;
    ID3DXLine* m_pLine = nullptr;

    bool m_bMove = false;
    bool m_bVanish = false;
    bool m_bDown = false;
    int m_hoverIndex = -1;
    int m_StageIndex = -1;
    
    int m_iStageNum = 0;
};

