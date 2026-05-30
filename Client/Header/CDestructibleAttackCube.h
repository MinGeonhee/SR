#pragma once
#include "CEventCube.h"


class CDestructibleAttackCube
	: public CEventCube
{
private:
	CDestructibleAttackCube(LPDIRECT3DDEVICE9 Device);
	CDestructibleAttackCube(const CDestructibleAttackCube& cpy);
	virtual ~CDestructibleAttackCube();


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
	static CDestructibleAttackCube* Create(LPDIRECT3DDEVICE9 Device);
};

