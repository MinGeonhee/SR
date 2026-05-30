#include "CTexture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
    : CResourceBase(RESOURCETYPE::TEXTURE, pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
    : CResourceBase(rhs)
{
}

CTexture::~CTexture()
{
}

// 스카이 박스 할 때 필요함.
HRESULT CTexture::Ready_Texture(const wstring& pPath,
    TEXTUREID eType)
{
    LPDIRECT3DCUBETEXTURE9* pTexture = NULL;


    if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, pPath.c_str(), pTexture)))
    {
        MSG_BOX("큐브 텍스처 로딩 실패!");
        return E_FAIL;
    }

     

    return S_OK;
}

// 사진 텍스처를 여러장 저장해놓고
// 애니메이션 돌리면 돌아갈 때마다 SetTexture가 호출됨.
void CTexture::Set_Texture()
{
    m_pGraphicDev->SetTexture(0, m_Texture);
}

void CTexture::Load(const wstring& path)
{
    IDirect3DTexture9* pTexture = nullptr;

    D3DXCreateTextureFromFileEx(
        m_pGraphicDev,
        path.c_str(),
        D3DX_DEFAULT, D3DX_DEFAULT,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED,
        D3DX_FILTER_POINT,     // ← 보간 없이 리사이징은 허용
        D3DX_FILTER_NONE,
        0,
        NULL,
        NULL,
        &pTexture);


    if (pTexture == NULL)
    {
        MSG_BOX("텍스처 로딩 실패!");
        return;
    }

    m_Texture = pTexture;
}

CResourceBase* CTexture::Clone()
{
    return new CTexture(*this);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pPath)
{
    CTexture* pTexture = new CTexture(pGraphicDev);

    if (pTexture == NULL)
        return nullptr;

    pTexture->Load(pPath);

    return pTexture;
}

CTexture* CTexture::CubeCreate(LPDIRECT3DDEVICE9 pGraphicDev,
    const wstring& pPath,
    TEXTUREID eType)
{
    CTexture* pTexture = new CTexture(pGraphicDev);

    if (FAILED(pTexture->Ready_Texture(pPath, eType)))
        return nullptr;

    return pTexture;

}

void CTexture::Free()
{
    Safe_Release(m_Texture);

    CResourceBase::Free();
}
