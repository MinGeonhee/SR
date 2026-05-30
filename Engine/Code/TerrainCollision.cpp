#include "TerrainCollision.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CTerrainTex.h"
#include "CInputManager.h"

IMPLEMENT_SINGLETON(TerrainCollision)

TerrainCollision::TerrainCollision()
{

}

void TerrainCollision::Collision_Update()
{
    Terrain_IndexCalculate();
}

void TerrainCollision::Terrain_IndexCalculate()
{
    //if (m_pPlayer == NULL || m_pTex == NULL)
    //    return;


    //auto& player = m_pPlayer;
    //CTransform* pPlayerTransform = player->Get_Transform();

    //_vec3 PlayerPos;
    //pPlayerTransform->Get_Info(INFO_POS, &PlayerPos);

    //int iVtxItv = m_pTex->Get_VtxITV();
    //int iCntX = m_pTex->Get_CntX();

    //// 플레이어 좌표를 기반으로 지형 정점 인덱스 계산
    //int playerXIndex = PlayerPos.x / iVtxItv;
    //int playerZIndex = PlayerPos.z / iVtxItv;

    //// 인덱스 범위 체크
    //if (playerXIndex < 0 || playerZIndex < 0 ||
    //    playerXIndex + 1 >= iCntX || playerZIndex + 1 >= m_pTex->Get_CntZ())
    //    return;

    //// 4개의 정점 추출
    //int baseIdx = playerXIndex + playerZIndex * iCntX;
    //_vec3 LB = m_pTex->Get_VertexPos(baseIdx);               // 좌하단
    //_vec3 RB = m_pTex->Get_VertexPos(baseIdx + 1);           // 우하단
    //_vec3 LT = m_pTex->Get_VertexPos(baseIdx + iCntX);       // 좌상단
    //_vec3 RT = m_pTex->Get_VertexPos(baseIdx + iCntX + 1);   // 우상단

    //_vec3 vRayOrigin = PlayerPos; // 너무 가까우면 안될 수도 있음.
    //_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);

    //float fDist = 0.f;
    //BOOL bHit = FALSE;
    //_vec3 vIntersect = {};

    //// 삼각형 1 충돌
    //bHit = D3DXIntersectTri(&LT, &LB, &RT, &vRayOrigin, &vRayDir, nullptr, nullptr, &fDist);
    //
    //// 삼각형 1이랑 충돌을 안했으면 
    //// 두번째 삼각형과 충돌을 시켜본다.
    //if (!bHit)
    //    bHit = D3DXIntersectTri(&LB, &RB, &RT, &vRayOrigin, &vRayDir, nullptr, nullptr, &fDist);

    //// 충돌 처리
    //if (bHit)
    //{
    //    vIntersect = vRayOrigin + vRayDir * fDist;
    //    PlayerPos.y = vIntersect.y + 0.5f;
    //    pPlayerTransform->Set_Pos(PlayerPos.x, PlayerPos.y, PlayerPos.z);
    // }
    //
}

void TerrainCollision::Free()
{
    Safe_Release(m_pPlayer);
    Safe_Release(m_pTex);
}