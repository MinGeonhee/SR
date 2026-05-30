#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer :
    public CComponent
{
protected:
    // explicit CViBuffer 
    explicit CVIBuffer();
    explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CVIBuffer(const CVIBuffer& rhs);
    virtual ~CVIBuffer();

public:

    virtual HRESULT     Ready_Buffer();
    virtual void        Render_Buffer();

protected:

    virtual void        Make_Index(INDEX32* pINDEX) {}
    virtual void        Make_Vertex(VTXCOL* pVertex) {}

protected:

    LPDIRECT3DVERTEXBUFFER9     m_pVB = nullptr; // 버텍스 버퍼
    LPDIRECT3DINDEXBUFFER9      m_pIB = nullptr; // 인덱스 버퍼

    _ulong                      m_dwVtxSize = 0; // 버텍스 사이즈 정의
    _ulong                      m_dwVtxCnt = 0; // 버텍스 카운트
    _ulong                      m_dwTricnt = 0;  // 몇 개의 삼각형을 만들건지 정의.
    _ulong                      m_dwFVF = 0;

    _ulong                      m_dwIdxSize;
    D3DFORMAT                   m_IdxFmt;

public:
    virtual void    Free();

private:


};

END

