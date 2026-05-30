#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CSoundSystem :
    public CBase
{
    DECLARE_SINGLETON(CSoundSystem)

public:
    explicit CSoundSystem();
    virtual ~CSoundSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    void    FireSound();


private:
    void    Free();
};

END