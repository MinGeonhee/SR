#include "CCollisionEventManager.h"
#include "CTile.h"
#include "CGraphicDev.h"
#include "CTileManager.h"
#include "CToolBufferRect.h"
#include "CToolBufferCube.h"
#include "CMapToolCube.h"
#include "CCubeManager.h"
#include "CollisionFunc.h"
#include "CTransform.h"
#include "CMonster.h"
#include "CEventBus.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CMapObjectManager.h"
#include "CUIManager.h"
#include "CInputManager.h"
#include "CMapToolTile.h"
#include "CBullet.h"
#include "CStatus.h"
#include "CColliderBase.h"

// 추가 필요 헤더(이 파일에서 직접 사용함)
#include "CParticle.h"
#include "CAAABBB.h"
#include "CCube.h"
#include "CInteractable.h"
#include "CEventCube.h"
#include "CEventObject.h"
#include "CEyeOfChaos.h"

#include <cmath>
using namespace std;

IMPLEMENT_SINGLETON(CCollisionEventManager)

CCollisionEventManager::CCollisionEventManager()
{
}

CCollisionEventManager::~CCollisionEventManager()
{
    Free();
}

_int CCollisionEventManager::Update_Event()
{
    return 0;
}

void CCollisionEventManager::Late_Update()
{
}

void CCollisionEventManager::TriggerCollision_Event(CGameObject* _left, CGameObject* _right)
{
    auto Left_OBJTYPE = _left->Get_OBJTYPE();
    auto Right_OBJTYPE = _right->Get_OBJTYPE();

    // 파티클 바닥 타입 업데이트
    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_PARTICLE && _left->Get_OBJTYPE() == OBJTYPE::OBJ_TILE)
    {
        dynamic_cast<CParticle*>(_right)->Change_OnFloorType(_left->Get_Transform()->GetInfoPos()->y);
    }

    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_PARTICLE && _left->Get_OBJTYPE() == OBJTYPE::OBJ_CUBE)
    {
        dynamic_cast<CParticle*>(_right)->Change_OnFloorType(_left->Get_Transform()->GetInfoPos()->y);
    }

    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_PARTICLE && _left->Get_OBJTYPE() == OBJTYPE::OBJ_EVENTCUBE)
    {
        dynamic_cast<CParticle*>(_right)->Change_OnFloorType(_left->Get_Transform()->GetInfoPos()->y);
    }

    // 문
    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_DOOR)
        Door_CollisionEvent(_left, _right);

    // 총알: 타일/큐브 같은 지형 오브젝트에 맞아도 이벤트는 패스(HEAD 로직 유지)
    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_BULLET
        && _left->Get_OBJTYPE() != OBJTYPE::OBJ_TILE
        && _left->Get_OBJTYPE() != OBJTYPE::OBJ_CUBE)
    {
        Bullet_CollsionEvent(_left, _right);
    }

    // 아이템
    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_ITEM)
        Item_CollisionEvent(_left, _right);

    // 벽 보정(스피어 주체 vs 큐브)
    if (_left->Get_OBJTYPE() == OBJTYPE::OBJ_CUBE)
        Wall_Collision(_right, _left);

    // 벽 보정(스피어 주체 vs 큐브)
    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_MAPOBSTACLE || _right->Get_OBJTYPE() == OBJTYPE::OBJ_MAPEVENT || _right->Get_OBJTYPE() == OBJTYPE::OBJ_CUBE
            || _right->Get_OBJTYPE() == OBJTYPE::OBJ_LIGHT || _right->Get_OBJTYPE() == OBJTYPE::OBJ_CREATURECANTGO)
        Wall_Collision(_left, _right);


    if (Left_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)
    {
        Wall_Collision(_right, _left);
        if (Right_OBJTYPE == OBJTYPE::OBJ_PLAYER)
            EventCube_CollisionEvent_BY_PLAYER(_left, _right);
        if (Right_OBJTYPE == OBJTYPE::OBJ_BULLET)
            EventCube_CollisionEvent_BY_BULLET(_left, _right);
    }
    else if (Right_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)
    {
        Wall_Collision(_left, _right);
        if (Left_OBJTYPE == OBJTYPE::OBJ_PLAYER)
            EventCube_CollisionEvent_BY_PLAYER(_left, _right);
        if (Left_OBJTYPE == OBJTYPE::OBJ_BULLET)
            EventCube_CollisionEvent_BY_BULLET(_left, _right);
    }


    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_BULLET && (_left->Get_OBJTYPE() == OBJTYPE::OBJ_TILE || _left->Get_OBJTYPE() == OBJTYPE::OBJ_CUBE))
        Bullet_Env_CollsionEvent(_left, _right);


    //맵 이벤트 오브젝트 처리 관련 
    if (_right->Get_OBJTYPE() == OBJTYPE::OBJ_MAPEVENT || _left->Get_OBJTYPE() == OBJTYPE::OBJ_MAPEVENT)
    {
        // 맵 이벤트 오브젝트가 현재 다른 충돌체와 비교했을때 enum값이 제일 높을거기 때문에 아마 오른쪽 고정이라고 바도 무방할듯?
         MapEventObject_Collision(_left, _right);
    }
}

void CCollisionEventManager::Bullet_CollsionEvent(CGameObject* _left, CGameObject* _right)
{
    // 여기는 무조건 총알과의 충돌.
    if (_right->Get_ObjectOwner() == _left) return;

    // 오른쪽 왼쪽 둘다 몬스터면 리턴
    if (_right->Get_ObjectOwner()->Get_OBJTYPE() != OBJ_PLAYER && _left->Get_OBJTYPE() != OBJ_PLAYER) return;

    // 오른쪽 총알이 몬스터 총알이면, 베리어와 충돌 x
    if (_right->Get_ObjectOwner()->Get_OBJTYPE() != OBJ_PLAYER && _left->Get_OBJTYPE() == OBJ_BARRIER) return;

    if (_right->Get_ObjectOwner()->Get_OBJTYPE() != OBJ_PLAYER &&
        _left->Get_OBJTYPE() != OBJ_PLAYER)
        return;

    auto bus = CEventBus::GetInstance();
    auto bu = CObjectPoolingManager::GetInstance()->Resolve(_right->Get_Handle());
    auto bullet = static_cast<CBullet*>(bu);

    // 맞은 쪽에게 데미지 이벤트
    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::ONDAMAGED,
        CHANNEL::Combat,
        bu,
        _left,
        bu->Get_Handle(),
        _left->Get_Handle()
    };
    bus->publish(event);

    // 빙결 탄: 스택 증가
    if (bullet->Get_BulletState() == BULLETSTATE::BULLET_PLAYER_ICE)
    {
        auto status = static_cast<CStatus*>(_left->Get_Component(L"Com_Status", ID_DYNAMIC));   if (status == NULL) return;
        status->Add_FrozenCount();
    }


    // 총알 제거 및 콜라이더 무력화
    if (_left->Get_OBJTYPE() != OBJ_BARRIER) {
        bullet->Set_BulletStatus(BULLET_STATUS::BULLET_STATUS_DEAD);
        static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_NONE);
    }
}

