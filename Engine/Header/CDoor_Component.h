#pragma once
#include "CComponent.h"

BEGIN(Engine)

enum DOORSTATE { DOOR_NONE, DOOR_OPEN, DOOR_CLOSE, DOOR_END};

class ENGINE_DLL CDoor_Component :
    public CComponent
{
private:
    explicit CDoor_Component();
    explicit CDoor_Component(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDoor_Component(const CDoor_Component& rhs);
    virtual ~CDoor_Component() { Free(); };

public:
    
    virtual _int Update_Component(const _float& iTimeDelta);
    virtual void LateUpdate_Component(const _float& iTimeDelta);

public:

    void         OpenDoor() { m_eDoorState = DOOR_OPEN; }
    void         CloseDoor() { m_eDoorState = DOOR_CLOSE; }

    DOORSTATE    Get_DoorState() { return m_eDoorState; }
    void         Set_DoorState(DOORSTATE state) { m_eDoorState = state; }

    DOORTYPE     Get_DoorType() { return m_eDoorType; }
    void         Set_DoorType(DOORTYPE eType) { m_eDoorType = eType; }

    void         Set_SwitchCount(int count) { m_iswitchDoorCount = count; }
    int          Get_SwitchCount() { return m_iswitchDoorCount; }
    void         Decrease_Switchcount() { m_iswitchDoorCount--; if (m_iswitchDoorCount <= 0) m_iswitchDoorCount = 0; }

public:
    HRESULT            Ready_DoorComponent();
    void               DoorType_Setting();  // 내 주인놈 DoorType 설정 해주는 것. 초기 Initialize 될 때 한번 해주도록 하자.

    static CDoor_Component* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    CComponent*        Clone();
    void               Free();

private:

    DOORSTATE          m_eDoorState = DOORSTATE::DOOR_CLOSE;
    DOORTYPE           m_eDoorType = DOORTYPE::NONE_DOOR;
    int                m_iswitchDoorCount = 0;
    
};

END
