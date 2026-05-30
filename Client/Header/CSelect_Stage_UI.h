#pragma once
#include "CUI.h"

class CSelect_Stage_UI :
    public CUI
{
    explicit CSelect_Stage_UI(const CSelect_Stage_UI& rhs);
    explicit CSelect_Stage_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSelect_Stage_UI();

    virtual void            Free();

public:
    static CSelect_Stage_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    void Set_StNum(int Num) { m_iStageNum = Num; }
protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Render_ChildTex();
    void Select_Stage();
    void Key_Input();

    void          PullStageResult();

private:
    bool m_bMove = false;

    int m_hoverIndex = -1;
    int m_iStageNum = 0;

    float m_parTime = 0.f;
    float m_bestTime = 0.f;
    int   m_BestScore = 0;
};

