#pragma once
#include "CMapToolMapObject.h"


BEGIN(Engine)

class ENGINE_DLL CMapToolEventObject :
    public CMapToolMapObject
{

private:
    explicit CMapToolEventObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMapToolEventObject(const CMapToolEventObject& rhs);
    virtual ~CMapToolEventObject();
public:

    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void        SetBufferInt(int Count) { m_iBufferInt = Count; }
    int         GetBufferInt() { return m_iBufferInt; }


private:

    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();

public:

    static  CMapToolEventObject* Create(LPDIRECT3DDEVICE9 Device);


protected:
    
    _int                    m_iBufferInt;


private:
    virtual     void        Free();
};


END

