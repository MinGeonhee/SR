#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CCollisionManager.h"

BEGIN(Engine)

class CGameObject;
class CToolBufferRect;
class CToolBufferCube;
struct EVENT;
// 이벤트 매니저에서 내가 등록한 오브젝트 간의 상호작용을 담당한다.

class ENGINE_DLL CCollisionEventManager :
    public CBase
{
    DECLARE_SINGLETON(CCollisionEventManager)

private:
    explicit CCollisionEventManager();
    virtual ~CCollisionEventManager();

public:
    _int    Update_Event();
    void    Late_Update();

public:   
    // 충돌 이벤트를 처리해주는 함수
    // ex) 플레이어와 적 탄환과 충돌을 했다.
    void    TriggerCollision_Event( CGameObject* _left , CGameObject* _right);
    void    RayCollision_Event(const RAY_TYPE& eRayType, const RAYINFO& tColliedInfo);
    void    UI_Click_Collision(const RAYINFO& tColliedInfo);

private:

    void    Bullet_CollsionEvent(CGameObject* _left, CGameObject* _right);
    void    Bullet_Env_CollsionEvent(CGameObject* _left, CGameObject* _right);
    void    Door_CollisionEvent(CGameObject* _left, CGameObject* _right);
    void    Item_CollisionEvent(CGameObject* _left, CGameObject* _right);
    void    Wall_Collision(CGameObject* _left, CGameObject* _right);

    void    MapEventObject_Collision(CGameObject* _left, CGameObject* _right);
    void    Interaction_Collision(CGameObject* _left , CGameObject* _right);

    void    Interaction_RayCollision(const RAYINFO& tColliedInfo);
    void    MapObject_RayCollision(EVENT* event, const RAYINFO& tColliedInfo);

    void    EventCube_CollisionEvent_BY_PLAYER(CGameObject* _left, CGameObject* _right);
    void    EventCube_CollisionEvent_BY_BULLET(CGameObject* _left, CGameObject* _right);
    void    EventCube_RayCollision(const RAYINFO& tColliedInfo);

    void    EyeOfChaos_Collision_Event(CGameObject* _left, CGameObject* _right);
    void    EyeOfChaos_RayCollision(const RAYINFO& tColliedInfo);

#pragma region 맵툴 전용 함수 1ㄷ1 매칭해놓은거 보고 놀라지말고 다시 접어라
    // 단순 위치 좌표 출력용 큐브 타일 콜리전 이벤트
    void    Tool_Tile_Floor_None_Click_Collision(const RAYINFO& tColliedInfo);
    void    Tool_Tile_Ceiling_None_Click_Collision(const RAYINFO& tColliedInfo);
    // 큐브 
    void    Tool_Cube_None_Click_Collision(const RAYINFO& tColliedInfo);

    // 타일관련 클릭했을때 생성해줄 콜리전 이벤트
    void    Tool_Tile_Floor_Click_Colision(const RAYINFO& tColliedInfo);
    void    Tool_Tile_Ceiling_Click_Colision(const RAYINFO& tColliedInfo);

    // 큐브관련 클릭했을때 큐브 생성해주는 콜리전 이벤트
    void    Tool_Cube_CLick_Collision(const RAYINFO& tColliedInfo);

    // Floor  Tex Mode Click , None Click
    void    Tool_Tile_Floor_Tex_Click_Collision(const RAYINFO& tColliedInfo);
    void    Tool_Tile_Floor_Tex_None_Click_Collision(const RAYINFO& tColliedInfo);
    // Floor  Tex Mode Click , None Click
    void    Tool_Tile_Ceiling_Tex_Click_Collision(const RAYINFO& tColliedInfo);
    void    Tool_Tile_Ceiling_Tex_None_Click_Collision(const RAYINFO& tColliedInfo);

    // Cube Tex Mode Click , None Click
    void    Tool_Cube_Tex_Click_Collision(const RAYINFO& tColliedInfo);
    void    Tool_Cube_Tex_None_Click_Collision(const RAYINFO& tColliedInfo);

    // Delete Ray Mode
    void    Tool_Delete_None_Click_Collision(const RAYINFO& tColliedInfo);
    void    Tool_Delete_Click_Collision(const RAYINFO& tColliedInfo);
    
    // Info Ray Mode
    void    Tool_Info_None_Click_Collision(const RAYINFO& tColliedInfo);
    void    Tool_Info_Click_Collision(const RAYINFO& tColliedInfo);

    // Object Ray mode
    void    Tool_Object_None_Click_Collision(const RAYINFO& tColliedInfo);

    void    Tool_Object_Click_Collision(const RAYINFO& tColliedInfo);

    

    CToolBufferRect*    MakeToolBufferRect(const _vec3& vPos);
    CToolBufferCube*    MakeToolBufferCube(const _vec3& vPos);


public:
    CGameObject* Gun_Ray_Collision();



#pragma endregion
    // Client 처리

    void    Mace_Collision(const RAYINFO& tColliedInfo);

private:

    virtual void    Free() override;
};

END