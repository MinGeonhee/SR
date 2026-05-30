#pragma once
#include "CLightObject.h"

namespace Engine {
	class CBillboard;
};

class CFirealtar :
    public CLightObject
{
public:
	explicit	CFirealtar(LPDIRECT3DDEVICE9 Device);
	explicit	CFirealtar(const CFirealtar& cpy);
	virtual		~CFirealtar();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CFirealtar* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();

protected:

	Engine::CBillboard* m_pBillboardCom;
};

