#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRectCol :
    public CVIBuffer
{
private:

    explicit CRectCol();
    explicit CRectCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRectCol(const CRectCol& rhs);
    virtual ~CRectCol();

public:
    // 누구 기준으로 
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

    void            Change_Color(const _vec4& Color);


protected:

    virtual void    Make_Vertex(VTXCOL* pVerTex) override;
    virtual void    Make_Index(INDEX32* pIndex) override;

public:
    virtual CComponent* Clone() override;

    static  CRectCol* Create(LPDIRECT3DDEVICE9 mGraphicDev);
    // 이거 객체 생성할 때 왜 컴 객체를 들고 오지.
    // 아 생성할 때 생성자에 넣어주려고.

private:
    virtual void    Free();

};

END
