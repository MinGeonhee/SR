#pragma once
#include "CLightObject.h"

namespace Engine {
	class CBillboard;
};

class CCandle :
    public CLightObject
{
public:
public:
	explicit	CCandle(LPDIRECT3DDEVICE9 Device);
	explicit	CCandle(const CCandle& cpy);
	virtual		~CCandle();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CCandle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();

protected:

	Engine::CBillboard* m_pBillboardCom;
};

