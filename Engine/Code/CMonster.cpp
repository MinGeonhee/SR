#include "CMonster.h"
#include "CBillboard.h"
#include "CObjectManager.h"
#include "CMonsterSystem.h"
#include "CSoundManager.h"
#include "CTimeManager.h"
#include "CActionCamera_Helper.h"
#include "CSceneManager.h"

static DWORD s_globalNextMoveVocalMs = 0;           // 전역 간격
static map<wstring, DWORD> s_nextMsByKey; // 키별 간격

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CMonster::CMonster(const CMonster& rhs)
	:CGameObject(rhs)
{

}

CMonster::~CMonster()
{

}

HRESULT	CMonster::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_MONSTER;
	m_eMonsterStatus = MONSTER_STATUS_IDLE;
	m_ePrevMonsterStatus = MONSTER_STATUS_END;
	m_vocalHearingMax = 25.0f;

	STATUS st = { 60.f,60.f, 10.f, 3.f, 2.f };	// MaxHp, CurHP, Attack, Defence, Speed
	m_pStatusCom->Set_Status(st);

	Set_Target(CObjectManager::GetInstance()->Get(OBJ_PLAYER));

	m_vocalTimer = ((float)rand() / (float)RAND_MAX) * ((3.0f + 6.0f) * 0.5f - 3.0f) + 3.0f;

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vPos = *this->Get_Transform()->GetInfoPos();
	CGameObject::Compute_ViewZ(&vPos);

	m_canPlay = true;
	if (auto pTarget = Get_Target()) {
		const _vec3 my = *Get_Transform()->GetInfoPos();
		const _vec3 ta = *pTarget->Get_Transform()->GetInfoPos();
		const _vec3 d = { ta.x - my.x, ta.y - my.y, ta.z - my.z };
		const float distSq = d.x * d.x + d.y * d.y + d.z * d.z;
		if (distSq > m_vocalHearingMax * m_vocalHearingMax)
			m_canPlay = false;
	}

	if (m_eMonsterStatus != MONSTER_STATUS::MONSTER_STATUS_DEAD &&
		m_eMonsterStatus != MONSTER_STATUS::MONSTER_STATUS_DYING)
	{
		MoveSound(fTimeDelta);
	}

	return 0;
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonster::Render_GameObject()
{

}

HRESULT CMonster::Add_Component()
{
	CComponent* pComponent = NULL;

	//================ 트랜스폼 컴포넌트 ===============
	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//================ 애니메이션 컴포넌트 ===============

	pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

	//================ 콜라이더 AAABBB 충돌 컴포넌트 ===============
	pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_SPHERE, CSphere);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });

	// =============== 빌보드 컴포넌트 ==================

	pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBillboardCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });


	// =============== 스테이터스 컴포넌트 ===============

	pComponent = m_pStatusCom = CLONE_PROTOTYPE(PROTO_STATUS, CStatus);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pStatusCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Status", pComponent });

	return S_OK;
}


void CMonster::Free()
{

	Engine::CGameObject::Free();
}

void CMonster::Hit()
{
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DEAD) return;
	m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_HIT;

	const wchar_t* soundKey = nullptr;
	switch (this->Get_OBJTYPE())
	{
	case OBJ_MONSTER_GOBLIN_FIREBALL: soundKey = L"goblin-caster-pain-1.wav"; break;
	case OBJ_MONSTER_GOBLIN_HAMMER:   soundKey = L"goblin-pain-1.wav";        break;
	case OBJ_MONSTER_ORGE_AXE:        soundKey = L"ogre-pain-1.wav";          break;
	case OBJ_MONSTER_RAT:             soundKey = L"rat-hit-2.wav";            break;
	case OBJ_MONSTER_SKELETON_SWORD:  soundKey = L"skeleton-hurt-2.wav";      break;
	case OBJ_MONSTER_WIZARD:          soundKey = L"lich-hurt-1.wav";          break;
	default:                          soundKey = L"goblin-pain-1.wav";        break;
	}

	if (soundKey && *soundKey && m_canPlay) {
		CSoundManager::GetInstance()->SoundPlay(soundKey, MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
	}

	if (m_pStatusCom->Get_CurrentHp() <= 0) {
		Die();
		CMonsterSystem::GetInstance()->AddMonsterKill();

		if(m_tHandle.m_eType == OBJ_MONSTER_BOSS || m_tHandle.m_eType == OBJ_MONSTER_WIZARD)
		CActionCamera_Helper::PlayDeathSequence(this, m_tHandle); // 시네마틱 카메라 장착.

		if (m_tHandle.m_eType == OBJ_MONSTER_BOSS)
		{
			EVENT event1 = { PHASE::PHASE_BEGIN, TOPIC::BOSS_CLEAR, CHANNEL::UI };
			CTimeManager::GetInstance()->TimeEventJob_Resist(
				event1,
				TimeType::TIME_NONE,
				/*Duration*/15.f,
				/*TimeScale*/1.f);
		}
	}
}

