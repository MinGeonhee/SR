#include "Engine_Define.h"
#include "CColliderBase.h"
#include "CAAABBB.h"
#include "CCollisionManager.h"
#include "COBB.h"

BEGIN(Engine)

typedef class ENGINE_DLL CollisionFunc
{
public:
	// AABB 충돌 vs 나머지
	static bool Collider_AABB_AAABBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_AABB_OBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_AABB_SPHERE(CColliderBase* pLeft, CColliderBase* pRight);

	// AAABBB 충돌 vs 나머지
	static bool Collider_AAABBB_AABB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_AAABBB_AAABBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_AAABBB_OBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_AAABBB_SPHERE(CColliderBase* pLeft, CColliderBase* pRight);

	// OBB 충돌 vs 나머지
	static bool Collider_OBB_AAABBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_OBB_OBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_OBB_SPHERE(CColliderBase* pLeft, CColliderBase* pRight);

	// 구체 충돌 vs 나머지
	static bool Collider_SPHERE_AABB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_SPHERE_AAABBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_SPHERE_OBB(CColliderBase* pLeft, CColliderBase* pRight);
	static bool Collider_SPHERE_SPHERE(CColliderBase* pLeft, CColliderBase* pRight);

	static bool Collider_false(CColliderBase* pLeft, CColliderBase* pRight);// 무조건 false 때림.
	static bool Collider_RAY(RAY_TYPE rayType, CGameObject* pObject, RAYINFO* pRayInfo);


	// 어느면 충돌인지 계산해서 뱉어주는 함수 Made My 민건희 -> 오류 있을시 문의 안받습니다
	static const  CUBE_FACE& Get_Collied_Face(const _vec3& Collied_Pos , const _vec3& Min, const _vec3& Max);
};

END