void CCollisionEventManager::Item_CollisionEvent(CGameObject* _left, CGameObject* _right)
{
    auto bus = CEventBus::GetInstance();

    // 
    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::COLLISION_ENTER,
        CHANNEL::ITEM,
        _left,
        _right,
        _left->Get_Handle(),
        _right->Get_Handle()
    };
    bus->publish(event);

}

void CCollisionEventManager::Bullet_Env_CollsionEvent(CGameObject* _left, CGameObject* _right)
{
    // 여기는 무조건 총알과의 충돌.
    if (_right->Get_ObjectOwner() == _left)
        return;

    auto bus = CEventBus::GetInstance();
    auto bu = CObjectPoolingManager::GetInstance()->Resolve(_right->Get_Handle());
    auto bullet = static_cast<CBullet*>(bu);

    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::ONDAMAGED,
        CHANNEL::Combat,
        bu,
        _left,
        bu->Get_Handle(),
        _left->Get_Handle()
    };
    bus->publish(event);

    bullet->Set_BulletStatus(BULLET_STATUS::BULLET_STATUS_DEAD);
    static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_NONE);

}

void CCollisionEventManager::RayCollision_Event(const RAY_TYPE& eRayType, const RAYINFO& tColliedInfo)
{
    switch (eRayType)
    {
    case Engine::RAY_NONE:
        break;
    case Engine::GUN_RAY:
        // 총 레이는 여기서 직접 처리 안 함 (총알 시스템에서 처리)
        break;
    case Engine::UI_RAY:
        break;
    case Engine::MACE_RAY:
        Mace_Collision(tColliedInfo);
        break;
    case Engine::PICKING_RAY:
        break;
    case Engine::INTERACTION_RAY:
        Interaction_RayCollision(tColliedInfo);
        break;
    case Engine::EVENT_CUBE_RAY:
        EventCube_RayCollision(tColliedInfo);
        break;
    case Engine::EYEOFCHAOS_RAY:
        EyeOfChaos_RayCollision(tColliedInfo);
        break;
#pragma region 맵툴 전용 광선 처리 (건드리지 말 것)
        // 단순 위치 렉트 출력용 (타일 모드)
    case Engine::MAPTOOL_TILE_FLOOR_NONE_CLICK_RAY:
        Tool_Tile_Floor_None_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_TILE_CEILING_NONE_CLICK_RAY:
        Tool_Tile_Ceiling_None_Click_Collision(tColliedInfo);
        break;

        // 단순 위치 좌표 출력용 (큐브 모드)
    case Engine::MAPTOOL_CUBE_NONE_CLICK_RAY:
        Tool_Cube_None_Click_Collision(tColliedInfo);
        break;

        // 생성 계열
    case Engine::MAPTOOL_TILE_FLOOR_CLICK_RAY:
        Tool_Tile_Floor_Click_Colision(tColliedInfo);
        break;
    case Engine::MAPTOOL_TILE_CEILING_CLICK_RAY:
        Tool_Tile_Ceiling_Click_Colision(tColliedInfo);
        break;
    case Engine::MAPTOOL_CUBE_CLICK_RAY:
        Tool_Cube_CLick_Collision(tColliedInfo);
        break;

        // 텍스처 모드 - 타일
    case Engine::MAPTOOL_TILE_FLOOR_CLICK_TEX_RAY:
        Tool_Tile_Floor_Tex_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_TILE_FLOOR_NONE_CLICK_TEX_RAY:
        Tool_Tile_Floor_Tex_None_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_TILE_CEILING_CLICK_TEX_RAY:
        Tool_Tile_Ceiling_Tex_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_TILE_CEILING_NONE_CLICK_TEX_RAY:
        Tool_Tile_Ceiling_Tex_None_Click_Collision(tColliedInfo);
        break;

        // 텍스처 모드 - 큐브
    case Engine::MAPTOOL_CUBE_CLICK_TEX_RAY:
        Tool_Cube_Tex_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_CUBE_NONE_CLICK_TEX_RAY:
        Tool_Cube_Tex_None_Click_Collision(tColliedInfo);
        break;

        // 삭제 모드
    case Engine::MAPTOOL_DELETE_NONE_CLICK_RAY:
        Tool_Delete_None_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_DELETE_CLICK_RAY:
        Tool_Delete_Click_Collision(tColliedInfo);
        break;

        // 정보(Info) 모드
    case Engine::MAPTOOL_NONE_CLICK_INFO_RAY:
        Tool_Info_None_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_CLICK_INFO_RAY:
        Tool_Info_Click_Collision(tColliedInfo);
        break;
        // 오브젝트 모드
    case Engine::MAPTOOL_OBJECT_NONE_CLICK_RAY:
        Tool_Object_None_Click_Collision(tColliedInfo);
        break;
    case Engine::MAPTOOL_OBJECT_CLICK_RAY:
        Tool_Object_Click_Collision(tColliedInfo);
        break;
#pragma endregion

    case Engine::END_RAY:
        break;
    default:
        break;
    }
}

#pragma region 맵툴 전용 충돌 이벤트 처리 (진짜 건들지 말 것)

// ------- 위치 가이드(버퍼 렉트/큐브) -------
void CCollisionEventManager::Tool_Tile_Floor_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pTile->Get_TileType() == TILETYPE::TILE_NONE)
    {
        CToolBufferRect* BufferRect = MakeToolBufferRect(*pTile->Get_Transform()->GetInfoPos());
        CTileManager::GetInstance()->Add_BufferRect(BufferRect);
    }
}

void CCollisionEventManager::Tool_Tile_Ceiling_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pTile->Get_TileType() == TILETYPE::TILE_NONE || pTile->Get_TileType() == TILETYPE::TILE_FLOOR)
    {
        _vec3 vPos = *pTile->Get_Transform()->GetInfoPos();
        _vec3 vScale = pTile->Get_Transform()->Get_Scale();
        vPos.y += vScale.y;
        CToolBufferRect* BufferRect = MakeToolBufferRect(vPos);
        CTileManager::GetInstance()->Add_BufferRect(BufferRect);
    }
}

void CCollisionEventManager::Tool_Cube_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = dynamic_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (!pColliedTile) return;

    if (pColliedTile->IsCubeOnTop())
        return;

    if (pColliedTile->Get_TileType() == TILETYPE::TILE_NONE || pColliedTile->Get_TileType() == TILETYPE::TILE_FLOOR)
    {
        _vec3 vPos = *pColliedTile->Get_Transform()->GetInfoPos();

        CToolBufferCube* BufferCube = MakeToolBufferCube(vPos);
        BufferCube->ChangeColor(_vec4{ 155 , 0 , 0 , 50 });

        CCubeManager::GetInstance()->Add_BufferCube(BufferCube);
    }
}

