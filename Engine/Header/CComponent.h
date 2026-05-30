#pragma once
#include "CBase.h"
#include "Engine_Define.h"

// 컴포넌트나 싱글톤 매니저들을 엔진 내부에서 생성했을 때,
// 반드시 namespace Engine으로 감싸주어야한다.

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent : // ENGINE_DLL = dllexport
    public CBase
{
protected:
    // LPDIRECT3DDEVICE9 = Com 객체의 디바이스 포인터.
    explicit CComponent(); // 명시적인 형변환만 허용하겠다.
    explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CComponent(const CComponent& rhs);
    virtual ~CComponent();

    // 컴포넌트는 2개의 집단으로 만든다.
    // -> 컴포넌트 enum, buffer component(로컬 스페이스 좌표를 보관하는 곳)
    // 매 프레임마다 적용 변경이 이루어져야하는 것(world Space).

    // Component 자식에는 static Component도 있다.

public:

    virtual _int Update_Component(const _float& iTimeDelta) { return 0; }
    virtual void LateUpdate_Component(const _float& iTimeDelta) {}

public:

    void            Set_MyOwner(CGameObject* pObject) { m_pMyOwner = pObject; }
    CGameObject*    Get_MyOwner() { return m_pMyOwner; }

protected:
    LPDIRECT3DDEVICE9       m_pGraphicDev;
    _bool                   m_bClone;
    CGameObject*            m_pMyOwner = nullptr;
    // 얕은 복사가 메모리 해제할 때 문제가 생긴다.
    // 하나의 주소가 복사가 되어 모두에게 동일하게 들어가기 때문에
    // m_bClone이라는 조건을 주어서 메모리 해제할 때 제동을 한번 걸어준다.

public:
    virtual CComponent* Clone() PURE;   // 프로토타입 디자인 패턴 사용 예
    // 프로토타입의 디자인 패턴을 사용하기 위함. (원형을 복제하기 위함이며 원본을 보호하며 복제해서 사용하기 위해 프로토타입의 디자인 패턴을 사용함)
    // -> 프로토타입 매니저를 만들어서 사용한다.

protected:
    virtual void        Free();
};


END
