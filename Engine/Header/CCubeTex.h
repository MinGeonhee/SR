#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex :
    public CVIBuffer
{
private:
    explicit CCubeTex();
    explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCubeTex(const CCubeTex& rhs);
    virtual ~CCubeTex();

private:

    virtual HRESULT Ready_Buffer();
    virtual HRESULT Ready_Buffer(_vec2 vCenterUV[CUBE_FACE_END]);

public:
    virtual void    Render_Buffer();

    virtual CComponent* Clone() override;

    static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        _vec2 vCenterUV[CUBE_FACE_END]);

    void            Default_Set_CenterUV();
    void            ChangeAllUI(const _vec2& vCenterUV);
public:

    void        ChangeTextureCubeFaceUV(const CUBE_FACE& etype, const _vec2& vCenterUV);
    _vec2*      GetCenterUV() { return m_vCenterUV; }
    _bool*      GetIsFilpHorizontalArr() { return m_isFilpHorizontal; }
    _bool*      GetIsFilpVerticalArr()   { return m_isFilpVertically; }

    void        Filp_Horizontal(const CUBE_FACE& etype);
    void        Filp_Vertically(const CUBE_FACE& etype);

    void        SetFilpHorizontalArr(_bool* FilpHorizontalArr)
    {
        if (FilpHorizontalArr == nullptr)
            return;

        for (int i = 0; i < CUBE_FACE_END; ++i)
        {
            m_isFilpHorizontal[i] = FilpHorizontalArr[i];
        }   
    }
    void        SetFilpVerticalArr(_bool* FilpArr)
    {
        if (FilpArr == nullptr)
            return;

        for (int i = 0; i < CUBE_FACE_END; ++i)
        {
            m_isFilpVertically[i] = FilpArr[i];
        }
    }


private:

    //큐브의 각 면에 따른 센터 UV좌표를 넣어준다
    _vec2       m_vCenterUV[CUBE_FACE_END];
    _bool       m_isFilpHorizontal[CUBE_FACE_END];
    _bool       m_isFilpVertically[CUBE_FACE_END];

private:
    virtual void        Free();
};

END