// ------- 생성(타일/천장/큐브) -------
void CCollisionEventManager::Tool_Tile_Floor_Click_Colision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pTile->Get_TileType() == TILETYPE::TILE_NONE)
    {
        pTile->Set_TileType(TILETYPE::TILE_FLOOR);
        pTile->SetRoomNum(CTileManager::GetInstance()->GetSettingRoomNum());
        //인덱스 번호 세팅
        pTile->Set_HandleIndex(CTileManager::GetInstance()->GetCurIndex());
        CTileManager::GetInstance()->PlusIndex();
    }
}

void CCollisionEventManager::Tool_Tile_Ceiling_Click_Colision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);

    if (pColliedTile->Get_TileType() == TILE_CEILING)
        return;

    // 바닥이 NONE/FLOOR일 때만 천장 생성, 이미 천장 있으면 스킵
    if (!pColliedTile->IsHaveCeiling())
    {
        pColliedTile->Set_IsHaveCeiling(true);

        _vec3 vPos = *pColliedTile->Get_Transform()->GetInfoPos();
        vPos.y += CTileManager::GetInstance()->GetTileScale().y - MARGIN;

        CMapToolTile* pNewTile = CMapToolTile::Create(
            CGraphicDev::GetInstance()->Get_GraphicDev(),
            CTileManager::GetInstance()->GetCurFloor(),
            CTileManager::GetInstance()->GetSettingRoomNum(),
            vPos
        );

        pNewTile->Set_TileType(TILE_CEILING);
        pNewTile->SetRoomNum(CTileManager::GetInstance()->GetSettingRoomNum());
        pNewTile->Set_Target(pColliedTile); // 천장의 타겟은 아래 바닥
        pNewTile->SetTargetMakeIndex(pColliedTile->Get_MakeIndex());

        //인덱스 번호 세팅
        pNewTile->Set_HandleIndex(CTileManager::GetInstance()->GetCurIndex());
        CTileManager::GetInstance()->PlusIndex();

        CTileManager::GetInstance()->Add_Tile(pNewTile);
    }

}

void CCollisionEventManager::Tool_Cube_CLick_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);

    if (!pColliedTile) return;

    if (pColliedTile->IsCubeOnTop())
        return;

    if (pColliedTile->Get_TileType() == TILETYPE::TILE_CEILING)
        return;

    _vec3 vPos = *pColliedTile->Get_Transform()->GetInfoPos();

    CMapToolCube* newCube = CMapToolCube::Create(
        CGraphicDev::GetInstance()->Get_GraphicDev(),
        CCubeManager::GetInstance()->GetCurFloor(),
        CCubeManager::GetInstance()->GetSettingRoomNum(),
        vPos
    );

    //인덱스 번호 세팅
    newCube->Set_HandleIndex(CCubeManager::GetInstance()->GetCurIndex());
    CCubeManager::GetInstance()->PlusIndex();

    pColliedTile->Set_OnTopCube(true);
    pColliedTile->Set_Target(newCube);
    newCube->SetTargetMakeIndex(pColliedTile->Get_MakeIndex());

    CCubeManager::GetInstance()->Add_Cube(newCube);
}

// ------- 텍스처 모드(타일) -------
void CCollisionEventManager::Tool_Tile_Floor_Tex_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pColliedTile->Get_TileType() == TILETYPE::TILE_FLOOR)
    {
        pColliedTile->Change_TextureUV(CTileManager::GetInstance()->GetCenterUV());
    }
}

void CCollisionEventManager::Tool_Tile_Floor_Tex_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pColliedTile->Get_TileType() == TILETYPE::TILE_FLOOR)
    {
        CToolBufferRect* BufferRect = MakeToolBufferRect(*pColliedTile->Get_Transform()->GetInfoPos());
        CTileManager::GetInstance()->Add_BufferRect(BufferRect);
    }
}

void CCollisionEventManager::Tool_Tile_Ceiling_Tex_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pColliedTile->Get_TileType() == TILETYPE::TILE_CEILING)
    {
        pColliedTile->Change_TextureUV(CTileManager::GetInstance()->GetCenterUV());
    }
}

void CCollisionEventManager::Tool_Tile_Ceiling_Tex_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    CMapToolTile* pColliedTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
    if (pColliedTile->Get_TileType() == TILETYPE::TILE_CEILING)
    {
        CToolBufferRect* BufferRect = MakeToolBufferRect(*pColliedTile->Get_Transform()->GetInfoPos());
        CTileManager::GetInstance()->Add_BufferRect(BufferRect);
    }
}

// ------- 텍스처 모드(큐브) -------
void CCollisionEventManager::Tool_Cube_Tex_Click_Collision(const RAYINFO& tColliedInfo)
{
    CCube* pColliedCube = static_cast<CCube*>(tColliedInfo.pGameobject);

    CUBE_FACE face = CollisionFunc::Get_Collied_Face(
        tColliedInfo.m_vHitPos,
        static_cast<CAAABBB*>(pColliedCube->Get_Collide())->Get_Min(),
        static_cast<CAAABBB*>(pColliedCube->Get_Collide())->Get_Max()
    );
    if (face == CUBE_FACE_END) return;

    pColliedCube->Change_CubeTextureFaceUV(face, CCubeManager::GetInstance()->GetCurCenterUV());

}

