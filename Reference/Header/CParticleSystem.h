#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"
#include "CParticle.h"
#include "CObjectPoolingManager.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem :
    public CGameObject
{
protected:
    explicit    CParticleSystem(const CParticleSystem& rhs);
    explicit    CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CParticleSystem();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

protected:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();


public:
    static CParticleSystem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    Engine::CTransform*     m_pTargetTransCom;


// 파티클 시스템 상태 =========================
protected:
    PARTICLE_SYSTEM_STATUS m_eParticleSystemStatus = PARTICLE_SYSTEM_STATUS::PARTICLE_SYSTEM_STATUS_INACTIVE;

public:
	void Set_ParticleSystemStatus(PARTICLE_SYSTEM_STATUS eStatus) { m_eParticleSystemStatus = eStatus; }
	PARTICLE_SYSTEM_STATUS Get_ParticleSystemStatus() { return m_eParticleSystemStatus; }


// 파티클 시스템 활성화 =======================
public:
    void Activate_ParticleSystem();
	
// 파티클 입자 저장용 벡터 =====================
protected:
    vector<CGameObject*>      m_vecParticles; 

public:
	void Insert_Particles_From_PoolingMgr()
	{
        m_vecParticles = CObjectPoolingManager::GetInstance()->GetMany(OBJ_PARTICLE, m_iMaxParticleNum);
	}


// 바운딩 박스 크기 ============================
protected:
    _vec3 m_vBoundingBoxScale = { 0.f,0.f,0.f };


// 파티클 갯수 설정 ============================
protected:
    int m_iParticleNum = 20;
    int m_iMaxParticleNum = 20;


// 파티클 타입 설정 ============================
protected:
    PARTICLE_TYPE m_eParticleType = PARTICLE_TYPE_TEST;


// 입자 텍스쳐 설정 ============================
protected:
    int m_iParticleTextureIndex_X = 0;
    int m_iParticleTextureIndex_Y = 0;

// 입자 크기 (일정할때) ========================
protected:
    float m_fParticleSize = 0.1;


// 입자 크기 일정 or 랜덤 ======================
protected:
    bool m_bIsParticleRandomSize = false;


protected:
	float m_fParticleTimer = 0.f;

protected:
    virtual void  Free();
};

END



