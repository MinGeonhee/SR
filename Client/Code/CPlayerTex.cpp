#include "pch.h"
#include "CPlayerTex.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CWeaponController.h"
#include "CCombatSystem.h"
#include "CEventbus.h"
#include "CUIManager.h"
#include "CCollisionManager.h"
//대기 모션
static inline void Idle( const _vec3& basePos, float t, float speed, const _vec3& amp, float rotAmp, float phase, _vec3& outPos, float& outRotZ)
{
	const float a = t * speed + phase;
	const float sx = sinf(a);
	const float cy = cosf(a);
	outPos = basePos + _vec3(amp.x * sx, amp.y * cy, 0.f);
	outRotZ = rotAmp * sx; 
}

static inline void Walking(const _vec3& basePos, float t, float speed,
	const _vec3& amp, float phase,
	_vec3& outPos)
{
	const float a = t * speed + phase;
	const float s = sinf(a);

	const float x = amp.x * s;
	const float y = amp.y * (-0.5f * cosf(2.f * a));

	outPos = basePos + _vec3(x, y, 0.f);
}


CPlayerTex::CPlayerTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CPlayerTex::CPlayerTex(const CPlayerTex& rhs)
	:CUI(rhs)
{

}

CPlayerTex::~CPlayerTex()
{

}

HRESULT	CPlayerTex::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = true;
	m_tHandle.m_eType = OBJ_UI;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_pWeaponCon = pPlayer->Get_WeaponController();
	m_eWeaponState = m_pWeaponCon->Get_WeaponType();
	m_prevWeaponState = m_eWeaponState;

	m_pTransform->Set_Pos(0.f, -200.f, 0.f);
	return S_OK;
}

_int CPlayerTex::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return 0;
	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CPlayerTex::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return;
	if (fTimeDelta == 0.f) return;
	CUI::LateUpdate_GameObject(fTimeDelta);

	const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"PlayerTex");
	if (tag != m_cachedTag) {
		m_cachedTag = tag;
		if (tag == L"Clear" && m_status != PlayerStatus::CLEAR) {
			m_status = PlayerStatus::CLEAR;
			m_clearEventFired = false;
		}
	}

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_pWeaponCon = pPlayer->Get_WeaponController();
	m_eWeaponState = m_pWeaponCon->Get_WeaponType();
	m_iPlayerBul = pPlayer->Get_WeaponController()->Get_Bullet();

	if (m_status != m_prevStatus)
	{
		if ((m_prevStatus == PlayerStatus::IDLE || m_prevStatus == PlayerStatus::WALKING)
			&& m_status == PlayerStatus::CLEAR)
			ResetWeaponSprite();
		m_prevStatus = m_status;
	}

	if (m_status == PlayerStatus::CLEAR)
	{
		Clear_Action(fTimeDelta);
		return;
	}
	else if (m_status == PlayerStatus::DEAD)
	{
		Dead_Action(fTimeDelta);
		return;
	}

	isMoving = false;

	{
		auto in = CInputManager::GetInstance();
		isMoving = in->GetButtonPress(KEY_TYPE::W) || in->GetButtonPress(KEY_TYPE::A)
			|| in->GetButtonPress(KEY_TYPE::S) || in->GetButtonPress(KEY_TYPE::D);
	}

	if (CInputManager::GetInstance()->Get_KeyLock())
	{
		isMoving = false;
		return;
	}

	if (isMoving && m_eMoveState == MoveState::NONE && m_eLeftState == MoveState::NONE && m_eRightState == MoveState::NONE)
	{
		m_status = PlayerStatus::WALKING;
	}
	else
	{
		m_status = PlayerStatus::IDLE;
	}

	m_idleT += fTimeDelta;
	if (m_status == PlayerStatus::WALKING) m_walkT += fTimeDelta;
	else        
		m_walkT = 0.f;

	if (m_status != m_prevStatus)
	{
		if (m_prevStatus == PlayerStatus::CLEAR && m_status == PlayerStatus::IDLE)
			ResetWeaponSprite();

		if (m_prevStatus == PlayerStatus::DEAD && m_status == PlayerStatus::IDLE)
			ResetWeaponSprite();

		m_prevStatus = m_status;
	}

	if (m_eWeaponState != m_prevWeaponState)
	{
		ResetWeaponSprite();
		m_prevWeaponState = m_eWeaponState;
	}

	switch (m_eWeaponState)
	{
	case WEAPON_MACE:
		Mace_Action(fTimeDelta);
		break;
	case WEAPON_ICEWAND:
		Ice_Action(fTimeDelta);
		break;
	case WEAPON_FIRERING:
		FireBall_Action(fTimeDelta);
		break;
	case WEAPON_GUN:
		Gun_Action(fTimeDelta);
		break;
	case WEAPON_FIREWAND:
		FireWand_Action(fTimeDelta);
		break;
	}
}

