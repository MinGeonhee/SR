#include "pch.h"
#include "CUI_Image.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"

CUI_Image::CUI_Image(LPDIRECT3DDEVICE9 pGraphicDev)
    :CUI(pGraphicDev)
{

}

CUI_Image::CUI_Image(const CUI_Image& rhs)
    :CUI(rhs)
{

}

CUI_Image::~CUI_Image()
{

}

HRESULT CUI_Image::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_tHandle.m_eType = OBJ_UI;
    m_bActive = true;
    return S_OK;
}

_int CUI_Image::Update_GameObject(const _float& fTimeDelta)
{
    CUI::Update_GameObject(fTimeDelta);

    return 0;
}

void CUI_Image::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CUI::LateUpdate_GameObject(fTimeDelta);
}


void CUI_Image::Render_GameObject()
{
    if (!m_bActive)
        return;
    CUI::Render_GameObject();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    CResourceManager::GetInstance()->Get<CTexture>(m_strTextureKey)->Set_Texture();

    int idx = m_tileIndex;
    if (idx < 0) idx = 0;
    if (idx >= m_tileCount) idx = m_tileCount - 1;

    m_pBufferCom->Change_UV(idx, m_tileCount, 1, 1);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CUI_Image* CUI_Image::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& texKey, int count)
{
    CUI_Image* pImg = new CUI_Image(pGraphicDev);
    pImg->Set_TextureKey(texKey);
    pImg->SetSpriteCount(count);

    if (FAILED(pImg->Ready_GameObject()))
    {
        Safe_Release(pImg);
        MSG_BOX("pImg Create Failed");
        return nullptr;
    }

    return pImg;
}

void CUI_Image::Free()
{
    CUI::Free();
}