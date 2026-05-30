#pragma once
#include "CUI.h"

class CDialogue_UI :
    public CUI
{
private:
    explicit CDialogue_UI(const CDialogue_UI& rhs);
    explicit CDialogue_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CDialogue_UI();

    virtual void            Free();

public:
    static CDialogue_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    // 캐릭터, 대사
    struct Line {
        wstring speaker;
        wstring text;
    };

    void Start(const wstring& tag, bool typewriter = true, float cps = 48.f);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    void DialogueFromTag(const wstring& tag, vector<Line>& out);
    void Key_Input(float fTimeDelta);
    void Typewriter(float fTimeDelta);
    void DrawTexts();

private:
    RECT m_rcBase{}, m_rcName{}, m_rcText{};
    vector<Line> m_lines;
    wstring m_cachedTag; //지금 적용된 태그.

    bool  m_useTypewriter = false;//한글자씩효과쓸건지

    int   m_charVisible = 0;//이번줄에서 보여주는중인 글자수
    int   m_cur = 0;//몇번째줄? 
    float m_cps = 15.f;//글자 속도
    float m_acc = 0.f;//누적값, 1넘으면 글자 1개 출력함
    float m_blink = 0.f;//화살표깜빡이는거
};

