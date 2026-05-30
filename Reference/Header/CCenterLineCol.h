#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCenterLineCol :
    public CVIBuffer
{
private:

    explicit CCenterLineCol();
    explicit CCenterLineCol(LPDIRECT3DDEVICE9 device);
    explicit CCenterLineCol(const CCenterLineCol& cpy);
    virtual ~CCenterLineCol();



public:
    virtual HRESULT Ready_Buffer(const _float& Floor);
    virtual void    Render_Buffer();

public:
    static      CCenterLineCol* Create(LPDIRECT3DDEVICE9 device, const _float& Floor);

    virtual CComponent* Clone() override;


private:
    virtual     void    Free();
};

END