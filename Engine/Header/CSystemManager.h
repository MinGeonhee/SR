#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

// 그냥 모든 시스템 업데이트를 돌려주는 시스템 매니저
class ENGINE_DLL CSystemManager :
    public CBase
{
    DECLARE_SINGLETON(CSystemManager)

 public:
     explicit CSystemManager();
     virtual ~CSystemManager();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    virtual void        Free();
};

END