#pragma once
#include "CBase.h"
#include "CGraphicDev.h"

class CShape :
    public CBase
{
public:
    explicit CShape(LPDIRECT3DDEVICE9 pGraphicClass)
        : m_pDeviceClass(pGraphicClass)
    {}
    virtual ~CShape() {}

public:

    void    Ready_DrawShape();
    void    Ready_DrawTriangle();
    void    Draw_Triangle();
    int     Update_MainApp(const float& fTimeDelta); // 우리가 쓰던 업데이트
    void    LateUpdate_MainApp(const float& fTimeDelta); // 
    void    Offset(const float& fTimeDelta);
    void    Update_Vertex();
    void    Dir_Calculate();
    

    D3DXMATRIX    Make_WorldMatrix();
    D3DXMATRIX    Make_RotationMatrix();


private:

    IDirect3DVertexBuffer9*         m_vVertexBuffer = nullptr;
    IDirect3DIndexBuffer9*          m_vIndexBuffer = nullptr;
    LPDIRECT3DDEVICE9               m_pDeviceClass;

    typedef struct m_tInfo
    {
        _vec3                       m_vPos;
        _vec3                       m_vDir;
        _vec3                       m_vLook;
        _vec3                       m_vScale;
        _vec3                       m_vDefault; // 방향 default 벡터
    }INFO;

    INFO                            m_tInfo;
    float                           m_fSpeed;
    _vec3                           m_vAngle;

protected:

    virtual     void        Free(){}
    
};

