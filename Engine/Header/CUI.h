#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class ENGINE_DLL CUI :
    public CGameObject
{
protected:
    explicit CUI(const CUI& rhs);
    explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUI();

    wstring m_strTag;
public:
    static CUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    void Set_Tag(const wstring& tag) { m_strTag = tag; }
    const wstring& Get_Tag() const { return m_strTag; }
    static void FormatTimeStr(float t, wchar_t* out, size_t cap);

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Set_UIActive(bool bActive) { m_bActive = bActive; }
    bool Is_UIActive() const { return m_bActive; }
    void Set_Finish() { m_bFinish = true; }
    bool Is_Finish() const { return m_bFinish; }

    static bool IsPaused() { return s_bPaused; }
    static void SetPaused(bool b) { s_bPaused = b; }
protected:
    HRESULT     Add_Component();

    void DrawShadowedText(
        ID3DXFont* font,
        const wchar_t* text,
        const RECT& rc,
        DWORD colorMain,
        int shadowOfs = 2,
        DWORD colorShadow = D3DCOLOR_ARGB(160, 0, 0, 0),
        DWORD format = DT_CENTER | DT_VCENTER | DT_SINGLELINE);

protected:
    Engine::CUITileTex* m_pBufferCom;
    Engine::CAnimation* m_pAniCom;

protected:
    virtual void            Free() override;

    LPD3DXFONT m_big = nullptr;
    LPD3DXFONT m_mid = nullptr;
    LPD3DXFONT m_small = nullptr;
    LPD3DXFONT m_mini = nullptr;

    bool m_bActive = false;
    bool m_bFinish = false;
    _vec3 m_vOriginalPos = { 0.f, 0.f, 0.f };

    float m_fTimer = 0.f;
    static bool s_bPaused;

};

END