#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimeSystem :
    public CBase
{
    DECLARE_SINGLETON(CTimeSystem)

public:
    explicit CTimeSystem();
    virtual ~CTimeSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    void    Time();


private:
    void    Free();
};

END