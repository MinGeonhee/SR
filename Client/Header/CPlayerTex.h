#pragma once
#include "CUI.h"
#include "CProtoTypeManager.h"

enum MoveState { NONE, TO_RIGHT, TO_LEFT, TO_DOWN, TO_UP, TO_ORIGIN, HOLD, WALKING };
enum class PlayerStatus { IDLE, WALKING, ATTACKING, CLEAR, DEAD,};
class CWeaponController;

class CPlayerTex :
    public CUI
{
private:

    explicit CPlayerTex(const CPlayerTex& rhs);
    explicit CPlayerTex(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CPlayerTex();

public:

    static CPlayerTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject() override;

    void Mace_Action(const _float& fTimeDelta);
    void Ice_Action(const _float& fTimeDelta);
    void FireBall_Action(const _float& fTimeDelta);
    void FireWand_Action(const _float& fTimeDelta);
    void Gun_Action(const _float& fTimeDelta);
	void Clear_Action(const _float& fTimeDelta);
	void Dead_Action(const _float& fTimeDelta);

    void ResetWeaponSprite();
private:
    virtual void            Free() override;

    Engine::CWeaponController* m_pWeaponCon;

    _vec3 vDir;

    int m_iUVX = 0;
    int m_iUVY = 0;

    bool isMoving;
    bool bChargeAttack = false;
    wstring m_cachedTag;

    PlayerStatus m_status = PlayerStatus::IDLE;
    PlayerStatus m_prevStatus = PlayerStatus::IDLE;
    MoveState m_eMoveState = MoveState::NONE;
    MoveState m_eLeftState = MoveState::NONE;
    MoveState m_eRightState = MoveState::NONE;

    WEAPONSTATE m_eWeaponState;
    WEAPONSTATE m_prevWeaponState;

    _vec3 m_vLeftOfs = { 0.f, 0.f, 0.f };
    _vec3 m_vRightOfs = { 0.f, 0.f, 0.f };
    _vec3 m_vOfs = { 0.f, 0.f, 0.f };
    _vec3 m_vScaleOfs = { 1.f, 1.f, 1.f };

    float m_fRightRot = 0.f;
    float m_fLeftRot = 0.f;
    float m_fRot = 0.f;

    float HoldTimer = 0.0f;
    const  float kHoldDuration = 0.3f;

    int m_iPlayerBul;
    int m_iLeftUVX = 0; // 왼손 프레임
    int m_iRightUVX = 0;// 오른손 프레임
    int m_iUVXOffset = 0;//양손uv좌표오프셋
    bool m_bLeftHandOn = true;
    bool m_clearEventFired = false;

    float m_idleT = 0.f; // 경과 시간
    float m_idleSpeed = 1.6f; // 대기모션 시간(속도용)
    _vec3 m_idleAmp = { 8.f, 6.f, 0.f }; // 흐느들림수치
    float m_idleRotAmp = D3DXToRadian(1.f); // 흔들림회전수치
    float m_idlePhase = D3DX_PI * 0.65f; //두손일때 한손은 출발시점에 차이를 줘서 움직임이 좀더다르게
    //위에거 걷기용
    float m_walkT = 0.f;
    float m_walkSpeed = 5.f;
    _vec3 m_walkAmp = { 28.f, 30.f, 0.f };
    float m_walkPhase = D3DX_PI;
};

