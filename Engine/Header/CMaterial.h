#pragma once
#include "CResourceBase.h"
#include <d3dx9.h>
#include <string>

using namespace std;

BEGIN(Engine)

class ENGINE_DLL CMaterial : public CResourceBase
{
    using Super = CResourceBase;

public:
    struct Desc
    {
        D3DXCOLOR Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
        D3DXCOLOR Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
        D3DXCOLOR Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
        D3DXCOLOR Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
        float     Power = 0.f; // 하이라이트 지수. 0이면 스페큘러 거의 없음.
    };

protected:

    explicit CMaterial(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMaterial(const CMaterial& rhs);
    virtual ~CMaterial();

public:

    HRESULT          Ready_Material(const Desc& d); 
    void             Set_Material() const;       
    virtual void     Load(const wstring& path) override; // 굳이 굳이 Json 써보기. 간지나잖아

    const D3DMATERIAL9& GetNative() const { return m_Material; }

    static CMaterial* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& path);
    virtual CResourceBase* Clone() override;
    virtual void        Free() override;

private:

    D3DMATERIAL9 m_Material; 
};

END
