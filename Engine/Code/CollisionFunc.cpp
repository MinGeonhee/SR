#include "CollisionFunc.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CAAABBB.h"
#include "CAABB.h"
#include "COBB.h"
#include "CSphere.h"
#include "CCameraManager.h"
#include "CInputManager.h"
#include "CGraphicDev.h"
#include "CDebugLineHelper.h"

// AABB 충돌 vs 나머지
bool CollisionFunc::Collider_AABB_AAABBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 아 이분은 AABB님이시다.
    CAABB* pA = static_cast<CAABB*>(pLeft);
    CAAABBB* pB = static_cast<CAAABBB*>(pRight);
    
    if (pA == NULL || pB == NULL)
        return false;

    // 일단 절대 변경 불가능하게 const 박겠습니다.
    // 큐브에서 가장 작은 값 Min, 가장 큰 값 Max
    const _vec3& minA = pA->Get_Min();
    const _vec3& maxA = pA->Get_Max();

    // 이하생략
    const _vec3& minB = pB->Get_Min();
    const _vec3& maxB = pB->Get_Max();

    // 서로의 모든 x,y,z축에 대해 검사를 한다. 하나라도 false면 충돌 안함.

    if (minA.x > maxB.x || maxA.x < minB.x)
        return false;

    if (minA.y > maxB.y || maxA.y < minB.y)
        return false;

    if (minA.z > maxB.z || maxA.z < minB.z)
        return false;

    return true;
}

bool CollisionFunc::Collider_AABB_OBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 몰라 어차피 안쓸건데 구현해야하나
    return false;
}

bool CollisionFunc::Collider_AABB_SPHERE(CColliderBase* pLeft, CColliderBase* pRight)
{
    CAABB* pTile = static_cast<CAABB*>(pLeft);
    CSphere* pSphere = static_cast<CSphere*>(pRight);

    if (pTile == NULL || pSphere == NULL)
        return false;

    const _vec3& min = pTile->Get_Min();
    const _vec3& max = pTile->Get_Max();

    // 현재 충돌체의 중점
    _vec3 center = pSphere->Get_Collide_Center();
    float radius = pSphere->Get_Radius();

    _vec3 closest;
    closest.x = max(min.x, min(center.x, max.x));
    closest.y = max(min.y, min(center.y, max.y));
    closest.z = max(min.z, min(center.z, max.z));

    _vec3 dir = center - closest;
    float distSq = D3DXVec3LengthSq(&dir);

    return distSq <= radius * radius;
}


// AAABBB 충돌 vs 나머지
bool CollisionFunc::Collider_AAABBB_AABB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 야호 위에꺼 부르고 날먹
    return Collider_AABB_AAABBB(pRight, pLeft);
}

bool CollisionFunc::Collider_AAABBB_AAABBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 어차피 너네 AABB
    CAAABBB* pA = static_cast<CAAABBB*>(pLeft);
    CAAABBB* pB = static_cast<CAAABBB*>(pRight);

    // 둘 중에 하나라도 nullptr이면 return false때린다.
    if (pA == nullptr || pB == nullptr)
        return false;

    // 저얼대 변경 불가능하게 const 박을게~
    // 큐브에서 가장 작은 값 Min, 가장 큰 값 Max
    const _vec3& minA = pA->Get_Min();
    const _vec3& maxA = pA->Get_Max();

    // 이하생략
    const _vec3& minB = pB->Get_Min();
    const _vec3& maxB = pB->Get_Max();

    // 서로의 모든 x,y,z축에 대해 검사를 한다. 하나라도 false면 충돌 안함.

    if (minA.x > maxB.x || maxA.x < minB.x)
        return false;

    if (minA.y > maxB.y || maxA.y < minB.y)
        return false;

    if (minA.z > maxB.z || maxA.z < minB.z)
        return false;

    return true;

}

bool CollisionFunc::Collider_AAABBB_SPHERE(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 내 사전에 dynamic은 없다. 어차피 이거 호출됐으면 AABB, CSphere이겠지.

    CAAABBB* pBox = static_cast<CAAABBB*>(pLeft);
    CSphere* pSphere = static_cast<CSphere*>(pRight);

    if (pBox == NULL || pSphere == NULL)
        return false;

    const _vec3& min = pBox->Get_Min();
    const _vec3& max = pBox->Get_Max();

    // 현재 충돌체의 중점
     _vec3 center = pSphere->Get_Collide_Center();
    float radius = pSphere->Get_Radius();

    _vec3 closest;
    closest.x = max(min.x, min(center.x, max.x));
    closest.y = max(min.y, min(center.y, max.y));
    closest.z = max(min.z, min(center.z, max.z));

    _vec3 dir = center - closest;
    float distSq = D3DXVec3LengthSq(&dir);

    return distSq <= radius * radius;
}

