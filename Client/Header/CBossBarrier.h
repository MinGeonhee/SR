#pragma once
#include "CMonster.h"

class CBossBarrier :
    public CMonster
{
private:
	explicit CBossBarrier();
	explicit CBossBarrier(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossBarrier(const CBossBarrier& rhs);
	virtual ~CBossBarrier() { Free(); };

public:
	HRESULT         Ready_GameObject() override;
	virtual _int    Update_GameObject(const _float& fTimeDelta) override;

	// 얘는 왜 LateUpdate에 시간 값이 안들어가냐.
	virtual void    LateUpdate_GameObject(const _float& fTimeDelta) override;
	void			Render_GameObject();

private:

	void	SetMyPosition();
	void	Set_Animation();
	HRESULT	Add_Component();

public:

	static  CBossBarrier* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:

	CTexture*				m_pBarrierTexture = nullptr;
	CGameObject*			m_pBossOwner = nullptr;
	CBarrier*				m_pBarrier = nullptr;
};