void CPlayerTex::Render_GameObject()
{	
	if (!Is_UIActive()) return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	CResourceManager::GetInstance()->Get<CTexture>(L"Player_Weapon")->Set_Texture();

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	if (m_eWeaponState == WEAPON_FIRERING || m_eWeaponState == WEAPON_GUN || m_eWeaponState == WEAPON_ICEWAND)
	{
		D3DXMATRIX TR, SR, WR, RR;
		D3DXMatrixScaling(&SR, 0.8f, 1.8f, 1.f);
		D3DXMatrixRotationZ(&RR, m_fRightRot);
		D3DXMatrixTranslation(&TR, m_vRightOfs.x, m_vRightOfs.y, m_vRightOfs.z);
		WR = (SR * RR * TR) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &WR);
		m_pBufferCom->Change_UV(m_iRightUVX + m_iUVXOffset, 8, m_iUVY, 7);
		m_pBufferCom->Render_Buffer();

		D3DXMATRIX TL, SL, WL, RL;
		D3DXMatrixScaling(&SL, 0.8f, 1.8f, 1.f );
		D3DXMatrixRotationZ(&RL, m_fLeftRot);
		D3DXMatrixTranslation(&TL, m_vLeftOfs.x, m_vLeftOfs.y, m_vLeftOfs.z);
		WL = (SL * RL * TL) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &WL);
		m_pBufferCom->Change_UV(m_iLeftUVX, 8, m_iUVY, 7);
		m_pBufferCom->Render_Buffer();
	}
	else
	{
		D3DXMATRIX T, S, W, R;
		D3DXMatrixScaling(&S, m_vScaleOfs.x, m_vScaleOfs.y, m_vScaleOfs.z);
		D3DXMatrixRotationZ(&R, m_fLeftRot);
		D3DXMatrixTranslation(&T, m_vOfs.x, m_vOfs.y, m_vOfs.z);
		W = (S * R * T) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
		m_pBufferCom->Change_UV(m_iUVX, 8, m_iUVY, 7);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CPlayerTex* CPlayerTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerTex* pPlayerTex = new CPlayerTex(pGraphicDev);

	if (FAILED(pPlayerTex->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pPlayerTex);
		MSG_BOX("pPlayerTex Create Failed");
		return nullptr;
	}

	return pPlayerTex;
}

void CPlayerTex::Free()
{
	CUI::Free();
}