bool CollisionFunc::Collider_AAABBB_OBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // AABB & OBB 충돌
    CAAABBB* pAABB = static_cast<CAAABBB*>(pLeft);
    COBB* pOBB = static_cast<COBB*>(pRight);

    if (pAABB == NULL || pOBB == NULL)
        return false;

    // AABB의 중심과 크기
    _vec3 aabbMin = pAABB->Get_Min();
    _vec3 aabbMax = pAABB->Get_Max();
    _vec3 aabbCenter = pAABB->Get_Collide_Center();
    _vec3 aabbHalfSize = (aabbMax - aabbMin) * 0.5f;

    // OBB의 중심과 축, 반크기
    _vec3 obbCenter = pOBB->Get_Collide_Center();

    _vec3 obbAxis[3]; // OBB 축 3개 선언하고

    pOBB->Get_MyOwner()->Get_Transform()->Get_Info(INFO_RIGHT, &obbAxis[0]);
    pOBB->Get_MyOwner()->Get_Transform()->Get_Info(INFO_UP, &obbAxis[1]);
    pOBB->Get_MyOwner()->Get_Transform()->Get_Info(INFO_LOOK, &obbAxis[2]);

    // OBB의 절반 사이즈 가져온다.
    _vec3 obbHalfSize = pOBB->Get_HalfSize();

    // 중심 간 거리
    _vec3 diff = obbCenter - aabbCenter;

    // AABB는 월드축 정렬된 박스 => 기본 축은 (1,0,0), (0,1,0), (0,0,1)
    _vec3 aabbAxis[3] = {
        _vec3(1.f, 0.f, 0.f),
        _vec3(0.f, 1.f, 0.f),
        _vec3(0.f, 0.f, 1.f)
    };

    // SAT 15축 테스트
    for (int i = 0; i < 3; ++i) // AABB 축
    {
        float ra = aabbHalfSize[i];
        float rb =
            fabs(D3DXVec3Dot(&obbAxis[0], &aabbAxis[i])) * obbHalfSize.x +
            fabs(D3DXVec3Dot(&obbAxis[1], &aabbAxis[i])) * obbHalfSize.y +
            fabs(D3DXVec3Dot(&obbAxis[2], &aabbAxis[i])) * obbHalfSize.z;

        float dist = fabs(D3DXVec3Dot(&diff, &aabbAxis[i]));
        if (dist > ra + rb) return false;
    }

    for (int i = 0; i < 3; ++i) // OBB 축
    {
        float ra =
            fabs(D3DXVec3Dot(&aabbAxis[0], &obbAxis[i])) * aabbHalfSize.x +
            fabs(D3DXVec3Dot(&aabbAxis[1], &obbAxis[i])) * aabbHalfSize.y +
            fabs(D3DXVec3Dot(&aabbAxis[2], &obbAxis[i])) * aabbHalfSize.z;

        float rb = obbHalfSize[i];

        float dist = fabs(D3DXVec3Dot(&diff, &obbAxis[i]));
        if (dist > ra + rb) return false;
    }

    // 여기에 교차 축 테스트까지 넣으면 완벽한 SAT인데, 간단화해서 생략할 수도 있음.

    return true;
}


// OBB 충돌 vs 나머지
bool CollisionFunc::Collider_OBB_AAABBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 그냥 AABB vs OBB를 좌우 바꿔서 호출
    return Collider_AAABBB_OBB(pRight, pLeft);
}

