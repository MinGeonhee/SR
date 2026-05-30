#pragma once
#include "CBase.h"
#include "CEventBus.h"
#include "Engine_Define.h"

BEGIN(Engine)

// 이 시스템은 EventBus에서 CHANNEL DOOR를 구독하는 시스템이다.

class ENGINE_DLL CDoorSystem 
    : public CBase
{
    DECLARE_SINGLETON(CDoorSystem)

public:
    explicit CDoorSystem();
    virtual ~CDoorSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    void    DoorCollisionShowUINotice();
    void    FireInterationRayCast();

private:
    void    Free();
};

END