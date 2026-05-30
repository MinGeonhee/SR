#pragma once

#include "CLightObject.h"


namespace Engine {
	class CBillboard;
};


class CStatue :
	public CLightObject
{
public:
	explicit	CStatue(LPDIRECT3DDEVICE9 Device);
	explicit	CStatue(const CStatue& cpy);
	virtual		~CStatue();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();

protected:

	Engine::CBillboard* m_pBillboardCom = nullptr;

};