void CPlayerTex::Mace_Action(const _float& fTimeDelta)
{
	m_vScaleOfs = { 1.1f, 1.9f, 1.f };
	_vec3 vLeftTargetPos = { -300.f, -100.f, m_vOfs.z };
	_vec3 vRightTargetPos = { 550.f, 400.f, m_vOfs.z };
	_vec3 vOriginalTargetPos = { 300.f, 0.f, m_vOfs.z };
	_vec3 vTemp;

	m_iUVY = 1;

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LBUTTON) && m_eMoveState == MoveState::NONE )
	{
		bChargeAttack = false;
		m_eMoveState = MoveState::TO_RIGHT;
	}
	else if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::RBUTTON) && m_eMoveState == MoveState::NONE )
	{
		bChargeAttack = true;
		m_eMoveState = MoveState::TO_RIGHT;
	}

	if (bChargeAttack == false)
	{
		switch (m_eMoveState)
		{
		case MoveState::NONE:
		{
			_vec3 base = vOriginalTargetPos;
			if (m_status == PlayerStatus::WALKING)
			{
				Walking(base, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vOfs);
				m_fLeftRot = 0.f;
			}
			else {
				float rotZ;
				Idle(base, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, 0.f, m_vOfs, rotZ);
				m_fLeftRot = rotZ;    
			}
			break;
		}
		case MoveState::TO_RIGHT:
			vDir = vRightTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);
			m_vOfs += vDir * 7000.f * fTimeDelta;

			vTemp = vRightTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 35.f)
			{
				m_eMoveState = MoveState::TO_LEFT;
			}
			break;

		case MoveState::TO_LEFT:
			vDir = vLeftTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);
			m_vOfs += vDir * 8000.f * fTimeDelta;

			vTemp = vLeftTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 45.f)
			{
				m_eMoveState = MoveState::TO_ORIGIN;
				CCollisionManager::GetInstance()->FireRaycast(MACE_RAY);
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vOriginalTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);
			m_vOfs += vDir * 2000.f * fTimeDelta;

			vTemp = vOriginalTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 20.f)
			{
				m_eMoveState = MoveState::NONE;
			}
			break;
		}
	}
	else
	{
		switch (m_eMoveState)
		{
		case MoveState::NONE:
		{
			_vec3 base = vOriginalTargetPos;
			if (m_status == PlayerStatus::WALKING)
				Walking(base, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vOfs);
			else {
				m_vOfs = base;
				m_fLeftRot = 0.f;
			}
			break;
		}
		case MoveState::TO_RIGHT:
			vDir = vRightTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);
			m_vOfs += vDir * 7000.f * fTimeDelta;

			vTemp = vRightTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 35.f)
			{
				m_vOfs = vRightTargetPos;
				if(CInputManager::GetInstance()->GetButtonUp(KEY_TYPE::RBUTTON))
				m_eMoveState = MoveState::TO_LEFT;
			}
			break;

		case MoveState::TO_LEFT:
			vDir = vLeftTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);
			m_vOfs += vDir * 8000.f * fTimeDelta;

			vTemp = vLeftTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 45.f)
			{
				m_eMoveState = MoveState::TO_ORIGIN;
				//m_iUVX = 1;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vOriginalTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);
			m_vOfs += vDir * 2000.f * fTimeDelta;

			vTemp = vOriginalTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 20.f)
			{
				m_eMoveState = MoveState::NONE;
			}
			break;
		}
	}
}