void CCollisionEventManager::Tool_Cube_Tex_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    CCube* pColliedCube = static_cast<CCube*>(tColliedInfo.pGameobject);
    if (!pColliedCube || pColliedCube->Get_OBJTYPE() != OBJ_CUBE) return;

    CUBE_FACE face = CollisionFunc::Get_Collied_Face(
        tColliedInfo.m_vHitPos,
        static_cast<CAAABBB*>(pColliedCube->Get_Collide())->Get_Min(),
        static_cast<CAAABBB*>(pColliedCube->Get_Collide())->Get_Max()
    );

    _vec3 vPos = *pColliedCube->Get_Transform()->GetInfoPos();
    vPos.y += CCubeManager::GetInstance()->GetCubeScale().y / 2;
    float Offset = 0.1f;

    // 각 면에 맞춰 가이드 렉트 생성 + 회전
    if (face == CUBE_FACE_NEGATIVE_Z)
    {
        vPos.z -= CCubeManager::GetInstance()->GetCubeScale().z / 2 + Offset;
        CToolBufferRect* r = CToolBufferRect::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
        r->Get_Transform()->Rotation(ROT_X, D3DXToRadian(90.f));
        CTileManager::GetInstance()->Add_BufferRect(r);
        return;
    }
    else if (face == CUBE_FACE_POSITIVE_Z)
    {
        vPos.z += CCubeManager::GetInstance()->GetCubeScale().z / 2 + Offset;
        CToolBufferRect* r = CToolBufferRect::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
        r->Get_Transform()->Rotation(ROT_X, D3DXToRadian(90.f));
        CTileManager::GetInstance()->Add_BufferRect(r);
        return;
    }
    else if (face == CUBE_FACE_NEGATIVE_X)
    {
        vPos.x -= CCubeManager::GetInstance()->GetCubeScale().x / 2 + Offset;
        CToolBufferRect* r = CToolBufferRect::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
        r->Get_Transform()->Rotation(ROT_Z, D3DXToRadian(90.f));
        CTileManager::GetInstance()->Add_BufferRect(r);
        return;
    }
    else if (face == CUBE_FACE_POSITIVE_X)
    {
        vPos.x += CCubeManager::GetInstance()->GetCubeScale().x / 2 + Offset;
        CToolBufferRect* r = CToolBufferRect::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
        r->Get_Transform()->Rotation(ROT_Z, D3DXToRadian(90.f));
        CTileManager::GetInstance()->Add_BufferRect(r);
        return;
    }
    else if (face == CUBE_FACE_POSITIVE_Y)
    {
        vPos.y += CCubeManager::GetInstance()->GetCubeScale().y / 2 + Offset;
        CToolBufferRect* r = CToolBufferRect::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
        CTileManager::GetInstance()->Add_BufferRect(r);
        return;
    }
}

// ------- 삭제 모드 -------
void CCollisionEventManager::Tool_Delete_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    OBJTYPE type = tColliedInfo.pGameobject->Get_OBJTYPE();

    if (type == OBJ_TILE)
    {
        CMapToolTile* t = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
        if (t->Get_TileType() == TILETYPE::TILE_NONE) return;

        CToolBufferRect* r = MakeToolBufferRect(*t->Get_Transform()->GetInfoPos());
        CTileManager::GetInstance()->Add_BufferRect(r);
    }
    else if (type == OBJ_CUBE)
    {
        CToolBufferCube* c = MakeToolBufferCube(*tColliedInfo.pGameobject->Get_Transform()->GetInfoPos());
        CCubeManager::GetInstance()->Add_BufferCube(c);
    }
}

void CCollisionEventManager::Tool_Delete_Click_Collision(const RAYINFO& tColliedInfo)
{
    OBJTYPE type = tColliedInfo.pGameobject->Get_OBJTYPE();

    if (type == OBJ_TILE)
    {
        CMapToolTile* pTile = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
        TILETYPE tileType = pTile->Get_TileType();

        if (tileType == TILETYPE::TILE_NONE)
            return;
        else if (tileType == TILETYPE::TILE_FLOOR)
        {
            pTile->Change_TextureUV(_vec2(DEFUALT_TILE_UV_CENTERX, DEFUALT_TILE_UV_CENTERY));
            CTileManager::GetInstance()->Setting_HandleIndex_AfterDelete(pTile->Get_HandleIndex());
            pTile->Set_HandleIndex(-1);
            pTile->Set_TileType(TILE_NONE);
            return;
        }
        else if (tileType == TILETYPE::TILE_CEILING)
        {
            //Make Index번호로 접근해서 세팅값 건드리기
            CMapToolTile* pTargetTile = CTileManager::GetInstance()->GetMapToolTile_ByMakeIndex(pTile->GetFloor(), pTile->GetTargetMakeIndex());
            pTargetTile->Set_IsHaveCeiling(false);

            CTileManager::GetInstance()->Setting_HandleIndex_AfterDelete(pTile->Get_HandleIndex());
            CTileManager::GetInstance()->MinusIndex();
            CTileManager::GetInstance()->Delete_Tile(pTile);
            pTile = nullptr;
        }
    }
    else if (type == OBJ_CUBE)
    {
        CMapToolCube* pCMapToolCube = static_cast<CMapToolCube*>(tColliedInfo.pGameobject);

        //Make Index번호로 접근해서 세팅값 건드리기
        CMapToolTile* pTargetTile = CTileManager::GetInstance()->GetMapToolTile_ByMakeIndex(pCMapToolCube->GetFloor(), pCMapToolCube->GetTargetMakeIndex());
        pTargetTile->Set_OnTopCube(false);

        CCubeManager::GetInstance()->Delete_Cube(pCMapToolCube);
        pCMapToolCube = nullptr;
    }
    else if (type == OBJ_MAPOBSTACLE || type == OBJ_MAPDECOR || type == OBJ_MAPEVENT || type == OBJ_ITEM || type == OBJ_MONSTER || type == OBJ_DOOR || type == OBJ_LIGHT)
    {
        // 맵툴용 데코 , 방해물 , 이벤트 관련 오브젝트
        CMapObjectManager::GetInstance()->Delete_MapObject(tColliedInfo.pGameobject);
    }
}

// ------- 정보(Info) 모드 -------
void CCollisionEventManager::Tool_Info_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    OBJTYPE type = tColliedInfo.pGameobject->Get_OBJTYPE();

    if (type == OBJ_TILE)
    {
        CMapToolTile* t = static_cast<CMapToolTile*>(tColliedInfo.pGameobject);
        CToolBufferRect* r = MakeToolBufferRect(*t->Get_Transform()->GetInfoPos());
        r->Change_Color(_vec4{ 155.f,0.f,0.f, 200.f });
        CTileManager::GetInstance()->Add_BufferRect(r);
    }
    else if (type == OBJ_CUBE)
    {
        CToolBufferCube* c = MakeToolBufferCube(*tColliedInfo.pGameobject->Get_Transform()->GetInfoPos());
        c->ChangeColor(_vec4{ 0.f,0.f,155.f, 200.f });
        CCubeManager::GetInstance()->Add_BufferCube(c);
    }
}

