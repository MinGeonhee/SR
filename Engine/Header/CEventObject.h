#pragma once
#include "CMapObject.h"
class CEventObject :
    public CMapObject
{
protected:
    explicit CEventObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CEventObject(const CEventObject& rhs);
    virtual ~CEventObject();

public:
    virtual void    Setting_EventObject(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale,
        const       MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const EVENT_PATH& EventPath) = 0;

    void                        SetObject_Event_Type(const OBJECT_EVENT_TYPE& eType) { m_eObject_Event_Type = eType; }
    const OBJECT_EVENT_TYPE&    GetObject_Event_Type() { return m_eObject_Event_Type; }

protected:
    OBJECT_EVENT_TYPE   m_eObject_Event_Type;
};

