#include "pch.h"
#include "CPlayer.h"
#include "CRendererManager.h"
#include "CInputManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CDebugLineHelper.h"
#include "CUI_NoticeBanner.h"
#include "CUIManager.h"
#include "CPauseUI.h"
#include "CEventBus.h"
#include "CSoundManager.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev) ,  m_PlayerDefaultY(0.f)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	:CGameObject(rhs)
{

}

CPlayer::~CPlayer()
{

}

HRESULT	CPlayer::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSpeed = 20.f;
	m_pTransform->Set_Pos(0.f, 2.f, 0.f);
	m_tHandle.m_eType = OBJ_PLAYER;

	STATUS stat = { 1000.f, 500.f, 10.f, 0.f, 3.f };
	m_pStatusCom->Set_Status(stat);
	m_pWalletCom->Set_Money(30000);
	m_pColliderCom->Set_ColliderScale(_vec3{ 1.f,1.f,1.f });

	m_prevPos = *m_pTransform->GetInfoPos();
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	if (!m_sentStageFail) {
		const float hp = m_pStatusCom->Get_CurrentHp();
		if (hp <= 0.f) {
			EVENT event{
				PHASE::PHASE_BEGIN,
				TOPIC::Stage_Fail,
				CHANNEL::UI,
				nullptr,
				nullptr,
				{},
				{}
			};
			CEventBus::GetInstance()->publish(event);
			m_sentStageFail = true;
		}
	}


	_vec3 vPos = *m_pTransform->GetInfoPos();

	m_pTransform->Set_Pos(vPos.x , m_PlayerDefaultY , vPos.z);

	// 디버그용
	//CDebugLineHelper::GetInstance()->AxisLine_Helper(m_pTransform);
	return 0;
}

void CPlayer::LateUpdate_GameObject(const _float& dt)
{
	CGameObject::LateUpdate_GameObject(dt);

	if (Engine::CUI::IsPaused()) {

		CSoundManager::GetInstance()->SoundStop(PLAYER_SOUND);
		m_footTimer = 0.f;
		m_lastStepIdx = -1;
		return;
	}

	if (!CInputManager::GetInstance()->Get_KeyLock()) {
		Key_Input(dt);
		Mouse_Move(dt);
		CInputManager::GetInstance()->Mouse_Fix();
	}

	_vec3 cur = *m_pTransform->GetInfoPos();
	m_pTransform->Set_Pos(cur.x, m_PlayerDefaultY, cur.z);

	float dx = cur.x - m_prevPos.x;
	float dz = cur.z - m_prevPos.z;
	float dist2 = dx * dx + dz * dz;

	const float minMove2 = 1e-4f;
	bool moving = (dist2 > minMove2);

	auto sm = CSoundManager::GetInstance();

	if (moving) {
		float speed = sqrtf(dist2) / max(dt, 1e-4f);
		float interval = 1.3f * max(0.28f, min(0.55f, 0.45f * (3.0f / max(speed, 0.5f))));

		m_footTimer += dt;
		if (m_footTimer >= interval) {
			m_footTimer = 0.f;

			int idx; 
			do { idx = rand() % 4; } 
			while (idx == m_lastStepIdx);
			m_lastStepIdx = idx;

			static const wchar_t* keys[4] = {
				L"Heavy Running  Dirt footsteps 1.wav",
				L"Heavy Running  Dirt footsteps 2.wav",
				L"Heavy Running  Dirt footsteps 3.wav",
				L"Heavy Running  Dirt footsteps 4.wav"
			};

			sm->SoundPlay(keys[idx], PLAYER_SOUND, sm->GetVol().sfx * 0.2f, false);
		}
	}
	else {
		m_footTimer = 0.f;
	}

	m_prevPos = cur;
}


