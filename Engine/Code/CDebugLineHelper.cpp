#include "CDebugLineHelper.h"
#include "CInputManager.h"
#include "CTransform.h"
#include "CGraphicDev.h"
#include "CDebugLine.h"

IMPLEMENT_SINGLETON(CDebugLineHelper)

_int CDebugLineHelper::Debug_Update()
{
    if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::M))
        m_bDebuglineActive = !m_bDebuglineActive;

    return 0;
}

void CDebugLineHelper::AxisLine_Helper(CTransform* pTrans)
{
    if (!pTrans || !m_bDebuglineActive)
        return;

    D3DXVECTOR3 vPos, vRight, vUp, vLook;
    pTrans->Get_Info(INFO_POS, &vPos);
    pTrans->Get_Info(INFO_RIGHT, &vRight);
    pTrans->Get_Info(INFO_UP, &vUp);
    pTrans->Get_Info(INFO_LOOK, &vLook);

    D3DXVec3Normalize(&vRight, &vRight);
    D3DXVec3Normalize(&vUp, &vUp);
    D3DXVec3Normalize(&vLook, &vLook);

    float fLength = 5.0f;

    // 선 추가: (중심점 → 중심점 + 축방향)
    CDebugLine::AddLine(vPos, vPos + vRight * fLength, D3DXCOLOR(1, 0, 0, 1));  // 빨강
    CDebugLine::AddLine(vPos, vPos + vUp * fLength, D3DXCOLOR(0, 1, 0, 1));     // 초록
    CDebugLine::AddLine(vPos, vPos + vLook * fLength, D3DXCOLOR(0, 0, 1, 1));   // 파랑
}

void CDebugLineHelper::RayLine_Helper(_vec3 _raypos, _vec3 _rayDir, float _rayDist)
{
    if (!m_bDebuglineActive) return;

    CDebugLine::AddLine(_raypos, _raypos + _rayDir * _rayDist, D3DXCOLOR(1, 1, 1, 1));  // X: 빨강
}

void CDebugLineHelper::DrawAABBBox(const _vec3& min, const _vec3& max, const D3DXCOLOR& color)
{
    if (!m_bDebuglineActive) return;

    _vec3 corners[8] = {
        {min.x, min.y, min.z}, // 0
        {max.x, min.y, min.z}, // 1
        {max.x, max.y, min.z}, // 2
        {min.x, max.y, min.z}, // 3
        {min.x, min.y, max.z}, // 4
        {max.x, min.y, max.z}, // 5
        {max.x, max.y, max.z}, // 6
        {min.x, max.y, max.z}  // 7
    };

    // 아래면
    CDebugLine::AddLine(corners[0], corners[1], color);
    CDebugLine::AddLine(corners[1], corners[2], color);
    CDebugLine::AddLine(corners[2], corners[3], color);
    CDebugLine::AddLine(corners[3], corners[0], color);

    // 위면
    CDebugLine::AddLine(corners[4], corners[5], color);
    CDebugLine::AddLine(corners[5], corners[6], color);
    CDebugLine::AddLine(corners[6], corners[7], color);
    CDebugLine::AddLine(corners[7], corners[4], color);

    // 옆면 연결선
    CDebugLine::AddLine(corners[0], corners[4], color);
    CDebugLine::AddLine(corners[1], corners[5], color);
    CDebugLine::AddLine(corners[2], corners[6], color);
    CDebugLine::AddLine(corners[3], corners[7], color);
}

void CDebugLineHelper::DrawAABBTile(const _vec3& min, const _vec3& max, const D3DXCOLOR& color)
{
    if (!m_bDebuglineActive) return;

    _vec3 corners[4] = {
        {min.x, min.y, min.z}, // 0 // 좌하단
        {max.x, min.y, min.z}, // 1 // 우하단
        {max.x, min.y, max.z}, // 2 // 우상단
        {min.x, min.y, max.z}, // 3 // 좌상단
    };

    // 아래면
    CDebugLine::AddLine(corners[0], corners[3], color);
    CDebugLine::AddLine(corners[3], corners[2], color);
    CDebugLine::AddLine(corners[2], corners[1], color);
    CDebugLine::AddLine(corners[1], corners[0], color);
}


void CDebugLineHelper::Free()
{
    Safe_Release(m_pLine);
}