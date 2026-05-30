#pragma once
#include "CScene.h"

class CToolManager;




class CToolScene :
    public CScene
{
public:
	explicit CToolScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolScene();

public:
	virtual			HRESULT				Ready_Scene();
	virtual			_int				Update_Scene(const _float& fTimeDelta);
	virtual			void				LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void				Render_Scene();

	void								Ready_ResourceTileImage(LPDIRECT3DDEVICE9 device);

private:
	HRESULT								Ready_Environment_Layer(const wstring& pLayerTag);
	HRESULT								Ready_GameLogic_Layer(const wstring& pLayerTag);
	HRESULT								Ready_UI_Layer(const wstring& pLayerTag);
	HRESULT								Ready_Prototype();
	HRESULT								Ready_CollisionTable();
	HRESULT								Ready_Resource();
	HRESULT								Ready_ToolManager();


private:
	HRESULT								Add_Cameraman(CLayer* pLayer);					

public:
	static CToolScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CToolManager*			m_pToolManager;

protected:
	virtual void			Free();
};

