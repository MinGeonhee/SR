#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

class CParticleMgr :
    public CBase
{
    DECLARE_SINGLETON(CParticleMgr)

private:
    explicit CParticleMgr();
    virtual ~CParticleMgr();

// 1. 바운딩 박스 크기
private:
    _vec3 m_vBoundingBoxScale = {0.f,0.f,0.f};
public:
	void Set_BoundingBoxScale(const _vec3& vScale) { m_vBoundingBoxScale = vScale; }
	_vec3 Get_BoundingBoxScale() const { return m_vBoundingBoxScale; }

// 2. 파티클 타입 설정
public:
	enum PARTICLE_TYPE{
        PARTICLE_TYPE_TEST = 0,
        PARTICLE_TYPE_SPARKLE,
        PARTICLE_TYPE_GO_UP,
        PARTICLE_TYPE_DROP,
        PARTICLE_TYPE_FLIES,
        
		PARTICLE_TYPE_END
	};
private:
	PARTICLE_TYPE m_eParticleType = PARTICLE_TYPE_TEST;
    bool m_bIsParticleTypeChanged = false;
public:
	void Set_ParticleType(const PARTICLE_TYPE& eType) { m_eParticleType = eType; }
	PARTICLE_TYPE Get_ParticleType() const { return m_eParticleType; }
    
    void Set_IsParticleTypeChanged(const bool& b) { m_bIsParticleTypeChanged = b; }
    bool Get_IsParticleTypeChanged() const { return m_bIsParticleTypeChanged; }

// 3. 입자 텍스쳐 설정
private:
    int m_iParticleTextureIndex_X = 0;
    int m_iParticleTextureIndex_Y = 0;
	bool m_bIsTextureIndexChanged = false; 
public:
	void Set_ParticleTextureIndex(const int& iX, const int& iY)
	{
		m_iParticleTextureIndex_X = iX;
		m_iParticleTextureIndex_Y = iY;
	}
	int Get_ParticleTextureIndex_X() const { return m_iParticleTextureIndex_X; }
	int Get_ParticleTextureIndex_Y() const { return m_iParticleTextureIndex_Y; }
	
    void Set_IsTextureIndexChanged(const bool& b) { m_bIsTextureIndexChanged = b; }
	bool Get_IsTextureIndexChanged() const { return m_bIsTextureIndexChanged; }

// 4. 입자 갯수
private:
    int m_iParticleNum = 1;
public:
    void Set_ParticleNum(const int& fNum) { m_iParticleNum = fNum; }
    int Get_ParticleNum() const { return m_iParticleNum; }


// 5. 입자 크기 일정 or 랜덤
private:
    bool m_bIsParticleRandomSize = false;
public:
    void Set_IsParticleRandomSize(const bool& b) { m_bIsParticleRandomSize = b; }
    bool Get_IsParticleRandomSize() const { return m_bIsParticleRandomSize; }


// 6. 입자 크기 (일정할때)
private:
    float m_fParticleSize = 0.1;
public:
    void  Set_ParticleSize(const float& fNum) { m_fParticleSize = fNum; }
    float Get_ParticleSize() const { return m_fParticleSize; }

// 7. 고 업 타입일 때 올라가는 속도
private:
	float m_fSpeed_GoUp = 1.6f; // 올라가는 속도
public:
	void Set_Speed_GoUp(const float& fSpeed) { m_fSpeed_GoUp = fSpeed; }
	float Get_Speed_GoUp() const { return m_fSpeed_GoUp; }
    
// 8. 드롭 타입일 때 드롭 속도 및 중력 가속도
private:
	float m_fDropSpeed_XZ = 0.6f; // XZ 평면에서의 속도
	float m_fDropSpeed_Y = 7.f;   // Y축 방향 속도
	float m_fDropGravity = 9.8f;  // 중력 가속도
public:
	void Set_DropSpeed_XZ(const float& fSpeed) { m_fDropSpeed_XZ = fSpeed; }
	float Get_DropSpeed_XZ() const { return m_fDropSpeed_XZ; }
	void Set_DropSpeed_Y(const float& fSpeed) { m_fDropSpeed_Y = fSpeed; }
	float Get_DropSpeed_Y() const { return m_fDropSpeed_Y; }
	void Set_DropGravity(const float& fGravity) { m_fDropGravity = fGravity; }
	float Get_DropGravity() const { return m_fDropGravity; }

// 10. 세팅값 적용 트리거
private:
    bool m_bApplySettings = false;
public:
    void Set_ApplySettings(const bool& b) { m_bApplySettings = b; }
    bool Get_ApplySettings() const { return m_bApplySettings; }


private:
    virtual void                Free();
};

