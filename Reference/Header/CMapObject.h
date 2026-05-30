#pragma once
#include "CGameObject.h"


namespace Engine
{
    class CMapObjectTex;
}


BEGIN(Engine)


class ENGINE_DLL CMapObject:
    public CGameObject
{

protected:

    explicit CMapObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMapObject(const CMapObject& rhs);
    virtual ~CMapObject();

protected:

    HRESULT    Add_Component();

public:

    const MAPOBJECT_CATEGORY& Get_Category() { return m_eCategory; }
    const MAPOBJECT_PLACEMENT& Get_Placement() { return m_ePlacement; }
    const _bool& IsEvent() { return m_IsEvent; }

public:

    void		Set_Category(const MAPOBJECT_CATEGORY& eType) { m_eCategory = eType; }
    void		Set_Placement(const MAPOBJECT_PLACEMENT& eType) { m_ePlacement = eType; }
    void		Set_Event(const _bool& IsEvent) { m_IsEvent = IsEvent; }

public:

    // x y 시작  uv 좌표   w , h  넓이 높이    Mw , Mh 최대 넓이 최대 높이 = 사진의크기...
    virtual void	Setting(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale,
                            const MAPOBJECT_CATEGORY& eCat , const MAPOBJECT_PLACEMENT& ePlacement , const SpriteRect& tSpriteRect);
    
    virtual void	Setting(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale,
        const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const UVRect& tUVRect);

    void    Change_UV_Scale(const SpriteRect& tSpriteRect);
    void    Change_UV_Scale(const UVRect& tUVRect);

    // 미리 업데이트 땡기기 트랜스폼 // 콜라이더
    void	SemiUpdate();

protected:

    //오브젝트에서 타입 카테고리  카테고리
    MAPOBJECT_CATEGORY		    m_eCategory;
    //맵 오브젝트 타입의 분류 땅인지 벽인지에 대해서 
    MAPOBJECT_PLACEMENT			m_ePlacement;

    //버퍼
    CMapObjectTex*              m_pBuffer;

    //업데이트 처리할지 말지에 대한 이벤트
    bool				        m_IsEvent;

protected:
    virtual     void        Free();
};

END