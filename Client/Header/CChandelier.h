#pragma once
#include "CLightObject.h"



namespace Engine {
	class CBillboard;
};


class CChandelier :
    public CLightObject
{

public:
		explicit	CChandelier(LPDIRECT3DDEVICE9 Device);
		explicit	CChandelier(const CChandelier& cpy);
		virtual		~CChandelier();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CChandelier* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();

protected:

	Engine::CBillboard* m_pBillboardCom;
};

