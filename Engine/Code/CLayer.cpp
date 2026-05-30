#include "CLayer.h"
#include "CCollisionManager.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{

}

CComponent* CLayer::Get_Component(
	COMPONENTID eID,
	const wstring& pObjTag,
	const wstring& pComponentTag)
{
	auto	iter = find_if(m_mapObject.begin(),
							m_mapObject.end(), 
		[&pObjTag](const auto& pair) {return pObjTag == pair.first; });

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(eID, pComponentTag);

}

HRESULT CLayer::Add_GameObject(const wstring& pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject[pObjTag] = pGameObject;

	//OBJTYPE type = pGameObject->Get_OBJTYPE();
 //	CCollisionManager::GetInstance()->Add_Object(type, pGameObject);

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	// iResult & 0x80000000	결과값이 음수이면 에러로 판단
	for (auto& pObj : m_mapObject)
	{
		iResult = pObj.second->Update_GameObject(fTimeDelta);
			if (iResult & 0x80000000)
				return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_mapObject)
	{
		pObj.second->LateUpdate_GameObject(fTimeDelta);
	}
}

void CLayer::Render_Layer()
{
	for (auto& pObj : m_mapObject)
	{
		pObj.second->Render_GameObject();
	}
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
