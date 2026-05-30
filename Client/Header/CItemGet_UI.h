#pragma once
#include "CUI.h"
class CItemGet_UI :
    public CUI
{
private:
    explicit CItemGet_UI(const CItemGet_UI& rhs);
    explicit CItemGet_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CItemGet_UI();

    virtual void            Free();

public:
    struct BannerSlot {
        wstring text;
        int uvx = 0, uvy = 0;
        float t = 0.f;
    };

    static CItemGet_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    void BuildBannerFromTag(const wstring& tag, BannerSlot& out);

    void SetFlashDuration(float sec) { m_flashDuration = sec; }

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    int m_iTexUVX = 0;
    int m_iTexUVY = 0;
    float m_visibleTimer = 0.f;
    float m_flashDuration = 5.f;
    bool  m_prevActive = false; // 직전 프레임 active 상태

    deque<BannerSlot> m_slots;

    inline static const unordered_map<wstring, pair<int, int>> kIconMap = {
    {L"HEALTH SHARD", {4,0}}, {L"ARMOR SHARD", {5,2}}, {L"BERRIES", {6,0}}, {L"GREATER POTION", {1,0}},
    {L"BRONZE COIN", {0,1}}, {L"SILVER COIN", {1,1}}, {L"GOLD COIN", {2,1}}, {L"COIN PURSE", {2,1}}, {L"GOLD CHALICE", {2,1}},
    {L"LOW MANA", {7,2}}, {L"FIRE SHARD", {2,0}}, {L"ICE SHARD", {5,0}}, {L"GUN BULLET", {1,5}},
    {L"BRONZE KEY", {0,2}}, {L"SILVER KEY",{1,2}}, {L"GOLD KEY", {2,2}}, {L"TREASURE KEY",{3,2}},
    };

};