bool CollisionFunc::Collider_OBB_OBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    COBB* a = static_cast<COBB*>(pLeft);
    COBB* b = static_cast<COBB*>(pRight);

    if (a == NULL || b == NULL) 
        return false;

    _vec3 aCenter = a->Get_Collide_Center();

    _vec3 bCenter = b->Get_Collide_Center();

    _vec3 T = bCenter - aCenter;

    _vec3 A[3];
    a->Get_MyOwner()->Get_Transform()->Get_Info(INFO_RIGHT, &A[0]);
    a->Get_MyOwner()->Get_Transform()->Get_Info(INFO_UP, &A[1]);
    a->Get_MyOwner()->Get_Transform()->Get_Info(INFO_LOOK, &A[2]);

    _vec3 B[3];
    b->Get_MyOwner()->Get_Transform()->Get_Info(INFO_RIGHT, &B[0]);
    b->Get_MyOwner()->Get_Transform()->Get_Info(INFO_UP, &B[1]);
    b->Get_MyOwner()->Get_Transform()->Get_Info(INFO_LOOK, &B[2]);

    _vec3 aSize = a->Get_HalfSize();
    _vec3 bSize = b->Get_HalfSize();

    for (int i = 0; i < 3; ++i)
    {
        float ra = aSize[i];
        float rb =
            fabs(D3DXVec3Dot(&B[0], &A[i])) * bSize.x +
            fabs(D3DXVec3Dot(&B[1], &A[i])) * bSize.y +
            fabs(D3DXVec3Dot(&B[2], &A[i])) * bSize.z;

        float dist = fabs(D3DXVec3Dot(&T, &A[i]));
        if (dist > ra + rb) return false;
    }

    for (int i = 0; i < 3; ++i)
    {
        float ra =
            fabs(D3DXVec3Dot(&A[0], &B[i])) * aSize.x +
            fabs(D3DXVec3Dot(&A[1], &B[i])) * aSize.y +
            fabs(D3DXVec3Dot(&A[2], &B[i])) * aSize.z;

        float rb = bSize[i];

        float dist = fabs(D3DXVec3Dot(&T, &B[i]));
        if (dist > ra + rb) return false;
    }

    return true; // 교차하는 축이 없음 → 충돌
}

bool CollisionFunc::Collider_OBB_SPHERE(CColliderBase* pLeft, CColliderBase* pRight)
{
    COBB* box = static_cast<COBB*>(pLeft);
    CSphere* sphere = static_cast<CSphere*>(pRight);

    if (box == NULL || sphere == NULL)
        return false;

    _vec3 center = sphere->Get_Collide_Center();

    // 구 반지름 구하기.
    float radius = sphere->Get_Radius();

    _vec3 obbCenter = box->Get_Collide_Center();

    _vec3 axis[3];
    box->Get_MyOwner()->Get_Transform()->Get_Info(INFO_RIGHT, &axis[0]);
    box->Get_MyOwner()->Get_Transform()->Get_Info(INFO_UP, &axis[1]);
    box->Get_MyOwner()->Get_Transform()->Get_Info(INFO_LOOK, &axis[2]);

    // OBB 절반 사이즈.
    _vec3 halfSize = box->Get_HalfSize();

    // 각자의 중점 거리를 dist로 둔다
    _vec3 dist = center - obbCenter;

    float distSq = 0.f;
    for (int i = 0; i < 3; ++i)
    {
        float proj = D3DXVec3Dot(&dist, &axis[i]);
        float clamped = max(-halfSize[i], min(proj, halfSize[i]));
        _vec3 closest = obbCenter + axis[i] * clamped;
        dist = center - closest;
    }

    // 주어진 3D 벡터의 길이의 제곱을 반환하는 함수 D3DXVec3LengthSq()
    // 루트 연산은 속도가 느려서 제곱 연산을 한다고 한다.
    return D3DXVec3LengthSq(&dist) <= radius * radius;
}


// 구체 충돌 vs 나머지

bool CollisionFunc::Collider_SPHERE_AABB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 날먹 날먹
    return Collider_AABB_SPHERE(pRight, pLeft);
}

bool CollisionFunc::Collider_SPHERE_AAABBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 그냥 AABB vs Sphere 순서 바꿔서 호출
    return Collider_AAABBB_SPHERE(pRight, pLeft);
}

bool CollisionFunc::Collider_SPHERE_OBB(CColliderBase* pLeft, CColliderBase* pRight)
{
    // 그냥 OBB vs Sphere 순서 바꿔서 호출
    return Collider_OBB_SPHERE(pRight, pLeft);
}

bool CollisionFunc::Collider_SPHERE_SPHERE(CColliderBase* pLeft, CColliderBase* pRight)
{
    CSphere* a = static_cast<CSphere*>(pLeft);
    CSphere* b = static_cast<CSphere*>(pRight);

    if (a == NULL || b == NULL) 
        return false;

    _vec3 aCenter = a->Get_Collide_Center();
    _vec3 bCenter = b->Get_Collide_Center();

    // 구의 각자 중점을 뺀 거리
    _vec3 diff = aCenter - bCenter;
    //두 중심 사이 거리 ≤ 반지름 합 → 겹쳤음
   // 두 중심 사이 거리 > 반지름 합 → 충돌 아님
    float distSq = D3DXVec3LengthSq(&diff);     // 방향 벡터를 제곱해서 거리를 만든다네요.
    float radiusSum = a->Get_Radius() + b->Get_Radius();    // 서로 반지름을 더하고 제곱함.

    return distSq <= radiusSum * radiusSum; // 서로 제곱한 것을 비교하고 충돌 여부 뱉음
}


