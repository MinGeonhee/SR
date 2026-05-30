#pragma once

#include "CLightObject.h"



namespace Engine {
	class CBillboard;
};

class CHangingbrazier :
    public CLightObject
{
public:
	explicit	CHangingbrazier(LPDIRECT3DDEVICE9 Device);
	explicit	CHangingbrazier(const CHangingbrazier& cpy);
	virtual		~CHangingbrazier();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CHangingbrazier* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();

protected:

	Engine::CBillboard* m_pBillboardCom;

};

