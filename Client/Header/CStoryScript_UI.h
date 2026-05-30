#pragma once
#include "CUI.h"
#include "CProtoTypeManager.h"

class CStoryScript_UI :
    public CUI
{
private:
    explicit CStoryScript_UI(const CStoryScript_UI& rhs);
    explicit CStoryScript_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CStoryScript_UI();

    virtual void            Free();

public:
    static CStoryScript_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();
    void ApplyTag(const wstring& tag);
private:
    bool m_bMove = true;
    float  m_scrollOfs = 0.f;
    float  m_scrollSpeed = 40.f;
    bool  m_prevActive = false;
    wstring m_cachedTag; //지금 적용된 태그.
    bool m_bTextUp = false;
	static const wchar_t* PrologueTexts[];
    static const wchar_t* EpilogueTexts[];
	static const int PrologueTextCount;
    static const int EpilogueTextCount;

    const wchar_t** m_currTexts = PrologueTexts;
    int             m_currCount = PrologueTextCount;
    const wchar_t* m_currTitle = L"~ Prologue ~";
    const wchar_t* m_currBgTex = L"Prologue";
};