void CPlayerTex::Ice_Action(const _float& fTimeDelta)
{
	_vec3 vOriginalTargetPos = { 0.f, 0.f, 0.f };
	_vec3 vDownTargetPos = { 0.f, -150.f,0.f };
	_vec3 vTemp;
	_vec3 vROriginalTargetPos = { 400.f, -500.f, m_vRightOfs.z };
	_vec3 vRUpTargetPos = { 400.f, 150.f, m_vRightOfs.z };
	_vec3 vRDownTargetPos = { 400.f, 0.f, m_vRightOfs.z };
	_vec3 vRTemp;
	_vec3 vInvisiblePos = { m_vLeftOfs.x, -250.f, m_vRightOfs.z };

	m_iLeftUVX = 0;
	m_iUVY = 0;
	m_iUVXOffset = 3;

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LBUTTON) && m_eLeftState == MoveState::NONE && m_eRightState == MoveState::NONE && m_iPlayerBul > 0)
	{
		bChargeAttack = false;
		m_eLeftState = MoveState::TO_DOWN;
	}
	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::RBUTTON) && m_eLeftState == MoveState::NONE && m_eRightState == MoveState::NONE)
	{
		if (CCombatSystem::GetInstance()->HasFrozenObject())
		{
			bChargeAttack = true;
			m_eLeftState = MoveState::TO_DOWN;
		}
	}
	if (!bChargeAttack)
		switch (m_eLeftState)
		{
		case MoveState::NONE:
		{
			if (m_status == PlayerStatus::WALKING) {
				_vec3 baseL = vOriginalTargetPos;
				Walking(baseL, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vLeftOfs);
				m_fLeftRot = 0.f;
			}
			else {
				m_vLeftOfs = vOriginalTargetPos;
				m_fLeftRot = 0.f;
			}
			break;
		}

		case MoveState::TO_DOWN:
			vDir = vDownTargetPos - m_vLeftOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vLeftOfs += vDir * 1000.f * fTimeDelta;

			m_iLeftUVX = 1;
			vTemp = vDownTargetPos - m_vLeftOfs;
			if (D3DXVec3Length(&vTemp) < 55.f) {
				m_iLeftUVX = 2;
				m_eLeftState = MoveState::TO_ORIGIN;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vOriginalTargetPos - m_vLeftOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vLeftOfs += vDir * 1000.f * fTimeDelta;

			vTemp = vOriginalTargetPos - m_vLeftOfs;
			if (D3DXVec3Length(&vTemp) < 20.f) {
				m_vLeftOfs.y = 0.f;
				m_iLeftUVX = 0;
				m_eLeftState = MoveState::NONE;
			}
			break;
		}
	if (bChargeAttack)
	{
		switch (m_eLeftState)
		{
		case MoveState::NONE:
		{
			if (m_status == PlayerStatus::WALKING) {
				_vec3 baseL = vOriginalTargetPos;
				Walking(baseL, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vLeftOfs);
				m_fLeftRot = 0.f;
			}
			else {
				m_vLeftOfs = vOriginalTargetPos;
				m_fLeftRot = 0.f;
			}
			break;
		}

		case MoveState::TO_DOWN:
			vDir = vInvisiblePos - m_vLeftOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vLeftOfs += vDir * 1000.f * fTimeDelta;

			vTemp = vInvisiblePos - m_vLeftOfs;
			if (D3DXVec3Length(&vTemp) < 55.f) {
				if (m_eRightState == NONE)
					m_eRightState = MoveState::TO_UP;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vOriginalTargetPos - m_vLeftOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vLeftOfs += vDir * 1000.f * fTimeDelta;

			vTemp = vOriginalTargetPos - m_vLeftOfs;
			if (D3DXVec3Length(&vTemp) < 20.f) {
				m_vLeftOfs.y = 0.f;
				m_iLeftUVX = 0;
				m_eLeftState = MoveState::NONE;
			}
			break;
		}
	}
	switch (m_eRightState)
	{
	case MoveState::NONE:
		HoldTimer = 0;
		m_iRightUVX = 1;
		m_vRightOfs = vROriginalTargetPos;
		break;

	case MoveState::TO_UP:
		vDir = vRUpTargetPos - m_vRightOfs;
		D3DXVec3Normalize(&vDir, &vDir);

		m_vRightOfs += vDir * 2000.f * fTimeDelta;
		vTemp = vRUpTargetPos - m_vRightOfs;
		if (D3DXVec3Length(&vTemp) < 20.f) {
			m_eRightState = MoveState::TO_DOWN;
		}
		break;

	case MoveState::TO_DOWN:
		vDir = vRDownTargetPos - m_vRightOfs;
		D3DXVec3Normalize(&vDir, &vDir);

		m_vRightOfs += vDir * 2000.f * fTimeDelta;

		vTemp = vRDownTargetPos - m_vRightOfs;
		if (D3DXVec3Length(&vTemp) < 20.f) {
			m_eRightState = MoveState::HOLD;
		}
		break;

	case MoveState::HOLD:
		m_iRightUVX = 0;
		HoldTimer += fTimeDelta;
		if (HoldTimer >= kHoldDuration) {
			m_eRightState = MoveState::TO_ORIGIN;
		}
		break;

	case MoveState::TO_ORIGIN:
		vDir = vROriginalTargetPos - m_vRightOfs;
		D3DXVec3Normalize(&vDir, &vDir);

		m_vRightOfs += vDir * 700.f * fTimeDelta;

		vTemp = vROriginalTargetPos - m_vRightOfs;
		if (D3DXVec3Length(&vTemp) < 20.f) {
			m_eRightState = MoveState::NONE;
			m_eLeftState = MoveState::TO_ORIGIN;
		}
		break;
	}
}

void CPlayerTex::FireBall_Action(const _float& fTimeDelta)
{
	m_iLeftUVX = 0; // 왼손 프레임
	m_iRightUVX = 0; // 오른손 프레임
	m_iUVY = 2;
	m_iUVXOffset = 4;

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LBUTTON) && m_iPlayerBul > 0)
	{
		if (m_bLeftHandOn && m_eLeftState == MoveState::NONE)
		{
			m_eLeftState = MoveState::TO_UP;
			m_bLeftHandOn = false;
		}
		else if (!m_bLeftHandOn && m_eRightState == MoveState::NONE)
		{
			m_eRightState = MoveState::TO_UP;
			m_bLeftHandOn = true;
		}
	}

	_vec3 vROriginalTargetPos = { 400.f, 0.f, m_vRightOfs.z };
	_vec3 vRUpTargetPos = { 300.f, 150.f, m_vRightOfs.z };
	_vec3 vRTemp;
	_vec3 vLOriginalTargetPos = { -400.f, 0.f, m_vLeftOfs.z };
	_vec3 vLUpTargetPos = { -300.f, 150.f, m_vLeftOfs.z };
	_vec3 vLTemp;

	if (m_bLeftHandOn)
	{
		switch (m_eLeftState)
		{
		case MoveState::TO_UP:
			vDir = vLUpTargetPos - m_vLeftOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vLeftOfs += vDir * 1000.f * fTimeDelta;

			m_iLeftUVX = 3;
			vLTemp = vLUpTargetPos - m_vLeftOfs;
			if (D3DXVec3Length(&vLTemp) < 35.f) {
				m_iLeftUVX = 4;
				m_eLeftState = MoveState::TO_ORIGIN;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vLOriginalTargetPos - m_vLeftOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vLeftOfs += vDir * 1000.f * fTimeDelta;

			vLTemp = vLOriginalTargetPos - m_vLeftOfs;
			if (D3DXVec3Length(&vLTemp) < 20.f) {
				m_vLeftOfs.y = 0.f;
				m_iLeftUVX = 0;
				m_eLeftState = MoveState::NONE;
			}
			break;

		case MoveState::NONE:
		{
			{
				_vec3 baseL = vLOriginalTargetPos;
				if (m_status == PlayerStatus::WALKING) {
					Walking(baseL, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vLeftOfs);
					m_fLeftRot = 0.f;
				}
				else {
					float rotZL;
					Idle(baseL, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, 0.f, m_vLeftOfs, rotZL);
					m_fLeftRot = rotZL;
				}
			}
			{
				_vec3 baseR = vROriginalTargetPos;
				if (m_status == PlayerStatus::WALKING) {
					Walking(baseR, m_walkT, m_walkSpeed, m_walkAmp, D3DX_PI, m_vRightOfs);
					m_fRightRot = 0.f;
				}
				else {
					float rotZR;
					Idle(baseR, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, m_idlePhase, m_vRightOfs, rotZR);
					m_fRightRot = rotZR;
				}
			}
			m_iLeftUVX = 0;
			m_iRightUVX = 0;
			break;
		}
		}
	}
	else
	{
		switch (m_eRightState)
		{
		case MoveState::TO_UP:
			vDir = vRUpTargetPos - m_vRightOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vRightOfs += vDir * 1000.f * fTimeDelta;

			m_iRightUVX = 3;
			vRTemp = vRUpTargetPos - m_vRightOfs;
			if (D3DXVec3Length(&vRTemp) < 35.f) {
				m_iRightUVX = 4;
				m_eRightState = MoveState::TO_ORIGIN;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vROriginalTargetPos - m_vRightOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vRightOfs += vDir * 1000.f * fTimeDelta;

			vRTemp = vROriginalTargetPos - m_vRightOfs;
			if (D3DXVec3Length(&vRTemp) < 20.f) {
				m_vRightOfs.y = 0.f;
				m_iRightUVX = 0;
				m_eRightState = MoveState::NONE;
			}
			break;

		case MoveState::NONE:
		{
			{
				_vec3 baseL = vLOriginalTargetPos;
				if (m_status == PlayerStatus::WALKING) {
					Walking(baseL, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vLeftOfs);
					m_fLeftRot = 0.f;
				}
				else {
					float rotZL;
					Idle(baseL, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, 0.f, m_vLeftOfs, rotZL);
					m_fLeftRot = rotZL;
				}
			}
			{
				_vec3 baseR = vROriginalTargetPos;
				if (m_status == PlayerStatus::WALKING) {
					Walking(baseR, m_walkT, m_walkSpeed, m_walkAmp, D3DX_PI, m_vRightOfs);
					m_fRightRot = 0.f;
				}
				else {
					float rotZR;
					Idle(baseR, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, m_idlePhase, m_vRightOfs, rotZR);
					m_fRightRot = rotZR;
				}
			}
			m_iLeftUVX = 0;
			m_iRightUVX = 0;
			break;
		}
		}
	}
}

void CPlayerTex::FireWand_Action(const _float& fTimeDelta)
{
	m_vScaleOfs = { 1.2f, 1.8f, 1.f };
	_vec3 vOriginalTargetPos = { 300.f, -100.f, m_vOfs.z };
	_vec3 vDownTargetPos = { 430.f, -200.f, m_vOfs.z };
	_vec3 vTemp;

	m_iUVX = 0;
	m_iUVY = 4;

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LBUTTON) && m_eMoveState == MoveState::NONE && m_iPlayerBul > 0)
	{
		bChargeAttack = false;
		m_eMoveState = MoveState::TO_DOWN;
	}
	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::RBUTTON) && m_eMoveState == MoveState::NONE && m_iPlayerBul > 0)
	{
		bChargeAttack = true;
		m_eMoveState = MoveState::TO_DOWN;
	}
	if (bChargeAttack == false)
	{
		switch (m_eMoveState)
		{
		case MoveState::NONE:
		{
			_vec3 base = vOriginalTargetPos;
			if (m_status == PlayerStatus::WALKING) {
				Walking(base, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vOfs);
				m_fLeftRot = 0.f;
			}
			else {
				float rotZ;
				Idle(base, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, 0.f, m_vOfs, rotZ);
				m_fLeftRot = rotZ;
			}
			break;
		}
		case MoveState::TO_DOWN:
			vDir = vDownTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vOfs += vDir * 1500.f * fTimeDelta;

			m_iUVX = 1;
			vTemp = vDownTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 35.f) {
				m_eMoveState = MoveState::TO_ORIGIN;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vOriginalTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vOfs += vDir * 1500.f * fTimeDelta;

			vTemp = vOriginalTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 20.f) {
				m_iUVX = 0;
				m_eMoveState = MoveState::NONE;
			}
			break;
		}
	}
	else
	{
		switch (m_eMoveState)
		{
		case MoveState::NONE:
		{
			_vec3 base = vOriginalTargetPos;
			if (m_status == PlayerStatus::WALKING) {
				Walking(base, m_walkT, m_walkSpeed, m_walkAmp, 0.f, m_vOfs);
				m_fLeftRot = 0.f;
			}
			else {
				m_vOfs = base;
				m_fLeftRot = 0.f;
			}
			break;
		}
		case MoveState::TO_DOWN:
			vDir = vDownTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vOfs += vDir * 1500.f * fTimeDelta;

			m_iUVX = 1;
			vTemp = vDownTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 35.f)
			{
				m_vOfs = vDownTargetPos;
				if (CInputManager::GetInstance()->GetButtonUp(KEY_TYPE::RBUTTON))
					m_eMoveState = MoveState::TO_ORIGIN;
			}
			break;

		case MoveState::TO_ORIGIN:
			vDir = vOriginalTargetPos - m_vOfs;
			D3DXVec3Normalize(&vDir, &vDir);

			m_vOfs += vDir * 500.f * fTimeDelta;

			vTemp = vOriginalTargetPos - m_vOfs;
			if (D3DXVec3Length(&vTemp) < 20.f) {
				m_iUVX = 0;
				m_eMoveState = MoveState::NONE;
			}
			break;
		}
	}
}

