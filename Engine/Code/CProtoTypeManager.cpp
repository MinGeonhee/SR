#include "CProtoTypeManager.h"

IMPLEMENT_SINGLETON(CProtoTypeManager)

CProtoTypeManager::CProtoTypeManager()
{
}

CProtoTypeManager::~CProtoTypeManager()
{
	Free();
}

HRESULT CProtoTypeManager::Ready_Prototype(PROTOTYPE _enumkey, CComponent* pComponent)
{
	CComponent* pInstance = Find_Prototype(_enumkey);

	// 원본을 등록하려는데 이미 키값이 있다면.
	if (pInstance != nullptr)
		return E_FAIL; // 꺼져

	m_mapPrototype.insert({ _enumkey, pComponent }); // 없다면 키값 등록

	return S_OK;
}

CComponent* CProtoTypeManager::Clone_Prototype(PROTOTYPE _enumkey)
{
	// 저 혹시 원본있나요?
	CComponent* pInstance = Find_Prototype(_enumkey);

	if (pInstance == nullptr)
		return nullptr;		// 아니 꺼져

	else
		return pInstance->Clone(); // 복제 한 놈을 return 한다.
}

// 저기요 혹시 원본 가지고 계신가요
CComponent* CProtoTypeManager::Find_Prototype(PROTOTYPE _enumkey)
{
	auto iter = m_mapPrototype.find(_enumkey);

	if (iter == m_mapPrototype.end())
		return nullptr; // 아니요

	return iter->second; // 네 여기요
}

void CProtoTypeManager::Free()
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

void CProtoTypeManager::DeleteALL()
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}