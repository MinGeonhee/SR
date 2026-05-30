#pragma once
#include "CUI.h"
class CUI_Image :
    public CUI
{
private:
    explicit CUI_Image(const CUI_Image& rhs);
    explicit CUI_Image(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUI_Image();

public:
    static CUI_Image* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& texKey, int count = 1);

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Set_Scale(const _vec3& vScale) { if (m_pTransform) m_pTransform->Set_Scale(vScale); }
    void Set_Position(float x, float y, float z) { if (m_pTransform) m_pTransform->Set_Pos(x, y, z); }
    void Set_TextureKey(const wstring& key) { m_strTextureKey = key; }

    void SetSpriteCount(int count) { m_tileCount = max(1, count); }
    void SetSpriteIndex(int idx) { m_tileIndex = idx; }

private:

    wstring m_strTextureKey;
    _int m_tileCount = 1;
    _int m_tileIndex = 0;

    int m_hoverIndex = -1;
    bool m_bButtonFun = false;

private:
    void Free() override;
};

