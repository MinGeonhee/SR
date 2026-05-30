#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CStatus :
    public CComponent
{
private:
    explicit CStatus();
    explicit CStatus(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CStatus(const CStatus& rhs);
    virtual ~CStatus() { Free(); };

public:

    virtual _int Update_Component(const _float& iTimeDelta);
    virtual void LateUpdate_Component(const _float& iTimeDelta);

public:
    // 내 스테이터스 등록하는 함수
    void           Set_Status(STATUS tStatus) { m_tStatus = tStatus; }

    STATUS         Get_Status() { return m_tStatus; }
    // 내 체력 가져오는 함수
    float          Get_CurrentHp() { return m_tStatus.fCurrentHp; }
    // 내 체력 설정하는 함수
    void           Set_CurrentHp(float fHp) { m_tStatus.fCurrentHp = fHp; }
    // 내 체력 깎는 함수 (이벤트성 함수.)
    void OnDamaged(float fDamage)
    {
        m_tStatus.fCurrentHp -= fDamage;
    }

    void          Add_Armor(int count)
    {
        m_tStatus.fDefence += count;
    }
    // 내 체력 올리는 함수 (이벤트성 함수.)
    void           OnHealed(float fAmount) { m_tStatus.fCurrentHp += fAmount; }
    // 일정 MAX 체력보다 CurrentHp가 더 커지면 고정하는 함수
    void           ClampCurrentHp();
    void           Reset_Hp()
    {
        m_tStatus.fCurrentHp = m_tStatus.fMaxHp;
        // 풀링 전용
    }

    // 내 스피드 올리는 함수
    void           IncreaseSpeed(float fAmount) { m_tStatus.fSpeed += fAmount; };
    // 내 스피드 깎는 함수
    void           DecreaseSpeed(float fAmount) { m_tStatus.fSpeed -= fAmount; };
    // 내 스피드 가져오는 함수
    float          GetSpeed() const { return m_tStatus.fSpeed; };
    // 내 스피드 설정하는 함수
    void           SetSpeed(float fSpeed) { m_tStatus.fSpeed = fSpeed; };

public:

    STATUSEFFECT   Get_StatusEffect() { return m_eStatusEffect; }
    void           Add_FrozenCount() { m_iFrozenStack++; }

public:
    HRESULT            Ready_Status();

    static CStatus* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent*     Clone();
    void            Free();

private:

    void            Update_FrozenByIceStacks(const float& fTimeDelta);  // 얼어붙었니 체크

    STATUS          m_tStatus{ 0.f,0.f,0.f,0.f };

private:
    // 상태이상 관련 변수
    STATUSEFFECT        m_eStatusEffect;
    uint8_t             m_iFrozenStack = 0;
    float               m_fElapsedtime = 0.f;
    float               m_fFreezeTime = 3.f;
     
    bool                m_bFrozen = false;
    bool                m_bFrozenEvent = false;
public:
    bool                Get_bFrozen() { return m_bFrozen; }
};

END