void CCollisionEventManager::Tool_Info_Click_Collision(const RAYINFO& tColliedInfo)
{
    OBJTYPE type = tColliedInfo.pGameobject->Get_OBJTYPE();

    CTileManager::GetInstance()->Delete_BufferInfoRect();
    CCubeManager::GetInstance()->Delete_BufferInfoCube();

    if (type == OBJ_TILE)
    {
        CTileManager::GetInstance()->SetTargetObject(tColliedInfo.pGameobject);
        CCubeManager::GetInstance()->SetTargetObject(nullptr);
        CMapObjectManager::GetInstance()->SetInfoMapObject(nullptr);

        _vec3 vPos = *tColliedInfo.pGameobject->Get_Transform()->GetInfoPos();
        CToolBufferRect* r = MakeToolBufferRect(vPos);
        r->Set_Target(tColliedInfo.pGameobject);
        r->Change_Color(_vec4{ 155.f,0.f,0.f, 200.f });
        CTileManager::GetInstance()->Add_BufferInfoRect(r);
    }
    else if (type == OBJ_CUBE)
    {
        CCubeManager::GetInstance()->SetTargetObject(tColliedInfo.pGameobject);
        CTileManager::GetInstance()->SetTargetObject(nullptr);
        CMapObjectManager::GetInstance()->SetInfoMapObject(nullptr);

        _vec3 vPos = *tColliedInfo.pGameobject->Get_Transform()->GetInfoPos();
        CToolBufferCube* c = MakeToolBufferCube(vPos);
        c->Set_Target(tColliedInfo.pGameobject);
        c->ChangeColor(_vec4{ 0.f,0.f,155.f, 200.f });
        CCubeManager::GetInstance()->Add_BufferInfoCube(c);
    }
    else if (type == OBJ_MAPOBSTACLE || type == OBJ_MAPDECOR || type == OBJ_MAPEVENT || type == OBJ_ITEM || type == OBJ_MONSTER || type == OBJ_DOOR
        || type == OBJ_LIGHT)
    {
        CCubeManager::GetInstance()->SetTargetObject(nullptr);
        CTileManager::GetInstance()->SetTargetObject(nullptr);
        CMapObjectManager::GetInstance()->SetInfoMapObject(static_cast<CMapToolMapObject*>(tColliedInfo.pGameobject));
    }
}

void CCollisionEventManager::Tool_Object_None_Click_Collision(const RAYINFO& tColliedInfo)
{
    auto Mgr = CMapObjectManager::GetInstance();
    MAPOBJECT_PLACEMENT eType = Mgr->GetCurPlacement();

    if (eType == MAPOBJECT_PLACEMENT::GROUND)
    {
        if (tColliedInfo.pGameobject->Get_OBJTYPE() != OBJ_TILE)
            return;

        if (static_cast<CMapToolTile*>(tColliedInfo.pGameobject)->Get_TileType() == TILETYPE::TILE_CEILING)
            return;


        _vec3 vPos = tColliedInfo.m_vHitPos;

        if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LSHIFT))
        {
            vPos = *tColliedInfo.pGameobject->Get_Transform()->GetInfoPos();
        }

        // 기본 로테이션 세팅 값은 0 0 0 
        Mgr->Add_BufferMapObject(vPos, { 0.f,0.f,0.f });

    }
    else if (eType == MAPOBJECT_PLACEMENT::WALL)
    {
        if (tColliedInfo.pGameobject->Get_OBJTYPE() != OBJ_CUBE)
            return;

        CUBE_FACE face = CollisionFunc::Get_Collied_Face(
            tColliedInfo.m_vHitPos,
            static_cast<CAAABBB*>(tColliedInfo.pGameobject->Get_Collide())->Get_Min(),
            static_cast<CAAABBB*>(tColliedInfo.pGameobject->Get_Collide())->Get_Max()
        );
        if (face == CUBE_FACE_END ||/* face == CUBE_FACE_NEGATIVE_Y ||*/ face == CUBE_FACE_POSITIVE_Y) return;  //위아래 y축 방향은 제외한다

        _vec3 vRot = { 0.f, 0.f ,0.f };
        auto vPos = *tColliedInfo.pGameobject->Get_Transform()->GetInfoPos();
        auto vScale = tColliedInfo.pGameobject->Get_Transform()->Get_Scale();

        vPos.y += vScale.y * 0.5f;
        auto Offset_X = vScale.x * 0.5f + 0.05f;
        auto Offset_Z = vScale.z * 0.5f + 0.05f;

        switch (face)
        {
        case Engine::CUBE_FACE_NEGATIVE_Z:
            vPos.z -= Offset_Z;
            vRot.y = 0.f;
            break;
        case Engine::CUBE_FACE_POSITIVE_Z:
            vPos.z += Offset_Z;
            vRot.y = D3DXToRadian(180.f);
            break;
        case Engine::CUBE_FACE_NEGATIVE_X:
            vPos.x -= Offset_X;
            vRot.y = D3DXToRadian(-90.f);
            break;
        case Engine::CUBE_FACE_POSITIVE_X:
            vPos.x += Offset_X;
            vRot.y = D3DXToRadian(+90.f);
            break;
        default:
            return;
        }

        Mgr->Add_BufferMapObject(vPos, vRot);
    }
}

void CCollisionEventManager::Tool_Object_Click_Collision(const RAYINFO& tColliedInfo)
{
    auto Mgr = CMapObjectManager::GetInstance();
    MAPOBJECT_PLACEMENT eType = Mgr->GetCurPlacement();

    if (eType == MAPOBJECT_PLACEMENT::GROUND)
    {
        if (tColliedInfo.pGameobject->Get_OBJTYPE() != OBJ_TILE)
            return;

        if (static_cast<CMapToolTile*>(tColliedInfo.pGameobject)->Get_TileType() == TILETYPE::TILE_CEILING)
            return;

        _vec3 vPos = tColliedInfo.m_vHitPos;

        if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::LSHIFT))
        {
            vPos = *tColliedInfo.pGameobject->Get_Transform()->GetInfoPos();
        }


        // 기본 로테이션 세팅 값은 0 0 0 
        Mgr->Add_MapObject(vPos, { 0.f,0.f,0.f });

    }
    else if (eType == MAPOBJECT_PLACEMENT::WALL)
    {
        if (tColliedInfo.pGameobject->Get_OBJTYPE() != OBJ_CUBE)
            return;

        CUBE_FACE face = CollisionFunc::Get_Collied_Face(
            tColliedInfo.m_vHitPos,
            static_cast<CAAABBB*>(tColliedInfo.pGameobject->Get_Collide())->Get_Min(),
            static_cast<CAAABBB*>(tColliedInfo.pGameobject->Get_Collide())->Get_Max()
        );
        if (face == CUBE_FACE_END || /*face == CUBE_FACE_NEGATIVE_Y ||*/ face == CUBE_FACE_POSITIVE_Y) return;  //위아래 y축 방향은 제외한다

        _vec3 vRot = { 0.f, 0.f ,0.f };
        auto vPos = *tColliedInfo.pGameobject->Get_Transform()->GetInfoPos();
        auto vScale = tColliedInfo.pGameobject->Get_Transform()->Get_Scale();

        vPos.y += vScale.y * 0.5f;
        auto Offset_X = vScale.x * 0.5f + MARGIN;
        auto Offset_Z = vScale.z * 0.5f + MARGIN;

        switch (face)
        {
        case Engine::CUBE_FACE_NEGATIVE_Z:
            vPos.z -= Offset_Z;
            vRot.y = 0.f;
            break;
        case Engine::CUBE_FACE_POSITIVE_Z:
            vPos.z += Offset_Z;
            vRot.y = D3DXToRadian(180.f);
            break;
        case Engine::CUBE_FACE_NEGATIVE_X:
            vPos.x -= Offset_X;
            vRot.y = D3DXToRadian(-90.f);
            break;
        case Engine::CUBE_FACE_POSITIVE_X:
            vPos.x += Offset_X;
            vRot.y = D3DXToRadian(+90.f);
            break;
        default:
            return;
        }

        Mgr->Add_MapObject(vPos, vRot);
    }
}