void CPlayerTex::Gun_Action(const _float& fTimeDelta)
{
	m_iUVY = 3;
	m_iUVXOffset = 2;

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LBUTTON) && m_iPlayerBul > 0 && m_eLeftState == NONE && m_eRightState == NONE)
	{
		m_eRightState = MoveState::TO_UP;
	}

	_vec3 vLOriginalTargetPos = { 0.f, -500.f, m_vLeftOfs.z };
	_vec3 vLUpTargetPos = { 250, 10.f, m_vLeftOfs.z };
	_vec3 vLTemp;

	_vec3 vROriginalTargetPos = { 400, 0.f, m_vRightOfs.z };
	_vec3 vRUpTargetPos = { 450.f, 40.f, m_vRightOfs.z };
	_vec3 vRTemp;

	switch (m_eLeftState)
	{
	case MoveState::TO_UP:
		vDir = vLUpTargetPos - m_vLeftOfs;
		D3DXVec3Normalize(&vDir, &vDir);
		m_iLeftUVX = 1;
		m_iRightUVX = 4;
		m_vLeftOfs += vDir * 800.f * fTimeDelta;
		vLTemp = vLUpTargetPos - m_vLeftOfs;
		if (D3DXVec3Length(&vLTemp) < 35.f) 
		{
			m_iRightUVX = 0;
			m_vRightOfs = vRUpTargetPos;
			m_eLeftState = MoveState::TO_ORIGIN;
		}
		break;

	case MoveState::TO_ORIGIN:
		vDir = vLOriginalTargetPos - m_vLeftOfs;
		D3DXVec3Normalize(&vDir, &vDir);
		m_iLeftUVX = 0;
		m_vLeftOfs += vDir * 800.f * fTimeDelta;
		vLTemp = vLOriginalTargetPos - m_vLeftOfs;
		if (D3DXVec3Length(&vLTemp) < 20.f) {
			m_eLeftState = MoveState::NONE;
			m_eRightState = MoveState::TO_ORIGIN;
		}
		break;

	case MoveState::NONE:
		m_vLeftOfs = vLOriginalTargetPos;
		m_iLeftUVX = 0;
		break;
	}
	

	switch (m_eRightState)
	{
	case MoveState::TO_UP:
	{
		vDir = vRUpTargetPos - m_vRightOfs;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vRightOfs += vDir * 200.f * fTimeDelta;

		_vec3 ab = vRUpTargetPos - vROriginalTargetPos; // 시작점~목표점
		_vec3 ap = m_vRightOfs - vROriginalTargetPos; // 시작점~ㅕㅎㄴ재위치

		float abLen2 = D3DXVec3LengthSq(&ab); //벡터 ab길이의 제곱을 구함
		float s = max(0.f, min(1.f, D3DXVec3Dot(&ap, &ab) / abLen2));// 가야하는 정도에서 우리가 지금까지 간정도의 비율

		m_fRightRot = D3DXToRadian(- 45.f) * sinf(D3DX_PI * s);//만큼 돌림

		_vec3 vRTemp = vRUpTargetPos - m_vRightOfs;
		if (D3DXVec3Length(&vRTemp) < 35.f) {
			//m_vRightOfs = vRUpTargetPos;
			if (m_eLeftState == MoveState::NONE)
				m_eLeftState = MoveState::TO_UP;
		}
		break;
	}
	case MoveState::TO_ORIGIN:
		vDir = vROriginalTargetPos - m_vRightOfs;
		D3DXVec3Normalize(&vDir, &vDir);

		m_vRightOfs += vDir * 500.f * fTimeDelta;

		vRTemp = vROriginalTargetPos - m_vRightOfs;
		if (D3DXVec3Length(&vRTemp) < 20.f) {
			m_fRightRot = 0.f;
			m_eRightState = MoveState::NONE;
		}
		break;

	case MoveState::NONE:
	{
		_vec3 baseR = vROriginalTargetPos;
		if (m_status == PlayerStatus::WALKING) {
			Walking(baseR, m_walkT, m_walkSpeed, m_walkAmp, D3DX_PI, m_vRightOfs);
			m_fRightRot = 0.f;
		}
		else {
			float rotZR;
			Idle(baseR, m_idleT, m_idleSpeed, m_idleAmp, m_idleRotAmp, m_idlePhase, m_vRightOfs, rotZR);
			m_fRightRot = rotZR;
		}
		m_iRightUVX = 0;
		break;
	}
	}
}

