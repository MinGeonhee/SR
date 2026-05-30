#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;


// 싱글톤으로 안만드려고 했는데 그릴 떄마다 그래픽 디바이스 얻어와야하는 구조가 별로네
class ENGINE_DLL CDebugLineHelper :
    public CBase
{
    DECLARE_SINGLETON(CDebugLineHelper)

public:

    explicit CDebugLineHelper() {}
    virtual ~CDebugLineHelper() {}

public:
    _int    Debug_Update();
    // 각 스테이지에서 디버그 라인 매니저를 들고 있는다.

public:

    // 이것을 호출하면 내 x,y,z축 방향으로 라인을 그려준다.
    void AxisLine_Helper(CTransform* pTransform);
    void RayLine_Helper(_vec3 _raypos, _vec3 _rayDir, float _rayDist);
    void DrawAABBBox(const _vec3& min, const _vec3& max, const D3DXCOLOR& color);
    void DrawAABBTile(const _vec3& min, const _vec3& max, const D3DXCOLOR& color);

private:

    LPD3DXLINE      m_pLine;
    bool            m_bDebuglineActive = false;

public:
    virtual void        Free();
};

END