#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
private:
    explicit CCubeCol();
    explicit CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCubeCol(const CCubeCol& rhs);
    virtual ~CCubeCol();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

    void            Change_Color(const _vec4& Color);

protected:

    virtual void    Make_Vertex(VTXCOL* pVerTex) override;
    virtual void    Make_Index(INDEX32* pIndex) override;

public:
    virtual CComponent* Clone() override;

    static  CCubeCol* Create(LPDIRECT3DDEVICE9 mGraphicDev);

private:
    virtual void    Free();

};

END