void CMonster::Die()
{
	m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_DYING;

	const wchar_t* soundKey = nullptr;
	switch (this->Get_OBJTYPE())
	{
	case OBJ_MONSTER_GOBLIN_FIREBALL: soundKey = L"goblin-caster-die.wav"; break;
	case OBJ_MONSTER_GOBLIN_HAMMER:   soundKey = L"goblin-die.wav";        break;
	case OBJ_MONSTER_ORGE_AXE:        soundKey = L"ogre-death.wav";        break;
	case OBJ_MONSTER_RAT:             soundKey = L"rat-die.wav";           break;
	case OBJ_MONSTER_SKELETON_SWORD:  soundKey = L"skeleton-die.wav";      break;
	case OBJ_MONSTER_WIZARD:          soundKey = L"lich-die.wav";          break;
	case OBJ_MONSTER_BOSS:			  soundKey = L"inferno-die.wav";       break;
	default:                          soundKey = L"inferno-die.wav";       break;
	}

	if (soundKey && *soundKey && m_canPlay) {
		CSoundManager::GetInstance()->SoundPlay(soundKey, MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
	}
}

void CMonster::MoveSound(float fTimeDelta)
{
	m_vocalTimer -= fTimeDelta;
	if (m_vocalTimer > 0.f) return;

	if (!m_canPlay) {
		m_vocalTimer = ((float)rand() / (float)RAND_MAX) * 10.0f + 10.0f; // [10,20)
		return;
	}

	const wchar_t* soundKey = nullptr;
	switch (this->Get_OBJTYPE())
	{
	case OBJ_MONSTER_GOBLIN_FIREBALL:
	case OBJ_MONSTER_GOBLIN_HAMMER:  soundKey = L"goblin-annoyed.wav";   break;
	case OBJ_MONSTER_ORGE_AXE:       soundKey = L"ogre-annoyed.wav";     break;
	case OBJ_MONSTER_RAT:            soundKey = L"rat-aware.wav";        break;
	case OBJ_MONSTER_SKELETON_SWORD: soundKey = L"skeleton-walk-humanoid-light-armor.wav"; break;
	case OBJ_MONSTER_WIZARD:         break;
	default: break;
	}

	if (!(soundKey && *soundKey)) {
		m_vocalTimer = ((float)rand() / (float)RAND_MAX) * 10.0f + 10.0f;
		return;
	}

	DWORD now = GetTickCount64();
	DWORD& keyNext = s_nextMsByKey[soundKey];

	if (now < s_globalNextMoveVocalMs || now < keyNext) {
		m_vocalTimer = ((float)rand() / (float)RAND_MAX) * 4.0f + 3.0f;
		return;
	}

	CSoundManager::GetInstance()->SoundPlay(
		soundKey, MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 0.5f);

	const float globalMs = 250.f;
	const float keyMs = 1200.f + 300.f * ((float)rand() / (float)RAND_MAX);
	s_globalNextMoveVocalMs = now + (DWORD)globalMs;
	keyNext = now + (DWORD)keyMs;

	m_vocalTimer = ((float)rand() / (float)RAND_MAX) * 10.0f + 10.0f;
}

