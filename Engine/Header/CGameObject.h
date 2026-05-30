#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class CTransform;
class CColliderBase;
class CStatus;
class CTexture;

// 오브젝트 Event Bus를 하려면 라우팅을 해야함. 
// 라우팅을 하려면 정확히 내가 어디 소속인지 밝혀야한다.
// 
// 현재 우리 구조에서는 
// 1. Origin(어느 매니저 소속인지), 
// 2. OBJ_type(어떤 타입인지), 
// 3. Index(해당 슬롯의 몇번째 객체인지), 
// 4. Generation(내가 Pooling을 몇번 하였는가. (내가 지금 몇 세대인지?))

class ENGINE_DLL CGameObject :
    public CBase
{
protected:

    explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CGameObject(const CGameObject& rhs);
    virtual ~CGameObject();

public:
    //컴포넌트 Get 관련

    CComponent*             Get_Component(COMPONENTID eID, const wstring& pComponentTag);
    virtual CTransform*     Get_Transform() { return m_pTransform; }

    // 이것 떄문이라고?
    virtual CColliderBase*  Get_Collide() { return m_pColliderCom; }
    CStatus*                Get_Status() { return m_pStatusCom; }

public:

    
    virtual void            Set_SubType(void* SubType);

public:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

public: //핸들 관련

    //핸들
    const ObjectHandle&     Get_Handle() { return m_tHandle; }
    void                    Set_Handle(const ObjectHandle& tHandle) { m_tHandle = tHandle; }

    // 오브젝트 타입
    OBJTYPE                 Get_OBJTYPE() { return m_tHandle.m_eType; }
    void                    Set_OBJTYPE(OBJTYPE pType) { m_tHandle.m_eType = pType; }

    // 오리진 타입
    void                    Set_OriginType(OBJORIGINTYPE type) { m_tHandle.m_eOriginType = type; }
    OBJORIGINTYPE           Get_OriginType() { return m_tHandle.m_eOriginType; }

    // 층 번호
    void                    SetFloor(const FLOOR& eFloor) { m_tHandle.m_eFloor = eFloor; }
    const                   FLOOR& GetFloor() { return m_tHandle.m_eFloor; }
    // 방 번호
    void                    SetRoomNum(const _ushort& sRoomNum) { m_tHandle.m_sRoomNum = sRoomNum; }
    const _ushort&          GetRoomNum() { return m_tHandle.m_sRoomNum; }
    //핸들 인덱스
    const _int&             Get_HandleIndex() { return m_tHandle.m_iIndex; }
    void                    Set_HandleIndex(const _int& index) { m_tHandle.m_iIndex = index; }
    //
    //핸들 이벤트 인덱스 값
    void                    Set_HandleEventIndex(const _int& index) { m_tHandle.m_sEventIndex = index; }
    const _int&             Get_HandleEventIndex() { return m_tHandle.m_sEventIndex; }

private:
    // 오로지 PoolingManager와 ObjectManager에서 관리할 수 있도록 한다.
    int                     Get_Generation() { return m_tHandle.m_iGeneration; }
    void                    Set_Generation(int iIndex) { m_tHandle.m_iGeneration = iIndex; }

public:
    //파싱용   핸들 번호 , 포지션 , 회전 , 스케일
    void                    Setting_GameObject(const ObjectHandle& tHandle, const _vec3& vPos , const _vec3& vRot , const _vec3 vScale );

public:

    // 액티브
    virtual     void        Set_Active(bool pActive) { m_bActivate = pActive; }
    virtual     bool        Get_Active() { return m_bActivate; }
    // 타겟
    virtual     void        Set_Target(CGameObject* _object) { m_pTarget = _object; }
    CGameObject*            Get_Target() { return m_pTarget; }

    void                    Set_ObjectID(uint32_t ID) { m_iObjectID = ID; }
    uint32_t                Get_ObjectID() {return m_iObjectID; }

public:

    CComponent*             Get_Component(wstring Key, COMPONENTID ID)
    {
        auto it = m_mapComponent[ID].find(Key);

        if (it == m_mapComponent[ID].end())
            return nullptr;

        return it->second;
    }

    void                    Set_ObjectOwner(CGameObject* pOwner) { m_pOwner = pOwner; }
    CGameObject*            Get_ObjectOwner() { return m_pOwner; }

    // 이벤트 매니저에서 체력을 깎으려면 get,set 함수를 들고 있어야한다. 
    // Status를 안쓰는 얘들은 그냥 Component 생성을 해주지 말자.

public:
    //뷰포트
    _float		            Get_ViewZ() const { return m_fViewZ; }
    void			        Compute_ViewZ(const _vec3* pPos);


public:

    void                    SemiUpdate();

protected:
    Engine::CColliderBase* m_pColliderCom = nullptr;
    Engine::CTransform*    m_pTransform = nullptr;
    Engine::CStatus*       m_pStatusCom = nullptr;
    Engine::CTexture*      m_pTextureCom = nullptr;
    
protected:
    // 해쉬 맵을 사용해야함.
    unordered_map<wstring, CComponent*>               m_mapComponent[ID_END];
    // 해당 컴포넌트들을 구분해서 저장하고 관리하겠다는 구조를 나타낸다.
    LPDIRECT3DDEVICE9                                 m_pGraphicDev;
    CGameObject*                                      m_pTarget;
    CGameObject*                                      m_pOwner;     // 총알 때문에 넣음

    _float									          m_fViewZ;
    ObjectHandle                                      m_tHandle;
    uint32_t                                          m_iObjectID;
                                                      
    bool                                              m_bActivate;
    bool                                              m_bCollidable; // 충돌 가능 여부.
    float                                             m_fLifeTime;

private:
    CComponent* Find_Component(COMPONENTID eID, const wstring& pComponentTag);

protected:
    virtual    void         Free();

    friend class CObjectPoolingManager; // 매니저만 접근 가능하게 막는다.
    friend class CObjectManager;



};

END

