#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer :
    public CBase
{
protected:
    explicit CLayer();
    virtual ~CLayer();

public:
    CComponent* Get_Component(
        COMPONENTID eID,
        const wstring& pObjTag,
        const wstring& pComponentTag);

    HRESULT     Add_GameObject(const wstring& pObjTag, CGameObject* pGameObject);

public:
    virtual HRESULT     Ready_Layer();
    virtual _int        Update_Layer(const _float& fTimeDelta);
    virtual void        LateUpdate_Layer(const _float& fTimeDelta);
    virtual void        Render_Layer();

protected:
    unordered_map<wstring, CGameObject*>        m_mapObject;
    
public:
    // 외부에서 CLayer::Create를 통해서 생성하고 
    // CGameObject를 Add_GameObject를 통해 생성한다.

    static CLayer* Create();

    // Create가 있는 클래스는 실 객체를 만드는 클래스.

protected:
    virtual void    Free();

};

END
