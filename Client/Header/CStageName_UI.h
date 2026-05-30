#pragma once
#include "CUI.h"
class CStageName_UI :
    public CUI
{
private:
    explicit CStageName_UI(const CStageName_UI& rhs);
    explicit CStageName_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CStageName_UI();

    virtual void            Free();

public:
    static CStageName_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    void Set_TextureKey(const wstring& key) { m_strTextKey = key; }
    void SetFlashDuration(float sec) { m_flashDuration = sec; }          // 기본 1초 변경용
    void TriggerFlash(float sec = -1.f) {                                 // 강제로 1회 표시
        m_bActive = true;
        m_visibleTimer = (sec > 0.f ? sec : m_flashDuration);
    }

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();
    void UpdateText(SCENETYPE st);

private:
    wstring m_strTextKey = L"Get";
    wstring m_cachedTag;

    float m_visibleTimer = 0.f;   // 남은 표시 시간
    float m_flashDuration = 2.5f;   // 기본 1초
    bool  m_prevActive = false; // 직전 프레임 active 상태
};


