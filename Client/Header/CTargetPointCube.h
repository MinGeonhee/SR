#pragma once

#include "CEventCube.h"

class CTargetPointCube
	: public CEventCube
{
private:
	CTargetPointCube(LPDIRECT3DDEVICE9 Device);
	CTargetPointCube(const CTargetPointCube& cpy);
	virtual ~CTargetPointCube();


public:

	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();

public:

	void		PlayEvent() override;
	
private:

	virtual     HRESULT     Ready_GameObject();
	virtual		HRESULT	    Add_Component();

public:
	static CTargetPointCube* Create(LPDIRECT3DDEVICE9 Device);
};

