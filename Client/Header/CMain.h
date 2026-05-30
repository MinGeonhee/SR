#pragma once
#include "CScene.h"
#include "CLoading.h"
class CMain :
    public CScene
{
public:
	explicit CMain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMain();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(const wstring& pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const wstring& pLayerTag);
	HRESULT			Ready_UI_Layer(const wstring& pLayerTag);

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Manager();

public:
	static CMain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void			Free();
};

