#pragma once
#include "CLightObject.h"




class CWallTorch :
    public CLightObject
{
public:
	explicit	CWallTorch(LPDIRECT3DDEVICE9 Device);
	explicit	CWallTorch(const CWallTorch& cpy);
	virtual		~CWallTorch();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CWallTorch* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();
};

