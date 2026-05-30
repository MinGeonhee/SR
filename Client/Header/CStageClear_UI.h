#pragma once
#include "CUI.h"

class CUI_Image;
class CStageClear_UI :
    public CUI
{
private:
    explicit CStageClear_UI(const CStageClear_UI& rhs);
    explicit CStageClear_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CStageClear_UI();

public:
    static CStageClear_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Render_Text();
    void Set_Ani();
    void Set_Toggle();

    void          PullStageResult();
    static void FormatTimeMMSSms(float t, int& mm, int& ss, int& ms);
private:
    void Free() override;
    vector<CUI*> m_vecUIObjects;

    int m_iStageNum = 0;
    bool PrintOn = false;
    bool bActive = true;

    ID3DXSprite* m_pTextSprite = nullptr;

    float        m_revealTimer = 0.f;
    float        m_overallTime = -1.f;
    int          m_revealedCount = 0; //나온거 몇개
    float        m_lineTime[12] = { -1.f }; // -1이면 아직 안 보이게

    CUI_Image* m_lineIcons[3] = { nullptr, nullptr, nullptr };
    _vec3      m_iconBaseScale[3] = { {0.08f,0.13f,1.f}, {0.08f,0.13f,1.f}, {0.08f,0.13f,1.f} };

    const float  interval = 0.4f; //다음줄가는시간
    const float  pause_time = 0.25f; //지속시간

    bool  m_bPulled = false;
    bool  m_prevActive = false;
    float m_clearTime = 0.f;
    float m_parTime = 0.f;
    float m_bestTime = 0.f;
    int   m_treasure = 0;
    int   m_LevelScore = 0;
    int   m_BestScore = 0;
    int   m_totalScore = 0;
    int iKillCount = 0;
};

