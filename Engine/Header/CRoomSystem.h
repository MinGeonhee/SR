#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRoomSystem :
    public CBase
{
    DECLARE_SINGLETON(CRoomSystem)

public:
    explicit CRoomSystem();
    virtual ~CRoomSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    void    DoorCollisionShowUINotice();


private:
    void    Free();
};

END