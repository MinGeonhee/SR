#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"
#include "CFog.h"
#include "CObjectPoolingManager.h"

BEGIN(Engine)

class ENGINE_DLL CFogSystem :
    public CGameObject
{
protected:
    explicit    CFogSystem(const CFogSystem& rhs);
    explicit    CFogSystem(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CFogSystem();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

protected:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();



    // 포그 저장용 벡터 =====================
protected:
    vector<CFog*>      m_vecFogs;


    // 바운딩 박스 크기 ============================
protected:
    _vec3 m_vBoundingBoxScale = { 0.f,0.f,0.f };


    // 포그 갯수 설정 ============================
protected:
    int m_iMaxFogNum = 20;


    // 포그 크기 (일정할때) ========================
protected:
    float m_fFogSize = 0.1;


public:
    static CFogSystem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
    virtual void  Free();
};

END



