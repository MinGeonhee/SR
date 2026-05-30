#pragma once
#include "CLightObject.h"


namespace Engine {
	class CBillboard;
};

class CTripleCandle :
    public CLightObject
{
public:
	explicit	CTripleCandle(LPDIRECT3DDEVICE9 Device);
	explicit	CTripleCandle(const CTripleCandle& cpy);
	virtual		~CTripleCandle();

public:
	virtual     HRESULT      Ready_GameObject();
	virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
	virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void         Render_GameObject() override;

	static  CTripleCandle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

	HRESULT         Add_Component();
	virtual void    Free();

protected:

	Engine::CBillboard* m_pBillboardCom;
};

