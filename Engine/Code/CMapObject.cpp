#include "CMapObject.h"
#include "CRendererManager.h"
#include "CProtoTypeManager.h"
#include "CObstacle.h"

CMapObject::CMapObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev) , m_eCategory(MAPOBJECT_CATEGORY::_END) , m_ePlacement(MAPOBJECT_PLACEMENT::_END)
	, m_pBuffer(nullptr) , m_IsEvent(true) 
{
	m_tHandle.m_eType = OBJ_NONE;
}

CMapObject::CMapObject(const CMapObject& rhs)
	: CGameObject(rhs) , m_eCategory(rhs.m_eCategory), m_ePlacement(rhs.m_ePlacement) , m_IsEvent(rhs.m_IsEvent) , m_pBuffer(nullptr)
{
	m_tHandle.m_eType = OBJ_NONE;
}

CMapObject::~CMapObject()
{
}




HRESULT CMapObject::Add_Component()
{
	//트랜스폼 , 버퍼 생성
	CComponent* newCom = nullptr;

	newCom = m_pBuffer = CLONE_PROTOTYPE(PROTO_MAPOBJECTTEX, CMapObjectTex);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer" , newCom });


	newCom = nullptr;
	newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans" , newCom });


	return S_OK;
}



void CMapObject::Setting(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale, const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const SpriteRect& tSpriteRect)
{
	CGameObject::Setting_GameObject(tHandle, vPos , vRot , vScale);

	m_eCategory = eCat;
	m_ePlacement = ePlacement;

	//기본 픽셀 크기 기준으로 바꾼 스케일값 
	_vec3 vScal = PixelToWorldScale(tSpriteRect.w, tSpriteRect.h);
	m_pTransform->Set_Scale({ vScal.x  , vScal.y  , 1.f });

	auto MyScale = m_pTransform->Get_Scale();

	_float vScaleCol = vScale.x;


	//콜라이더가 없다면
	if (m_pColliderCom != nullptr)
	{
		if (eCat == MAPOBJECT_CATEGORY::OBSTACLE)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.3, MyScale.y * 0.8f, MyScale.x * 0.3));
		else if (eCat == MAPOBJECT_CATEGORY::EVENT)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.5, MyScale.y * 0.8f, MyScale.x * 0.5));
		else
			m_pColliderCom->Set_ColliderScale(_vec3(vScaleCol , MyScale.y , vScaleCol));


		//버퍼가 땅타입이면 콜라이더 위치를 스케일 y값의절반만큼 위로 올려줘야한다
		if (m_ePlacement == MAPOBJECT_PLACEMENT::GROUND)
			m_pColliderCom->Set_Offset(_vec3{ 0.f , 0.5f * MyScale.y  , 0.f });
	}



	m_pBuffer->Change_Buffer(m_ePlacement, tSpriteRect);


	//할까말까
	CMapObject::SemiUpdate();

	return;
}

void CMapObject::Setting(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale, const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const UVRect& tUVRect)
{

	if (m_pTransform == nullptr)
		return;

	CGameObject::Setting_GameObject(tHandle, vPos, vRot, vScale);

	m_eCategory = eCat;
	m_ePlacement = ePlacement;

	m_pTransform->Set_Scale({ tUVRect.vScale.x , tUVRect.vScale.y, tUVRect.vScale.z });

	_float vScaleCol = vScale.x;

	if (m_pColliderCom != nullptr)
	{
		auto MyScale = m_pTransform->Get_Scale();

		if (eCat == MAPOBJECT_CATEGORY::OBSTACLE)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.3, MyScale.y *0.8f , MyScale.x * 0.3));
		else if (eCat == MAPOBJECT_CATEGORY::EVENT)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.5, MyScale.y * 0.8f , MyScale.x * 0.5));
		else
			m_pColliderCom->Set_ColliderScale(_vec3(vScaleCol, MyScale.y , vScaleCol));

		//버퍼가 땅타입이면 콜라이더 위치를 스케일 y값의절반만큼 위로 올려줘야한다
		if (m_ePlacement == MAPOBJECT_PLACEMENT::GROUND)
			m_pColliderCom->Set_Offset(_vec3{ 0.f ,  0.5f * MyScale.y  , 0.f });
	}


	m_pBuffer->Change_Buffer(m_ePlacement , tUVRect);


	CMapObject::SemiUpdate();
}

void CMapObject::Change_UV_Scale(const SpriteRect& tSpriteRect)
{
	if (m_pBuffer == nullptr)
		return;
	if (m_pTransform == nullptr)
		return;

	m_pBuffer->Change_Buffer(tSpriteRect);

	_vec3 vScale = PixelToWorldScale(tSpriteRect.w, tSpriteRect.h);
	m_pTransform->Set_Scale({ vScale.x  , vScale.y  , vScale.z });

	_float vScaleCol = vScale.x;


	if (m_pColliderCom != nullptr)
	{
		auto MyScale = m_pTransform->Get_Scale();

		if (m_eCategory == MAPOBJECT_CATEGORY::OBSTACLE)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.3, MyScale.y * 0.8f, MyScale.x * 0.3));
		else if (m_eCategory == MAPOBJECT_CATEGORY::EVENT)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.5, MyScale.y * 0.8f, MyScale.x * 0.5));
		else
			m_pColliderCom->Set_ColliderScale(_vec3(vScaleCol, MyScale.y, vScaleCol));

		//버퍼가 땅타입이면 콜라이더 위치를 스케일 y값의절반만큼 위로 올려줘야한다
		if (m_ePlacement == MAPOBJECT_PLACEMENT::GROUND)
			m_pColliderCom->Set_Offset(_vec3{ 0.f ,  0.5f * MyScale.y  , 0.f });
	}

}

void CMapObject::Change_UV_Scale(const UVRect& tUVRect)
{
	if (m_pBuffer == nullptr)
		return;
	if (m_pTransform == nullptr)
		return;

	m_pBuffer->Change_Buffer(tUVRect);
	


	_vec3 vScale = tUVRect.vScale;
	m_pTransform->Set_Scale({ tUVRect.vScale.x  , tUVRect.vScale.y  , vScale.x});

	_float vScaleCol = vScale.x;


	if (m_pColliderCom != nullptr)
	{
		auto MyScale = m_pTransform->Get_Scale();

		if (m_eCategory == MAPOBJECT_CATEGORY::OBSTACLE)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.3, MyScale.y * 0.8f, MyScale.x * 0.3));
		else if (m_eCategory == MAPOBJECT_CATEGORY::EVENT)
			m_pColliderCom->Set_ColliderScale(_vec3(MyScale.x * 0.5, MyScale.y * 0.8f, MyScale.x * 0.5));
		else
			m_pColliderCom->Set_ColliderScale(_vec3(vScaleCol, MyScale.y, vScaleCol));

		//버퍼가 땅타입이면 콜라이더 위치를 스케일 y값의절반만큼 위로 올려줘야한다
		if (m_ePlacement == MAPOBJECT_PLACEMENT::GROUND)
			m_pColliderCom->Set_Offset(_vec3{ 0.f ,  0.5f * MyScale.y  , 0.f });
	}
}

void CMapObject::SemiUpdate()
{
	// 포지션 값과  콜라이더 업데이트를 미리 업데이트 하고 난 이후 업데이트 도는걸 막기위함
	m_pTransform->Update_Component(0.f);
	if( m_pColliderCom != nullptr)
		m_pColliderCom->LateUpdate_Component(0.f);
}

void CMapObject::Free()
{
	CGameObject::Free();
}
