#pragma once
#include "CEventCube.h"


class CPortalCube :
    public CEventCube
{
private:
	explicit CPortalCube(LPDIRECT3DDEVICE9 Device);
	explicit CPortalCube(const CPortalCube& cpy);
	virtual ~CPortalCube();


public:

	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();

public:

	void		PlayEvent() override;

private:

	virtual		HRESULT		Add_Component();
	virtual     HRESULT     Ready_GameObject();
	virtual		void		Free();

public:
	static CPortalCube* Create(LPDIRECT3DDEVICE9 Device);

};

