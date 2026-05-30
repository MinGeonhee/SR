#pragma once
#include "CUI.h"
class CUI_NoticeBanner :
    public CUI
{
private:
    explicit CUI_NoticeBanner(const CUI_NoticeBanner& rhs);
    explicit CUI_NoticeBanner(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUI_NoticeBanner();

    virtual void            Free();

public:
    static CUI_NoticeBanner* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    void DialogueFromTag(const wstring& tag);

    void Set_Key(const wstring& key) { m_strText = key; }
    void SetFlashDuration(float sec) { m_flashDuration = sec; }
    void TriggerFlash(float sec = -1.f) {
        m_bActive = true;
        m_visibleTimer = (sec > 0.f ? sec : m_flashDuration);
    }

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    wstring m_strText;
    wstring m_cachedTag;

    float m_visibleTimer = 0.f;
    float m_flashDuration = 3.f;
    bool  m_prevActive = false;
};

