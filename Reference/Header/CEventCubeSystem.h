#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)


struct EVENT;
class CEventCube;
class CGameObject;

class ENGINE_DLL CEventCubeSystem
	: public CBase
{
	DECLARE_SINGLETON(CEventCubeSystem)

public:
	explicit CEventCubeSystem();
	virtual ~CEventCubeSystem();

public:
	_int		System_Update(const float& fTimeDelta);
	void		System_LateUpdate(const float& fTimeDelta);

public:

	void		ApplyEventCube_EventBus();
	void		EventCube_InteractionEvent_ByPlayer(CGameObject* pSource,CGameObject* pTarget);
	void		EventCube_InteractionEvent_ByWeapon(CGameObject* pSource,CGameObject* pTarget);

private:
	virtual void	Free();
};

END