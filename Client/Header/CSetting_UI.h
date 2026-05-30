#pragma once
#include "CUI.h"

class CUI_Number;

class CSetting_UI :
    public CUI
{
private:
    explicit CSetting_UI(const CSetting_UI& rhs);
    explicit CSetting_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSetting_UI();

public:
    static CSetting_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    void Render_Text();
    void Key_Input();
    void Render_ChildTex();

    void Render_AudioSlider();
    void Update_AudioSlider_Input();

private:
    void Free() override;
    vector<CUI*> m_vecUIObjects;
    ID3DXLine* m_pLine = nullptr;

    bool m_prevActive = false;
    int m_hoverIndex = 0;
    int m_CategoryIndex = 2;
    int m_SelectedIndex = 0;

    int kCategorySize[4] = { 3, 4, 4, 2 };

    list<int> m_checkedGameplay;
    list<int> m_checkedVideo;
    list<int> m_checkedAudio;
    list<int> m_checkedInput;

    array<int, 4>   m_audioVals = { 100, 100, 100, 100 }; // Master, Sound, Music, Ambient
    array<RECT, 4>  m_sliderRects = {};
    bool                m_dragging = false;
    int                 m_dragIndex = -1; // 드래그 중인 슬라이더 인덱스 (0~3)

};

