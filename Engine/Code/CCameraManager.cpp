#include "CCameraManager.h"

IMPLEMENT_SINGLETON(CCameraManager)

// 등록시킬 카메라.
CCameraManager::CCameraManager()
	:m_pMainCamera(nullptr)
{
}

CCameraManager::~CCameraManager()
{
	Free();
}

HRESULT CCameraManager::Ready_Camera(const wstring& pCameraTag, CGameObject* pCamera)
{
	CGameObject* pInstance = Find_Camera(pCameraTag);

	// 원본을 등록하려는데 이미 키값이 있다면.
	if (pInstance != nullptr)
	{
		MSG_BOX("이미 있는 카메라!");
		return S_OK; // 꺼져
	}


 	m_mapCameraType.insert({ pCameraTag, pCamera }); // 없다면 키값 등록

	
	return S_OK;
}

void CCameraManager::Set_MainCamera(const wstring& pCameraTag)
{
	auto iter = m_mapCameraType.find({pCameraTag});
	m_pMainCamera = (*iter).second;
}

void CCameraManager::Set_UICamera(const wstring& pCameraTag)
{
	auto iter = m_mapCameraType.find({ pCameraTag });
	m_pUICamera = (*iter).second;
}

// 메인 카메라만 Set_Target 가능하다.
void CCameraManager::Set_CameraTarget(CGameObject* pObject)
{
	m_pMainCamera->Set_Target(pObject);
}

_uint CCameraManager::Update_MainCamera(const _float& _fTimeDelta)
{
	if(m_pMainCamera!= nullptr)
	m_pMainCamera->Update_GameObject(_fTimeDelta);

	return 0;
}

_uint CCameraManager::Update_UICamera(const _float& _fTimeDelta)
{
	if (m_pUICamera != nullptr)
		m_pUICamera->Update_GameObject(_fTimeDelta);

	return 0;
}

// 메인 카메라 Late_Update
void CCameraManager::LateUpdate_MainCamera(const _float& _fTimeDelta)
{
	if (m_pMainCamera != nullptr)
	m_pMainCamera->LateUpdate_GameObject(_fTimeDelta);

}

// UI 카메라 Late_Update
void CCameraManager::LateUpdate_UICamera(const _float& _fTimeDelta)
{
	if (m_pUICamera != nullptr)
	m_pUICamera->LateUpdate_GameObject(_fTimeDelta);
}

const _matrix& CCameraManager::Get_MainViewMatrix()
{
	if(m_pMainCameraCom == nullptr)
	{
		_matrix matIdentity;
		D3DXMatrixIdentity(&matIdentity);

		return matIdentity; 
	}
	return m_pMainCameraCom->Get_ViewMatrix();
}

const _matrix& CCameraManager::Get_MainProjMatrix()
{
	if (m_pMainCameraCom == nullptr)
	{
		_matrix matIdentity;
		D3DXMatrixIdentity(&matIdentity);

		return matIdentity; 
	}

	return m_pMainCameraCom->Get_ProjMatrix();
}

const _matrix& CCameraManager::Get_UIViewMatrix()
{
	if (m_pUiCameraCom == nullptr)
	{
		_matrix matIdentity;
		D3DXMatrixIdentity(&matIdentity);

		return matIdentity; 
	}

	return m_pUiCameraCom->Get_ViewMatrix();
}

const _matrix& CCameraManager::Get_UIProjMatrix()
{
	if (m_pUiCameraCom == nullptr)
	{
		_matrix matIdentity;
		D3DXMatrixIdentity(&matIdentity);
	
		return matIdentity; 
	}


	return m_pUiCameraCom->Get_ProjMatrix();
}

CGameObject* CCameraManager::Find_Camera(const wstring& pCameraTag)
{
	// 람다식 사용
	auto iter = find_if(m_mapCameraType.begin(), m_mapCameraType.end(), 
		[&pCameraTag](const auto& pair) {return pCameraTag == pair.first; });

	if (iter == m_mapCameraType.end())
		return nullptr; // 아니요

	return iter->second; // 네 여기요
}


void CCameraManager::Free()
{
	for_each(m_mapCameraType.begin(), m_mapCameraType.end(), CDeleteMap());
	m_mapCameraType.clear();
}
