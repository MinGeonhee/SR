#pragma once
#include "CUI.h"
class CUIBackGround :
    public CUI
{
private:
    explicit CUIBackGround(const CUIBackGround& rhs);
    explicit CUIBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUIBackGround();

    virtual void            Free();

public:
    static CUIBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring strTextureKey);

	void Set_TextureKey(const wstring& key) { m_strTextureKey = key; }

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    wstring m_strTextureKey;
};

