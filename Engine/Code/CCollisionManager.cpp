#include "CCollisionManager.h"
#include "CollisionFunc.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CAAABBB.h"
#include "COBB.h"
#include "CSphere.h"
#include "CCollisionEventManager.h"
#include "CObjectPoolingManager.h"
#include "CMapToolTile.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
	:m_arrCheck{}, m_RayCheck{}, m_vObjectList()
{
	Reset();
}


CCollisionManager::~CCollisionManager()
{
	Free();
}

void CCollisionManager::InitCollisionTable()
{
	// AABB 충돌 vs 나머지
	// 어차피 AABB vs AABB 일어날일이 없다.
	m_CollisionTable[COLLIDER_AABB][COLLIDER_AAABBB] = &CollisionFunc::Collider_AABB_AAABBB;
	m_CollisionTable[COLLIDER_AABB][COLLIDER_OBB] = &CollisionFunc::Collider_AABB_OBB;
	m_CollisionTable[COLLIDER_AABB][COLLIDER_SPHERE] = &CollisionFunc::Collider_AABB_SPHERE;

	// AAABBB 충돌 vs 나머지
	m_CollisionTable[COLLIDER_AAABBB][COLLIDER_AABB] = &CollisionFunc::Collider_AAABBB_AABB;
	m_CollisionTable[COLLIDER_AAABBB][COLLIDER_AAABBB] = &CollisionFunc::Collider_AAABBB_AAABBB;
	m_CollisionTable[COLLIDER_AAABBB][COLLIDER_OBB] = &CollisionFunc::Collider_AAABBB_OBB;
	m_CollisionTable[COLLIDER_AAABBB][COLLIDER_SPHERE] = &CollisionFunc::Collider_AAABBB_SPHERE;

	// OBB 충돌 vs 나머지
	m_CollisionTable[COLLIDER_OBB][COLLIDER_AAABBB] = &CollisionFunc::Collider_OBB_AAABBB;
	m_CollisionTable[COLLIDER_OBB][COLLIDER_OBB] = &CollisionFunc::Collider_OBB_OBB;
	m_CollisionTable[COLLIDER_OBB][COLLIDER_SPHERE] = &CollisionFunc::Collider_OBB_SPHERE;

	// 구 충돌 vs 나머지
	m_CollisionTable[COLLIDER_SPHERE][COLLIDER_AABB] = &CollisionFunc::Collider_SPHERE_AABB;
	m_CollisionTable[COLLIDER_SPHERE][COLLIDER_AAABBB] = &CollisionFunc::Collider_SPHERE_AAABBB;
	m_CollisionTable[COLLIDER_SPHERE][COLLIDER_OBB] =	 &CollisionFunc::Collider_SPHERE_OBB;
	m_CollisionTable[COLLIDER_SPHERE][COLLIDER_SPHERE] = &CollisionFunc::Collider_SPHERE_SPHERE;

	// 그냥 전부 충돌 false
	m_CollisionTable[COLLIDER_NONE][COLLIDER_AABB] = &CollisionFunc::Collider_false;
	m_CollisionTable[COLLIDER_NONE][COLLIDER_AAABBB] = &CollisionFunc::Collider_false;
	m_CollisionTable[COLLIDER_NONE][COLLIDER_OBB] = &CollisionFunc::Collider_false;
	m_CollisionTable[COLLIDER_NONE][COLLIDER_SPHERE] = &CollisionFunc::Collider_false;
	
	m_CollisionTable[COLLIDER_AABB]  [COLLIDER_NONE] = &CollisionFunc::Collider_false;
	m_CollisionTable[COLLIDER_AAABBB][COLLIDER_NONE] = &CollisionFunc::Collider_false;
	m_CollisionTable[COLLIDER_OBB]   [COLLIDER_NONE] = &CollisionFunc::Collider_false;
	m_CollisionTable[COLLIDER_SPHERE][COLLIDER_NONE] = &CollisionFunc::Collider_false;

	m_CollisionTable[COLLIDER_NONE][COLLIDER_NONE] = &CollisionFunc::Collider_false;
}


bool CCollisionManager::CallCollision(CColliderBase* pLeft, CColliderBase* pRight)
{
	if (!pLeft || !pRight) return false;

	COLLIDERTYPE eLeft = pLeft->Get_ColliderType();
	COLLIDERTYPE eRight = pRight->Get_ColliderType();

	CollisionFunction pFunc = m_CollisionTable[eLeft][eRight];

	if (pFunc)
		return pFunc(pLeft, pRight);

	return false;
}

// _uint 0x 0000 0000 0000 0010 

_uint CCollisionManager::Update_Collision()
{
	for (_uint iRow = 0; iRow < (_uint)OBJTYPE::OBJ_END; ++iRow)
	{
		m_arrCheck[iRow];
		for (_uint iCol = iRow; iCol < (_uint)OBJTYPE::OBJ_END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroupUpdate((OBJTYPE)iRow, (OBJTYPE)iCol);
			}
		}
	}

	return 0;
}

