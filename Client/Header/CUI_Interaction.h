#pragma once
#include "CUI.h"
class CUI_Interaction :
    public CUI
{
private:
    explicit CUI_Interaction(const CUI_Interaction& rhs);
    explicit CUI_Interaction(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUI_Interaction();

public:
    static CUI_Interaction* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Set_Scale(const _vec3& vScale) { if (m_pTransform) m_pTransform->Set_Scale(vScale); }
    void Set_Position(float x, float y, float z) { if (m_pTransform) m_pTransform->Set_Pos(x, y, z); }
    void Set_Output(const wstring& strOutput) { m_strOutput = strOutput; }
    void DialogueFromTag(const wstring& tag);

private:
    Engine::CUIRecTex* m_pBufferCom;
    wstring m_strOutput;
    wstring m_cachedTag;

private:
    void Free() override;
};