void CPlayerTex::Clear_Action(const _float& fTimeDelta)
{
	m_vScaleOfs = { 1.1f, 1.9f, 1.f };
	_vec3 vUpTargetPos = { -150.f, 0.f, m_vOfs.z };
	_vec3 vOriginalTargetPos = { -500.f, -1000.f, m_vOfs.z };
	_vec3 vTemp;
	m_iUVY = 5;

	if (m_eMoveState == MoveState::NONE)
	{
		HoldTimer = 0.f;
		m_iUVX = 0;
		m_vOfs = vOriginalTargetPos;
		m_eMoveState = MoveState::TO_UP;
	}

	switch (m_eMoveState)
	{
	case MoveState::NONE:
	{
		HoldTimer = 0;
		m_iUVX = 0;
		m_vOfs = vOriginalTargetPos;
		break;
	}
	case MoveState::TO_UP:
		vDir = vUpTargetPos - m_vOfs;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vOfs += vDir * 1500.f * fTimeDelta;
		m_iUVX = 1;
		vTemp = vUpTargetPos - m_vOfs;
		if (D3DXVec3Length(&vTemp) < 35.f)
		{
			m_eMoveState = MoveState::HOLD;
		}
		break;

	case MoveState::HOLD:
		m_iUVX = 3;
		HoldTimer += fTimeDelta;
		if (HoldTimer >= 2) {
			m_eMoveState = MoveState::TO_ORIGIN;
		}
		break;

	case MoveState::TO_ORIGIN:
		vDir = vOriginalTargetPos - m_vOfs;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vOfs += vDir * 1500.f * fTimeDelta;
		m_iUVX = 6;
		vTemp = vOriginalTargetPos - m_vOfs;
		if (D3DXVec3Length(&vTemp) < 20.f)
		{
			if (!m_clearEventFired) {
				EVENT event{
					PHASE::PHASE_BEGIN,
					TOPIC::Stage_Clear,
					CHANNEL::UI
				};
				CEventBus::GetInstance()->publish(event);
				m_clearEventFired = true;
				CUIManager::GetInstance()->SetCategoryTag(L"PlayerTex", L"None");
			}
			m_eMoveState = MoveState::NONE;
			m_status = PlayerStatus::IDLE;
			ResetWeaponSprite();
		}
		break;
	}
}