void CCollisionManager::CheckGroup(OBJTYPE _left, OBJTYPE _right)
{
	// 더 작은 값의 그룹 타입을 행으로.
	// 큰 값을 열(비트)로 사용

	_uint iRow = (_uint)_left;
	_uint iCol = (_uint)_right;


	if (iCol < iRow)
	{
		iRow = (_uint)_right; 
		iCol = (_uint)_left;
	}


	// 엄청 생소하다.
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}

	// 여기까지 오면 무조건 행이 더 작음.
	// 1 컬럼만큼을 비트 쉬프트로 밀어서 Row번째 인덱스에 넣어줌.

	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}

}

void CCollisionManager::CheckRaycastGroup(RAY_TYPE _left, OBJTYPE _right)
{
	_uint iRow = (_uint)_left;
	_uint iCol = (_uint)_right;

	// 이미 해당 비트 플래그에 숫자가 있을 때 그 쪽에 있는 숫자를 지운다.
	if (m_RayCheck[iRow] & (1 << iCol))
	{
		m_RayCheck[iRow] &= ~(1 << iCol);
	}


	m_RayCheck[iRow] |= (1 << iCol);
}

void CCollisionManager::CollisionGroupUpdate(OBJTYPE _left, OBJTYPE _right)
{
	// 내가 조합한 오브젝트 리스트끼리 충돌 검사를 함.
	// 그리고 만약에 true타입이 나왔다. 거기서 switch - case문을 만들고
	// 서로 오브젝트간의 상호작용을 한다.

	for (auto leftObject : m_vObjectList[_left])
	{
		if (leftObject && leftObject->Get_Active())
		{
			for (auto rightObject : m_vObjectList[_right])
			{
				if (rightObject && rightObject->Get_Active())
				{
					auto leftcollide = leftObject->Get_Collide();
					auto rightcollide = rightObject->Get_Collide();

					// 만약에 충돌이 일어났다???????????????????????  내 맵 오브젝트 어디가버렸어
					// 이벤트 매니저에서 충돌 처리하자.
 					if (leftcollide->Collide(rightcollide))
 						CCollisionEventManager::GetInstance()->TriggerCollision_Event(leftObject, rightObject);
		 		}
			}
		}

	}
}

void CCollisionManager::FireRaycast(RAY_TYPE rayType)
{
	RAYINFO closestHit;
	bool hitFound = false;

	// 내가 설정한 비트값에 있는 것들만 순회해서 충돌 검사를 한다.
	for (_uint i = 0; i < (_uint)OBJTYPE::OBJ_END; ++i)
	{
		if ((m_RayCheck[rayType] & (1 << i)) == 0)
			continue;

		for (auto object : m_vObjectList[i])
		{
			if (object->Get_Active())
			{
				int a = 0;
				RAYINFO pRay;
				if (CollisionFunc::Collider_RAY((RAY_TYPE)rayType, object, &pRay))
				{
					// closestHit의 초기 distance를 0으로 두었더니 절대 true 안됨...
					if (pRay.m_fDistance < closestHit.m_fDistance)
					{
						closestHit = pRay;
						hitFound = true;
 						closestHit.pGameobject = object;
					}
				}
			}

		}
	}

	if (hitFound)
	{
		// 좋았다 완벽해
		/*closestHit.pGameobject->Set_Active(false);*/
		//CObjectPoolingManager::GetInstance()->Return(closestHit.pGameobject);
		/*CCollisionEventManager::GetInstance()->TriggerCollision_Event(rayType, closestHit);*/
		CCollisionEventManager::GetInstance()->RayCollision_Event(rayType, closestHit);
	}
}

void CCollisionManager::FireRaycast(RAY_TYPE rayType, OBJTYPE ObjectType)
{
	RAYINFO closestHit;
	bool hitFound = false;

	if ((m_RayCheck[rayType] & (1 << ObjectType)) == 0)

		return;

	for (auto object : m_vObjectList[ObjectType])
	{
		RAYINFO pRay;
		if (CollisionFunc::Collider_RAY((RAY_TYPE)rayType, object, &pRay))
		{
			// closestHit의 초기 distance를 0으로 두었더니 절대 true 안됨...
			if (pRay.m_fDistance < closestHit.m_fDistance)
			{
				closestHit = pRay;
				hitFound = true;
				closestHit.pGameobject = object;
			}
		}
	}
	
	if (hitFound)
	{
		// 좋았다 완벽해
		/*closestHit.pGameobject->Set_Active(false);*/
		/*CCollisionEventManager::GetInstance()->TriggerCollision_Event(rayType, closestHit);*/
		CCollisionEventManager::GetInstance()->RayCollision_Event(rayType, closestHit);
	}
}


void CCollisionManager::Delete_ObjectList()
{
	// 플레이어를 제외한 모든걸 정리하는 함수
	for (int i = OBJ_PLAYER + 1; i < OBJ_END; i++)
	{
		m_vObjectList[i].clear();
	}
}

void CCollisionManager::Free()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		m_vObjectList[i].clear();
	}
}