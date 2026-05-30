#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBarrier :
    public CComponent
{
private:
	explicit CBarrier();
	explicit CBarrier(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBarrier(const CBarrier& rhs);
	virtual ~CBarrier() { Free(); };
	HRESULT Ready_Barrier();

public:

	virtual _int Update_Component(const _float& iTimeDelta);
	virtual void LateUpdate_Component(const _float& iTimeDelta);

public:

	void	Set_BarrierCount(int count) { m_ibarrierCount = count; }
	void	Set_BarrierCountDown() { m_ibarrierCount--; }
	int		Get_BarrierCount() { return m_ibarrierCount; }
	_vec3	Get_ReflectDir(const _vec3& bulletPos, const _vec3& Bulletdir);
	void	Set_BarrierActive(bool b) { m_fBarrierActive = b; }
	_bool	Get_BarrierActive() { return m_fBarrierActive; }

public:
	virtual CComponent* Clone();
	static  CBarrier* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	_bool				m_fBarrierActive = false;
	int					m_ibarrierCount = 0;
};

END
