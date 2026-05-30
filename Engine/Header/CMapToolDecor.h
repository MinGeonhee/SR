#pragma once
#include "CMapToolMapObject.h"


class CMapToolDecor final :
    public CMapToolMapObject 
{
private:
	explicit CMapToolDecor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMapToolDecor(const CMapToolDecor& rhs);
	virtual ~CMapToolDecor();


public:

	virtual     _int        Update_GameObject(const _float& fTimeDelta) override;
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void        Render_GameObject() override;

private:
	virtual     HRESULT			Ready_GameObject();
	HRESULT						Add_Component();
public:
	static  CMapToolDecor* Create(LPDIRECT3DDEVICE9 Device);


private:
	virtual     void        Free();
};

