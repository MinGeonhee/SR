#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class EVENT;
class CGameObject;

class ENGINE_DLL CInteractionSystem : public CBase
{
	DECLARE_SINGLETON(CInteractionSystem)

public:
	explicit CInteractionSystem();
	virtual ~CInteractionSystem();

public:
	_int    System_Update(const float& fTimeDelta);
	void    System_LateUpdate(const float& fTimeDelta);


public:
	void		InteractionCollisionEvent();

	void		InteractionEatEvent(EVENT& event);
	void		InteractionUseEvent(EVENT& event);
	void		InteractionSearchEvent(EVENT& event);

	void		FireInterationRayCast();

	void		InteractionDropItem(CGameObject* Object, const _int& DropAmount);
	void		SpawnMonsterGimmik(EVENT& event);	// 함정카드 발동
	void		SpawnMonsterAndBOOMGimmik(EVENT& event);	// 함정카드에 함정카드발동

	void		Play_Particle_Eat(CGameObject* pInteraction);
private:
	virtual void	Free();
};

END