void CPlayer::Render_GameObject()
{
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = NULL;

	//================ 버퍼 컴포넌트 ===============

	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_RECTEX, CRecTex);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

	//================ 트랜스폼 컴포넌트 ===============

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//================ 콜라이더 충돌 컴포넌트 ===============

	pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_SPHERE, CSphere);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });

	// =============== 스테이터스 컴포넌트 ===============


	pComponent = m_pStatusCom = CLONE_PROTOTYPE(PROTO_STATUS, CStatus);

	if (nullptr == pComponent)
		return E_FAIL;


	m_pStatusCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Status",pComponent });

	// =============== 무기 컴포넌트 ====================

	pComponent = m_pWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWeaponCom->Set_MyOwner(this);
	m_pWeaponCom->Weapon_Inintialize();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon",pComponent });

	// ============== 지갑 컴포넌트 ==================== 

	pComponent = m_pWalletCom = CLONE_PROTOTYPE(PROTO_WALLET, CWallet);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWalletCom->Set_MyOwner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Wallet",pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	auto playerSpeed = m_pStatusCom->GetSpeed();
	// 회전한 상태의 축을 가져온다??
	// 회전한 y축 방향대로 나아가면 되니까??
	_vec3 vLook;
	m_pTransform->Get_Info(INFO_LOOK, &vLook);

	_vec3 vRight;
	m_pTransform->Get_Info(INFO_RIGHT, &vRight);

	vLook.y = 0.0f;
	D3DXVec3Normalize(&vLook, &vLook);

	vRight.y = 0.0f;
	D3DXVec3Normalize(&vRight, &vRight);

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::W))
	{
		// 회전된 내 local Y축 방향으로 내가 움직이는데 그걸 단위벡터로 만들어서
		// 방향값만 가져오고 내 speed만큼 움직이겠다.
		m_pTransform->Move_Pos(&vLook, playerSpeed, fTimeDelta);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		// 뒤로도 가보자.
		m_pTransform->Move_Pos(&vLook, -playerSpeed, fTimeDelta);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransform->Move_Pos(&vRight, -playerSpeed, fTimeDelta);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransform->Move_Pos(&vRight, playerSpeed, fTimeDelta);
	}

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LBUTTON))
	{
		/*	CCollisionManager::GetInstance()->FireRaycast(GUN_RAY);*/
		m_pWeaponCom->Fire();
	}

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::RBUTTON))
	{
		/*	CCollisionManager::GetInstance()->FireRaycast(GUN_RAY);*/
		m_pWeaponCom->ChargeFire(fTimeDelta);
	}

	if (CInputManager::GetInstance()->GetButtonUp(KEY_TYPE::RBUTTON))
	{
		/*	CCollisionManager::GetInstance()->FireRaycast(GUN_RAY);*/
		m_pWeaponCom->Overdrive_Fire();
	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pPlayer = new CPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pPlayer;
}

void CPlayer::Free()
{
	Engine::CGameObject::Free();
}


void CPlayer::Mouse_Move(const _float& dt)
{
	//마우스 이동량 읽기
	LONG dx = CInputManager::GetInstance()->Get_DIMouseMove(DIMS_X);
	LONG dy = CInputManager::GetInstance()->Get_DIMouseMove(DIMS_Y);

	//지금 트랜스폼 회전각 가져오기
	_vec3 angRad = m_pTransform->Get_Angle();
	float pitchDeg = D3DXToDegree(angRad.x);
	float yawDeg = D3DXToDegree(angRad.y);
	float rollRad = angRad.z;

	// 이동량값만큼 상하좌우 누적값 업뎃
	yawDeg += dx * 0.1f;
	pitchDeg += dy * 0.1f;

	//누적값 -80/80넘으면 컷 
	if (pitchDeg < -80.f) pitchDeg = -80.f;
	if (pitchDeg > 80.f) pitchDeg = 80.f;

	//위에서ok받으면 그거 트랜스폼에 직접 갖다박음
	m_pTransform->Set_Angle( D3DXToRadian(pitchDeg), D3DXToRadian(yawDeg), rollRad );
}
