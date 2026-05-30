#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMapObjectTex :
    public CVIBuffer
{

private:
    explicit CMapObjectTex();
    explicit CMapObjectTex(LPDIRECT3DDEVICE9 Device);
    explicit CMapObjectTex(const CMapObjectTex& cpy);
    virtual ~CMapObjectTex();

public:
    //  x  y 시작 위치 UV  w h  -> 높이 넓이     Mw 사진의 최대 크기 , 최대 높이 그냥 사진의 크기.
    HRESULT         Change_Buffer(const MAPOBJECT_PLACEMENT& eType, const SpriteRect& tSprite);
    HRESULT         Change_Buffer(const MAPOBJECT_PLACEMENT& eType, const UVRect& tUVRect);
    HRESULT         Change_Buffer(const MAPOBJECT_PLACEMENT& eType);
    HRESULT         Change_Buffer(const SpriteRect& tSprite);
    HRESULT         Change_Buffer(const UVRect& tUVRect);
public:

    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

public:

    virtual CComponent* Clone() override;
    static CMapObjectTex* Create(LPDIRECT3DDEVICE9 Device);

private:
    virtual void        Free();
};

END