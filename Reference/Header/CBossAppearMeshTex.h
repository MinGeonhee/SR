#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBossAppearMeshTex :
    public CVIBuffer
{
private:
    explicit CBossAppearMeshTex();
    explicit CBossAppearMeshTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossAppearMeshTex(const CBossAppearMeshTex& rhs);
    virtual ~CBossAppearMeshTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();


public:
    virtual CComponent* Clone() override;
    static CBossAppearMeshTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);



private:
    virtual void Free() override;

};

END