// ------- 버퍼 유틸 -------
CToolBufferRect* CCollisionEventManager::MakeToolBufferRect(const _vec3& vPos)
{
    CToolBufferRect* p = CToolBufferRect::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
    if (!p) return nullptr;
    return p;
}

CToolBufferCube* CCollisionEventManager::MakeToolBufferCube(const _vec3& vPos)
{
    CToolBufferCube* p = CToolBufferCube::Create(CGraphicDev::GetInstance()->Get_GraphicDev(), vPos);
    if (!p) return nullptr;
    return p;
}

CGameObject* CCollisionEventManager::Gun_Ray_Collision()
{
    return nullptr;
}

#pragma endregion

void CCollisionEventManager::Mace_Collision(const RAYINFO& tColliedInfo)
{
    if (tColliedInfo.m_fDistance >= 2.f) return;

    auto bus = CEventBus::GetInstance();
    auto player = CObjectManager::GetInstance()->Get(OBJ_PLAYER);

    if (tColliedInfo.pGameobject->Get_OBJTYPE() == OBJ_EVENTCUBE)
    {
        EVENT event;
        event.channel = CHANNEL::EVENTCUBE;
        event.topic = TOPIC::ONDAMAGED;
        event.phase = PHASE::PHASE_BEGIN;
        event.sourcePtr = player;
        event.sourceHandle = player->Get_Handle();
        event.targetPtr = tColliedInfo.pGameobject;
        event.targetHandle = tColliedInfo.pGameobject->Get_Handle();
        bus->publish(event);
    }
    else
    {
        EVENT event
        {
            PHASE::PHASE_BEGIN,
            TOPIC::ONDAMAGED,
            CHANNEL::Combat,
            player,
            tColliedInfo.pGameobject,
            player->Get_Handle(),
            tColliedInfo.pGameobject->Get_Handle()
        };

        bus->publish(event);
    }

    // 1개의 이벤트를 여러명이 읽게 하냐.
    // 구독자 하나가 한개만 읽게만 읽을 수 있게 Topic을 세분화해서 나누느냐.
}

//맵 이벤트 관련 충돌처리
void CCollisionEventManager::MapEventObject_Collision(CGameObject* _left, CGameObject* _right)
{
    if (_right->Get_OBJTYPE() != OBJTYPE::OBJ_MAPEVENT)
        return;

    //right 쪽 타입 검사 화끈하게 조져주고 static_cast로    상남자식 형변환  Dynamic은 에겐남들이나 하는짓
    CEventObject* pEventObj = static_cast<CEventObject*>(_right);

    OBJECT_EVENT_TYPE Type = pEventObj->GetObject_Event_Type();

    switch (Type)
    {
    case Engine::OBJECT_EVENT_TYPE::INTERECT:
        Interaction_Collision(_left, _right);
        break;
    case Engine::OBJECT_EVENT_TYPE::DOOR:
        //Door 는 이미 있긴한데 흐으으음...

    case Engine::OBJECT_EVENT_TYPE::CHAOSOFEYE:
        EyeOfChaos_Collision_Event(_left, _right);
        break;
    default: break;
    }

    if (_left->Get_Handle().m_eType != OBJTYPE::OBJ_BULLET)
        return;
    CBullet* pBullet = static_cast<CBullet*>(_left);
    // 총알 제거 및 콜라이더 무력화
    pBullet->Set_BulletStatus(BULLET_STATUS::BULLET_STATUS_DEAD);
    static_cast<CColliderBase*>(pBullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_NONE);
}

void CCollisionEventManager::Interaction_Collision(CGameObject* _left, CGameObject* _right)
{
    //이미 위에서 타입검사해서 넘어와서 Interaction 클래스라는걸 알기때문에 이번엔 테토남식 static_cast도 쓰지않는다.
    //테토남은 바로 C언어식 형변환  완전 남자다잉~~~
    CInteractable* pInteractable = (CInteractable*)_right;

    const auto& Left_Obj_Type = _left->Get_OBJTYPE();

    const INTERACT_TYPE& Interact_Type = pInteractable->GetInteractType();


    //플레이어와 충돌이면 단순 충돌 UI System 일해줘~
    // Switch 는 UI 매니저에 Use 관련 문구 UI 시스템에 보내준다
    // 음식은 Ui 매니저에 Eat 이라는 관련 문구를 UI 시스템에 보내줘야한다.
    // 시체는 조사한다 라는 관련 문구를 Ui 시스템에 보내줘야한다.

    if (Left_Obj_Type == OBJ_PLAYER)
    {
        switch (Interact_Type)
        {
        case Engine::INTERACT_TYPE::BEERTABLE:
        case Engine::INTERACT_TYPE::CHICKENTABLE:
        case Engine::INTERACT_TYPE::FLOWERBUSH:
        case Engine::INTERACT_TYPE::WELL:
        case Engine::INTERACT_TYPE::SWITCH:
        case Engine::INTERACT_TYPE::PHOTOSWITCH:
        case Engine::INTERACT_TYPE::SKULLSWITCH:
        case Engine::INTERACT_TYPE::CORPSE:
        case Engine::INTERACT_TYPE::CHEST:
        case Engine::INTERACT_TYPE::WALLCHEST:
        case Engine::INTERACT_TYPE::GOLDCHEST:
        case Engine::INTERACT_TYPE::ORB:
            break;
        default: return;
        }

        if (!pInteractable->Get_IsCanInteraction())
            return;


        // Interest 시스템에 보낼 예정
        // UI 관련 처리 예정

        EVENT event
        {
            PHASE::PHASE_BEGIN,
            TOPIC::COLLISION_ENTER,
            CHANNEL::INTERACTION,
            _left,
            _right,
            _left->Get_Handle(),

            _right->Get_Handle()
        };

        CEventBus::GetInstance()->publish(event);

        return;
    }


    // 플레이어의 무기 == bullet 과 충돌이면 컴뱃 시스템에 보내줘야한다.  처리해줄 것만 싹싺 해서 보내주자
    if (Left_Obj_Type == OBJ_BULLET)
    {
        switch (Interact_Type)
        { //생성할 것들 나머지는 return
        case Engine::INTERACT_TYPE::ANTENAE:
        case Engine::INTERACT_TYPE::BARREL:
        case Engine::INTERACT_TYPE::BUSH:
        case Engine::INTERACT_TYPE::FLOWERBUSH:
        case Engine::INTERACT_TYPE::FLOWERPOT:
        case Engine::INTERACT_TYPE::LONGPOT:
        case Engine::INTERACT_TYPE::FATPOT:
        case Engine::INTERACT_TYPE::DUMMY:
        case Engine::INTERACT_TYPE::CORPSE:
        case Engine::INTERACT_TYPE::ORB:
        case Engine::INTERACT_TYPE::ORBFIELD:
        case Engine::INTERACT_TYPE::BOOK:
        case Engine::INTERACT_TYPE::POUCH:
        case Engine::INTERACT_TYPE::BROKEWALL:
            break;
            //나머지는 리턴 때려준다
        default: return;
        }


        //전투시스템에 넣어준다

        EVENT event
        {
            PHASE::PHASE_BEGIN,
            TOPIC::ONDAMAGED,
            CHANNEL::Combat,
            _left,
            _right,
            _left->Get_Handle(),
            _right->Get_Handle()
        };

        CEventBus::GetInstance()->publish(event);

    }
}

