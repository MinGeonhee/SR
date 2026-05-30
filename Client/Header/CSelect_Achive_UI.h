#pragma once
#include "CUI.h"
class CSelect_Achive_UI :
    public CUI
{
    explicit CSelect_Achive_UI(const CSelect_Achive_UI& rhs);
    explicit CSelect_Achive_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSelect_Achive_UI();

    virtual void            Free();

public:
    static CSelect_Achive_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev/*, int ClearNum*/);
    void SetCheck(int idx, bool on)
    {
        auto it = find(m_AchiveNum.begin(), m_AchiveNum.end(), idx);
        if (on) {
            if (it == m_AchiveNum.end()) m_AchiveNum.push_back(idx);
        }
        else {
            if (it != m_AchiveNum.end()) m_AchiveNum.erase(it);
        }
    }

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void SetAchievement(int idx, bool on) {
        auto& v = m_AchiveNum;
        auto it = find(v.begin(), v.end(), idx);
        if (on) { if (it == v.end()) v.push_back(idx); }
        else { if (it != v.end()) v.erase(it); }
    }
    void Render_ChildTex();
    void Select_Stage();

private:
    bool m_bMove = false;

    vector<CUI*> m_vecUIObjects;
    vector<int> m_AchiveNum;
    int m_iClearNum = -1;
};

