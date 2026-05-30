#pragma once


#include "CBase.h"
#include "Engine_Define.h"


BEGIN(Engine)


struct EVENT;
class CGameObject;


class ENGINE_DLL CEyeOfChaosSystem
	: public CBase
{
	DECLARE_SINGLETON(class CEyeOfChaosSystem);
public:
	explicit CEyeOfChaosSystem();
	virtual ~CEyeOfChaosSystem();

public:
	_int		System_Update(const float& fTimeDelta);
	void		System_LateUpdate(const float& fTimeDelta);

public:

	void		Apply_EventBus();

	void		EyeOfChaos_Event(CGameObject* pGameObject);

private:
	virtual void	Free();
};

END