void CCollisionEventManager::Interaction_RayCollision(const RAYINFO& tColliedInfo)
{
    // 상호작용 비이이이임을 맞은 아이들이 담겨있는 곳.

    if (!tColliedInfo.pGameobject) return;

    // 사거리 체크: 1 초과면 무시 (우선순위 버그 fix: 원래 '!dist > 1'였음)
    if (tColliedInfo.m_fDistance > 1.f) return;

    auto bus = CEventBus::GetInstance();
    auto OBJtype = tColliedInfo.pGameobject->Get_OBJTYPE();

    EVENT event;

    switch (OBJtype)
    {
    case OBJ_DOOR:
    {
        event.channel = CHANNEL::UI;
        event.topic = TOPIC::DoorUINotice;
        event.phase = PHASE::PHASE_BEGIN;

        bus->publish(event);
        break;
    }

    case OBJ_MAPEVENT:
        MapObject_RayCollision(&event, tColliedInfo);
        break;
    }
}

void CCollisionEventManager::MapObject_RayCollision(EVENT* event, const RAYINFO& tColliedInfo)
{
    if (static_cast<CEventObject*>(tColliedInfo.pGameobject)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::INTERECT)
        return;

    CInteractable* pInteractable = static_cast<CInteractable*>(tColliedInfo.pGameobject);

    INTERACT_TYPE Type = pInteractable->GetInteractType();



    switch (Type)
    {
        //EAT
    case Engine::INTERACT_TYPE::BEERTABLE:
    case Engine::INTERACT_TYPE::CHICKENTABLE:
    case Engine::INTERACT_TYPE::FLOWERBUSH:
    case Engine::INTERACT_TYPE::WELL:
    case Engine::INTERACT_TYPE::SWITCH:
    case Engine::INTERACT_TYPE::PHOTOSWITCH:
    case Engine::INTERACT_TYPE::SKULLSWITCH:
    case Engine::INTERACT_TYPE::CORPSE:
    case Engine::INTERACT_TYPE::CHEST:
    case Engine::INTERACT_TYPE::GOLDCHEST:
    case Engine::INTERACT_TYPE::WALLCHEST:

        event->channel = CHANNEL::UI;
        event->topic = TOPIC::INTERACTNOTICE;
        event->phase = PHASE::PHASE_BEGIN;
        event->targetPtr = tColliedInfo.pGameobject;
        event->targetHandle = tColliedInfo.pGameobject->Get_Handle();

        break;
    default:
        return;
    }

    CEventBus::GetInstance()->publish(*event);

}

void CCollisionEventManager::EventCube_CollisionEvent_BY_PLAYER(CGameObject* _left, CGameObject* _right)
{
    auto Left_OBJTYPE = _left->Get_OBJTYPE(); auto Right_OBJTYPE = _right->Get_OBJTYPE();


    //플레이어와 충돌처리를 할테니까
    CGameObject* pPlayerObj = nullptr;
    CEventCube* pEventCube = nullptr;

    if (Left_OBJTYPE == OBJTYPE::OBJ_PLAYER)                 pPlayerObj = _left;
    else if (Right_OBJTYPE == OBJTYPE::OBJ_PLAYER)           pPlayerObj = _right;
    else                                                     return;

    if (Left_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)              pEventCube = static_cast<CEventCube*>(_left);
    else if (Right_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)        pEventCube = static_cast<CEventCube*>(_right);
    else                                                     return;

    if (pPlayerObj == nullptr || pEventCube == nullptr)
        return;


    CUBE_EVENT_TYPE Event_Cube_Type = pEventCube->GetCubeEventType();

    if (Event_Cube_Type == CUBE_EVENT_TYPE::_END || Event_Cube_Type == CUBE_EVENT_TYPE::DESTRUCTIBLE_ATTACK || Event_Cube_Type == CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC || Event_Cube_Type == CUBE_EVENT_TYPE::POSITION)
        return;

    //플레이어가 벽에 부딫혔을는게 그게 EventCube 일때 발송해준다...

    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::COLLISION_ENTER,
        CHANNEL::EVENTCUBE,
        pPlayerObj,
        pEventCube,
        pPlayerObj->Get_Handle(),
        pEventCube->Get_Handle()
    };

    CEventBus::GetInstance()->publish(event);

}

void CCollisionEventManager::EventCube_CollisionEvent_BY_BULLET(CGameObject* _left, CGameObject* _right)
{
    auto Left_OBJTYPE = _left->Get_OBJTYPE(); auto Right_OBJTYPE = _right->Get_OBJTYPE();

    //플레이어와 충돌처리를 할테니까
    CGameObject* pPlayerObj = nullptr;
    CEventCube* pEventCube = nullptr;

    if (Left_OBJTYPE == OBJTYPE::OBJ_BULLET)                 pPlayerObj = _left;
    else if (Right_OBJTYPE == OBJTYPE::OBJ_BULLET)           pPlayerObj = _right;
    else                                                     return;

    if (Left_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)              pEventCube = static_cast<CEventCube*>(_left);
    else if (Right_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)        pEventCube = static_cast<CEventCube*>(_right);
    else                                                     return;

    if (pPlayerObj == nullptr || pEventCube == nullptr)
        return;

    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::ONDAMAGED,
        CHANNEL::EVENTCUBE,
        pPlayerObj,
        pEventCube,
        pPlayerObj->Get_Handle(),
        pEventCube->Get_Handle()
    };

    CEventBus::GetInstance()->publish(event);
}

