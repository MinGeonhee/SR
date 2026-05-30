#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCameraSystem :
    public CBase
{
    DECLARE_SINGLETON(CCameraSystem)

public:
    explicit CCameraSystem();
    virtual ~CCameraSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    void    Camera();

    void    Boss_Room_Camera();
    void    DeathSequenceCam();
    void    IslandTourCam();

private:
    void    Free();

private:
    _vec3       m_vSavePlayerPos;
};

END