// 광선 충돌 

bool CollisionFunc::Collider_RAY(RAY_TYPE rayType, CGameObject* pObject, RAYINFO* pRayInfo)
{
    // 오브젝트의 Collide 타입이 NONE이면 충돌처리 검사 x
    if (pObject->Get_Collide()->Get_ColliderType() == COLLIDER_NONE)
        return false;

    POINT   ptMouse = CInputManager::GetInstance()->GetMousePos();

    pRayInfo->pGameobject = pObject;

    // 뷰포트 -> 투영으로 내리는 단계
    _vec3 vMousePos;

    D3DVIEWPORT9    ViewPort;
    ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

    // 혹시 이거 때문에 누수 생기는거 아니겠지.
    CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

    // 마우스 좌표를 0,0 ~ 1,1로 내리기 위함
    vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    // 투영 -> 뷰스페이스

    D3DXMATRIX matProj;

    // Ray 타입이 UI가 아닌것들은 전부 
    if (rayType != UI_RAY)
        matProj = CCameraManager::GetInstance()->Get_MainProjMatrix();

    // 만약 광선 타입이 UI 타입이면 UI카메라에서 발사하는걸로.
    else
        matProj = CCameraManager::GetInstance()->Get_UIProjMatrix();

    D3DXMatrixInverse(&matProj, 0, &matProj);
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    // 뷰 스페이스 -> 월드
    D3DXMATRIX matView;

    // Ray 타입이 UI가 아닌것들은 전부 
    if (rayType != UI_RAY)
        matView = CCameraManager::GetInstance()->Get_MainViewMatrix();

    // 만약 광선 타입이 UI 타입이면 UI카메라에서 발사하는걸로.
    else
        matView = CCameraManager::GetInstance()->Get_UIViewMatrix();

    // 카메라의 월드 행렬을 만들어줌.
    D3DXMatrixInverse(&matView, 0, &matView);

    _vec3   vRayPos{ 0.f,0.f,0.f };     // 뷰스페이스
    _vec3   vRayDir = vMousePos - vRayPos; // 뷰스페이스

    // 

    // 카메라의 월드행렬이 곱해짐 위치(Position) 벡터
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);   // 카메라 월드 행렬을 곱하면 뷰스페이스 삭제
                                                            // 월드 단위 pos 

    // 카메라의 월드행렬이 곱해짐 방향(Direction) 벡터
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);  // 월드 행렬.
                                                             // 월드 단위 Dir
    D3DXVec3Normalize(&vRayDir, &vRayDir);
    // 충돌 했냐아아아!

    // 디버그용인데 출력 안됨
    /*CDebugLineHelper::GetInstance()->RayLine_Helper(vRayPos, vRayDir, 10.f);*/
    return pObject->Get_Collide()->RayIntersect(vRayPos, vRayDir, &(pRayInfo->m_fDistance), &(pRayInfo->m_vHitPos));

}

const CUBE_FACE& CollisionFunc::Get_Collied_Face(const _vec3& Collied_Pos, const _vec3& Min, const _vec3& Max)
{
    float Epsilon = 0.001f;

    if (fabs(Collied_Pos.x - Min.x) < Epsilon) return CUBE_FACE_NEGATIVE_X;
    else if (fabs(Collied_Pos.x - Max.x) < Epsilon) return CUBE_FACE_POSITIVE_X;

    //else if (fabs(Collied_Pos.y - Min.y) < Epsilon) return CUBE_FACE_NEGATIVE_Y;
    else if (fabs(Collied_Pos.y - Max.y) < Epsilon) return CUBE_FACE_POSITIVE_Y;

    else if (fabs(Collied_Pos.z - Min.z) < Epsilon) return CUBE_FACE_NEGATIVE_Z;
    else if (fabs(Collied_Pos.z - Max.z) < Epsilon) return CUBE_FACE_POSITIVE_Z;

    return CUBE_FACE_END;
}

bool CollisionFunc::Collider_false(CColliderBase* pLeft, CColliderBase* pRight)
{
    return false;
}