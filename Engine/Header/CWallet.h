#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CWallet :
    public CComponent
{
private:
	explicit CWallet();
	explicit CWallet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWallet(const CWallet& rhs);
	virtual ~CWallet() { Free(); };

public:
	HRESULT         Ready_PlayerWallet();
	virtual _int    Update_Component(const _float& fTimeDelta) override;

	// 얘는 왜 LateUpdate에 시간 값이 안들어가냐.
	virtual void    LateUpdate_Component(const _float& fTimeDelta) override;

public:

	void	Set_Money(int count) { m_iWalletMoney = count; }
	int		Get_Money() { return m_iWalletMoney; }
	void	ADD_Money(int iMoney) { m_iWalletMoney += iMoney; }
	void	Dicrease_Money(int iMoney) 
	{ 
		m_iWalletMoney -= iMoney; 
		if (m_iWalletMoney <= 0) m_iWalletMoney = 0;
	}
public:
	virtual CComponent* Clone();
	static  CWallet* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:

	int		m_iWalletMoney = 0;		// uint로 하기에는 -를 몰라서 예외처리하기 힘듬
};

END