void CCollisionEventManager::EventCube_RayCollision(const RAYINFO& tColliedInfo)
{
    NULLPTR_RETURN(tColliedInfo.pGameobject);

    OBJTYPE OBJTYPE = tColliedInfo.pGameobject->Get_OBJTYPE();

    if (OBJTYPE != OBJTYPE::OBJ_EVENTCUBE)
        return;

    CEventCube* pEventCube = static_cast<CEventCube*>(tColliedInfo.pGameobject);

    CUBE_EVENT_TYPE eCubeEventType = pEventCube->GetCubeEventType();

    if (eCubeEventType == CUBE_EVENT_TYPE::_END)
        return;

    auto player = CObjectManager::GetInstance()->Get(OBJ_PLAYER);

    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::RAYCOLLISION,
        CHANNEL::EVENTCUBE,
        player,
        tColliedInfo.pGameobject,
        player->Get_Handle(),
        tColliedInfo.pGameobject->Get_Handle()
    };

    CEventBus::GetInstance()->publish(event);

}

void CCollisionEventManager::EyeOfChaos_Collision_Event(CGameObject* _left, CGameObject* _right)
{
    auto Left_OBJTYPE = _left->Get_OBJTYPE(); auto Right_OBJTYPE = _right->Get_OBJTYPE();

    //플레이어와 충돌처리를 할테니까
    CGameObject* pPlayerObj = nullptr;
    CEventObject* pEventObj = nullptr;

    if (Left_OBJTYPE == OBJTYPE::OBJ_PLAYER)                 pPlayerObj = _left;
    else if (Right_OBJTYPE == OBJTYPE::OBJ_PLAYER)           pPlayerObj = _right;
    else                                                     return;

    if (Left_OBJTYPE == OBJTYPE::OBJ_MAPEVENT)              pEventObj = static_cast<CEventObject*>(_left);
    else if (Right_OBJTYPE == OBJTYPE::OBJ_MAPEVENT)        pEventObj = static_cast<CEventObject*>(_right);
    else                                                     return;


    if (pPlayerObj == nullptr || pEventObj == nullptr)


        if (pEventObj->GetObject_Event_Type() != OBJECT_EVENT_TYPE::CHAOSOFEYE) return;



    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::COLLISION_ENTER,
        CHANNEL::EYEOFCHAOS,
        pPlayerObj,
        pEventObj,
        pPlayerObj->Get_Handle(),
        pEventObj->Get_Handle()
    };

    CEventBus::GetInstance()->publish(event);


}

void CCollisionEventManager::EyeOfChaos_RayCollision(const RAYINFO& tColliedInfo)
{
    if (tColliedInfo.pGameobject->Get_OBJTYPE() != OBJTYPE::OBJ_MAPEVENT) return;
    if (static_cast<CEventObject*>(tColliedInfo.pGameobject)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::CHAOSOFEYE) return;



    EVENT event;

    event.channel = CHANNEL::UI;
    event.topic = TOPIC::STAGE_EXIT;
    event.phase = PHASE::PHASE_BEGIN;
    event.targetPtr = tColliedInfo.pGameobject;
    event.targetHandle = tColliedInfo.pGameobject->Get_Handle();

    CEventBus::GetInstance()->publish(event);


}





void CCollisionEventManager::Door_CollisionEvent(CGameObject* _left, CGameObject* _right)
{
    // 문 Collider와 부딪혔다면: 파고듦 보정
    Wall_Collision(_left, _right);

    // 가볍게 상호작용 Ray를 발사할 수 있도록 설계한다.
    EVENT event
    {
        PHASE::PHASE_BEGIN,
        TOPIC::COLLISION_ENTER,
        CHANNEL::Door,
        _left,
        _right,
        _left->Get_Handle(),
        _right->Get_Handle()
    };

    CEventBus::GetInstance()->publish(event);
}

void CCollisionEventManager::Wall_Collision(CGameObject* _left, CGameObject* _right)
{
    auto* leftCol = _left->Get_Collide();
    if (leftCol->Get_ColliderType() != COLLIDER_SPHERE) return;

    const _vec3 leftPos = *(_left->Get_Transform()->GetInfoPos());
    _vec3 refPos; // 벽에서 "가장 가까운 점"

    auto* rightCol = _right->Get_Collide();

    // 1) 벽이 AABB(AAABBB)인 경우: 스피어 중심을 AABB로 클램프해서 최근접점을 구함
    if (rightCol->Get_ColliderType() == COLLIDER_AAABBB)
    {
        const _vec3 mn = static_cast<CAAABBB*>(rightCol)->Get_Min();
        const _vec3 mx = static_cast<CAAABBB*>(rightCol)->Get_Max();

        refPos.x = (leftPos.x < mn.x) ? mn.x : (leftPos.x > mx.x ? mx.x : leftPos.x);
        refPos.y = (leftPos.y < mn.y) ? mn.y : (leftPos.y > mx.y ? mx.y : leftPos.y);
        refPos.z = (leftPos.z < mn.z) ? mn.z : (leftPos.z > mx.z ? mx.z : leftPos.z);
    }
    else
    {
        // 2) 그 외엔 벽의 중심을 근사치로 사용(정확도↓)
        refPos = *(_right->Get_Transform()->GetInfoPos());
    }

    // diff = 스피어 중심 -> 벽 최근접점
    _vec3 diff = { leftPos.x - refPos.x, leftPos.y - refPos.y, leftPos.z - refPos.z };
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

    const float radius = static_cast<CSphere*>(leftCol)->Get_Radius();
    const float rSq = radius * radius;

    // 충돌 아님(스피어가 밖에 있음) → 반환
    if (distSq >= rSq) return;

    // 완전 겹침(거의 0): 안전한 축으로 한 칸 밀어냄
    const float EPS = 1e-8f;
    if (distSq <= EPS)
    {
        // 벽이 AABB라면, 더 가까운 면의 법선으로 밀어주는 게 이상적이지만
        // 여기선 간단히 +X로 한 번 밀기(필요시 AABB 면법선 사용으로 개선)
        _left->Get_Transform()->Set_Pos(leftPos.x + radius, leftPos.y, leftPos.z);
        return;
    }

    // 관통량 = 반지름 - 실제 거리
    float dist = sqrtf(distSq);
    float penetration = radius - dist;

    // 법선 = diff / dist  (스피어 중심을 refPos에서 바깥쪽으로)
    _vec3 n = { diff.x / dist, diff.y / dist, diff.z / dist };

    _vec3 newPos = { leftPos.x + n.x * penetration,
                     leftPos.y + n.y * penetration,
                     leftPos.z + n.z * penetration };

    _left->Get_Transform()->Set_Pos(newPos.x, newPos.y, newPos.z);
}

void CCollisionEventManager::UI_Click_Collision(const RAYINFO& tColliedInfo)
{

}

void CCollisionEventManager::Free()
{
}
