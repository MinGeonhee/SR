#include "pch.h"
#include "CSkyBox.h"
#include "CRendererManager.h"
#include "CInputManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CDebugLineHelper.h"
#include "CUI_NoticeBanner.h"
#include "CUIManager.h"
#include "CPauseUI.h"
#include "CObjectManager.h"
#include "CCameraManager.h"
#include "CSceneManager.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	:CGameObject(rhs)
{

}

CSkyBox::~CSkyBox()
{

}

HRESULT	CSkyBox::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_SKYCUBE;
	m_pTransform->Set_Scale(_vec3{50.f, 50.f, 50.f});

	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_SKYBOX, this);

	// 카메라 eye 구하기 (View의 역행렬)
	_matrix view = CCameraManager::GetInstance()->Get_MainViewMatrix();
	_matrix inv; D3DXMatrixInverse(&inv, nullptr, &view);
	_vec3 eye(inv._41, inv._42, inv._43);
	m_pTransform->Set_Pos(eye.x, eye.y, eye.z);

	return 0;
}

void CSkyBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSkyBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());

	if(CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS)
		CResourceManager::GetInstance()->Get<CTexture>(L"SkyBox2")->Set_Texture();

	else if(CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS2)
		CResourceManager::GetInstance()->Get<CTexture>(L"SkyBox2")->Set_Texture();

	else if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::STAGE2)
		CResourceManager::GetInstance()->Get<CTexture>(L"SkyBox3")->Set_Texture();

	else CResourceManager::GetInstance()->Get<CTexture>(L"SkyBox1")->Set_Texture();
	m_pBufferCom->Render_Buffer();
}


HRESULT CSkyBox::Add_Component()
{
	CComponent* pComponent = NULL;

	//================ 버퍼 컴포넌트 ===============

	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_SKYCUBETEX, CSkyCube);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

	// ============== Transform 컴포넌트 ==============

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

	//================ 콜라이더 AAABBB 충돌 컴포넌트 ===============
	pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_SPHERE, CSphere);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->Set_ColliderType(COLLIDER_NONE);

	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pPlayer = new CSkyBox(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pPlayer;
}

void CSkyBox::Free()
{
	Engine::CGameObject::Free();
}