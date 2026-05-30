#pragma once
#include "CResourceBase.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :
    public CResourceBase
{
    // 자기 부모
    using Super = CResourceBase;

protected:

    explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTexture(const CTexture& rhs);
    virtual ~CTexture();

public:

    HRESULT          Ready_Texture(const wstring& pPath, TEXTUREID eType);
    void             Set_Texture();
    virtual void     Load(const wstring& path) override;

    _vec2            GetSize() const { return _size; }

    IDirect3DBaseTexture9* GetIDirect3DBaseTexture9()
    {
        if (m_Texture == nullptr)
            return nullptr;
        return m_Texture;
    }


private:

    // 실제 텍스처 객체를 저장하는 컨테이너
    IDirect3DBaseTexture9*              m_Texture;
    _vec2                               _size = { 0.f, 0.f };

public:

    CResourceBase* Clone();
    virtual void    Free() override;

    // 텍스처 파일 경로
    // 로딩할 텍스처의 수
    static CTexture* Create(
        LPDIRECT3DDEVICE9 pGraphicDev,
        const wstring& pPath);

    // 큐브로 만드는 텍스처. 스카이 박스 제외 사용하지 않는다.
    static CTexture* CubeCreate
    (LPDIRECT3DDEVICE9 pGraphicDev,
        const wstring& pPath,
        TEXTUREID eType);

};

END