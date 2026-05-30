#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CInputSystem :
    public CBase
{
    DECLARE_SINGLETON(CInputSystem)

public:
    explicit CInputSystem();
    virtual ~CInputSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    void    KeyInput_Publish();

private:
    void    Free();
};

END