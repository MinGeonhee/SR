#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRendererManager :
    public CBase
{
    DECLARE_SINGLETON(CRendererManager)
private:
   
    explicit CRendererManager();
    virtual ~CRendererManager();

public:
    void        Add_RenderGroup(RENDERID eType, CGameObject* pGameObject);
    void        Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Clear_RenderGroup();

private:
    void        Render_TileTex(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_GroundDecor(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_WallDecor(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_Obstacle(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_Event(LPDIRECT3DDEVICE9 pGraphicDev);

    void        Render_SkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_AlphaTest(LPDIRECT3DDEVICE9 Device);
    void        Render_AlphaBlend(LPDIRECT3DDEVICE9 pGraphicDev);
    void        Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    list<CGameObject*>              m_RenderGroup[RENDER_END];

private:
    virtual void                Free();
};

END