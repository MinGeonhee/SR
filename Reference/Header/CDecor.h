#pragma once
#include "CMapObject.h"

BEGIN(Engine)


class ENGINE_DLL CDecor : public CMapObject
{
protected:

    explicit CDecor(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDecor(const CDecor& rhs);
    virtual ~CDecor();


protected:

};

END