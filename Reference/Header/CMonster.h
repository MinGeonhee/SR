#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class CBillboard;

class ENGINE_DLL CMonster :
    public CGameObject
{
protected:
    //explicit    CMonster();
    explicit    CMonster(const CMonster& rhs);
    explicit    CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

protected:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT     Add_Component();

protected:
    Engine::CTransform* m_pPlayerTransCom;
    Engine::CBillboard* m_pBillboardCom;
    Engine::CAnimation* m_pAnimationCom;
    Engine::CRecTex*    m_pBufferCom;


protected:
    virtual void    Free();

protected:
    MONSTER_TYPE    m_eMonsterType;

public:
    void                Set_MonsterType(MONSTER_TYPE eType) { m_eMonsterType = eType; }
    MONSTER_TYPE        Get_MonsterType() { return m_eMonsterType; }

    void                Set_MonsterRank(MONSTERRANK eType) { m_eMonsterRank = eType; }
    MONSTERRANK         Get_MonsterRank() { return m_eMonsterRank; }

protected:

    MONSTER_STATUS  m_eMonsterStatus;
public:
    MONSTER_STATUS  Get_MonsterStatus() { return m_eMonsterStatus; }
    void            Set_MonsterStatus(MONSTER_STATUS _e) {  m_eMonsterStatus = _e; }

    MONSTER_STATUS  m_ePrevMonsterStatus;

    // 몬스터 등급
    MONSTERRANK     m_eMonsterRank;

protected:

    // 디폴트 10으로
    float   m_fPlayerRecog_Distance = 8.f;
    // 디폴트 3으로
    float   m_fAttack_Distance = 3.f;


public:
    void    Hit();
    void    Die();
    void    MoveSound(float fTimeDelta);
protected:
    bool    m_bIsBuffed = false;
public:
    void    Set_IsBuffed(bool _b) { m_bIsBuffed = _b; }
    bool    Get_IsBuffed() { return m_bIsBuffed; }

protected:
    bool    m_bIsSpawned = false;

    float m_vocalTimer = 0.f;
    float m_vocalHearingMax = 10.0f;
    bool m_canPlay = true;
};

END

