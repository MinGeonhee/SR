#pragma once
#include "CVIBuffer.h"


BEGIN(Engine)

class ENGINE_DLL CGridCol :
    public CVIBuffer
{
private:

    explicit CGridCol();
    explicit CGridCol(LPDIRECT3DDEVICE9 device);
    explicit CGridCol(const CGridCol& cpy);
    virtual ~CGridCol();



public:
    virtual HRESULT Ready_Buffer(const int32& CntX, const int32& CntZ, const int32 Floor, int32  Size = 1);
    virtual void    Render_Buffer();

public:
    static      CGridCol*  Create(LPDIRECT3DDEVICE9 device, const int32& CntX, const int32& CntZ, const int32 Floor, int32 Size = 1);

    virtual CComponent* Clone() override;


private:
    virtual     void    Free();
};


END

