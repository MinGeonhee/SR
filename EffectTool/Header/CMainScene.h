#pragma once
#include "CScene.h"

class CMainScene :
    public CScene
{
public:
	explicit CMainScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainScene();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag);

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Resource();

private:
	HRESULT			Add_Cameraman(CLayer* pLayer);


public:
	static CMainScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void			Free();
};