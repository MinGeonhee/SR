#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol :
    public CVIBuffer
{
private:
    explicit CTriCol();
    // 디바이스 컴 객체와 삼각형을 무슨 축 기준으로 vertex생성한 삼각형인지
    // 초기 생성할 때 설정해준다.
    explicit CTriCol(LPDIRECT3DDEVICE9 mGraphicDev, TRIANGLETYPE _type);
    explicit CTriCol(const CTriCol& rhs);
    virtual ~CTriCol();

public:
    // 누구 기준으로 
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();
    void            Set_TriangleType(TRIANGLETYPE type) { m_eType = type; }

protected:

    virtual void    Make_Vertex(VTXCOL* pVerTex) override;
    virtual void    Make_Index(INDEX32* pIndex) override;

public:

    virtual CComponent* Clone() override;
    static  CTriCol* Create(LPDIRECT3DDEVICE9 mGraphicDev, TRIANGLETYPE _type);
    // 이거 객체 생성할 때 왜 컴 객체를 들고 오지.
    // 아 생성할 때 생성자에 넣어주려고.

private:
    virtual void    Free();

private:
    TRIANGLETYPE    m_eType = TRIANGLETYPE::TRIANGLE_Y;

};

END
