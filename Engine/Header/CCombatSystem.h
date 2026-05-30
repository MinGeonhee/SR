#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject; 
class CBullet;
class CInteractable;
struct EVENT;

class ENGINE_DLL CCombatSystem :
    public CBase
{
    DECLARE_SINGLETON(CCombatSystem)

public:
    explicit CCombatSystem();
    virtual ~CCombatSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

    bool    HasFrozenObject()
    {
        return (m_lFrozenObjectList.size() > 0);
    }

private:

    void    ApplyStatusChanges();   // 전투를 통해 서로의 Status가 깎이거나 증가하는 형태.
    void    ApplyCombatRules();     // 게임 전투 룰 적용하는 함수.
                                    // ex) 상태이상) burn(화상), Freezed(빙결) 처리.


    void    InteractionCombatEvent(EVENT* event);
    void    ReflectBulletfromBarrierEVENT(EVENT* event);

    void    OrbFieldCombatEvent(CGameObject* pBullet, CInteractable* pInteraction);

    void    BrokenEvent(CGameObject* pBullet, CInteractable* pInteraction);
    void    SimpleBrokenEvent(CInteractable* pInteraction);
    void    ItemDropBrokenEvent(CInteractable* pInteraction);
    void    OrbBrokenEvent(CInteractable* pInteraction);
    void    FLowerBushBrokenEvent(CGameObject* pBullet, CInteractable* pInteraction);

    void    AntenaeBrokenEvent(CInteractable* pInteraction);

    void    ApplyStatusEffect();    // 상태이상 읽어들이는 곳.
private:
    void    Free();

private:

    list<CGameObject*>        m_lFrozenObjectList; // 얼어붙은 오브젝트 담아두는 배열.

private:
    void    Play_Particle_Bush(CInteractable* pInteraction);
    void    Play_Particle_Barrel(CInteractable* pInteraction);
    void    Play_Particle_Blood(CInteractable* pInteraction);
    void    Play_Particle_Book(CInteractable* pInteraction);
};

END