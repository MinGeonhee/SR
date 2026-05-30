#pragma once
#include "CEventCube.h"

class CDestructibleGimicCube
	: public CEventCube
{
private:
	CDestructibleGimicCube(LPDIRECT3DDEVICE9 Device);
	CDestructibleGimicCube(const CDestructibleGimicCube& cpy);
	virtual ~CDestructibleGimicCube();


public:

	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();

public:

	void		PlayEvent() override;

private:
	virtual		void		Free();
	virtual     HRESULT     Ready_GameObject();
	virtual		HRESULT	    Add_Component();

public:
	static CDestructibleGimicCube* Create(LPDIRECT3DDEVICE9 Device);
};

