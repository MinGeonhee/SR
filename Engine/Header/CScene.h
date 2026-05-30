#pragma once
#include "CBase.h"
#include "CLayer.h"

BEGIN(Engine)

class ENGINE_DLL CScene :
    public CBase
{
 public:
    explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CScene();

public:
    CComponent* Get_Component(
        COMPONENTID eID,
        const wstring& pLayerTag,
        const wstring& pObjTag,
        const wstring& pComponentTag);

public:
    virtual      HRESULT     Ready_Scene();
    virtual      _int        Update_Scene(const _float& fDeltaTime);
    virtual      void        LateUpdate_Scene(const _float& fDeletaTime);
    virtual      void        Render_Scene();


public:


protected:
    unordered_map<wstring, CLayer*>         m_mapLayer;
    LPDIRECT3DDEVICE9                   m_pGraphicDev;

protected:
    virtual     void        Free();

};

END