void CPlayerTex::Dead_Action(const _float& fTimeDelta)
{
}

void CPlayerTex::ResetWeaponSprite()
{
	m_eLeftState = MoveState::NONE;
	m_eRightState = MoveState::NONE;
	m_eMoveState = MoveState::NONE;
	bChargeAttack = false;
	HoldTimer = 0.f;
	m_idleT = 0.f;
	m_walkT = 0.f;

	switch (m_eWeaponState)
	{
	case WEAPON_GUN:
		m_iUVY = 3;
		m_iUVXOffset = 2;
		m_iLeftUVX = 0;  m_iRightUVX = 0;
		m_fLeftRot = 0.f; m_fRightRot = 0.f;
		m_vLeftOfs = { 0.f,  -500.f, m_vLeftOfs.z };
		m_vRightOfs = { 400.f,    0.f, m_vRightOfs.z };
		break;

	case WEAPON_ICEWAND:
		m_iUVY = 0;
		m_iUVXOffset = 3;
		m_iLeftUVX = 0;  m_iRightUVX = 1;
		m_fLeftRot = 0.f; m_fRightRot = 0.f;
		m_vLeftOfs = { 0.f,    0.f, m_vLeftOfs.z };
		m_vRightOfs = { 400.f, -500.f, m_vRightOfs.z };
		break;

	case WEAPON_FIRERING:
		m_iUVY = 2;
		m_iUVXOffset = 4;
		m_iLeftUVX = 0;  m_iRightUVX = 0;
		m_fLeftRot = 0.f; m_fRightRot = 0.f;
		m_vLeftOfs = { -400.f, 0.f, m_vLeftOfs.z };
		m_vRightOfs = { 400.f, 0.f, m_vRightOfs.z };
		break;

	case WEAPON_FIREWAND:
		m_iUVY = 4;
		m_iUVX = 0;
		m_fLeftRot = 0.f;
		m_vScaleOfs = { 1.2f, 1.8f, 1.f };
		m_vOfs = { 300.f, -100.f, m_vOfs.z };
		break;

	case WEAPON_MACE:
	default:
		m_iUVY = 1;
		m_iUVX = 0;
		m_fLeftRot = 0.f;
		m_vScaleOfs = { 1.1f, 1.9f, 1.f };
		m_vOfs = { 300.f, 0.f, m_vOfs.z };
		break;
	}
}
