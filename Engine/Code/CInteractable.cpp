#include "CInteractable.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CEnumStringFunction.h"
#include "CPathFunction.h"
#include "CMapObjectTex.h"
#include "JsonFunction.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CParticleSystem_Sparkle.h"



CInteractable::CInteractable(LPDIRECT3DDEVICE9 pGraphicDev)
    : CEventObject(pGraphicDev) , m_IsHaveEndSprite(false) , m_IsLastSprite(false) , m_tUVRect() , m_eInteractType(INTERACT_TYPE::_END) , m_pStatus(nullptr)
     , m_iHitToBreak(1000000) , m_iHealAmount(0) , m_isCanBreak(false) , m_isCanInteraction(true) , m_pBillboardCom(nullptr)
    , m_iOrbCount(1)
    , m_iDropItemCount(0)
{
    m_tHandle.m_eType = OBJ_MAPEVENT;
    m_eCategory = MAPOBJECT_CATEGORY::OBSTACLE;
    m_ePlacement = MAPOBJECT_PLACEMENT::_END;

}

CInteractable::CInteractable(const CInteractable& rhs) 
    : CEventObject(rhs) , m_eInteractType(rhs.m_eInteractType)
{
    m_tHandle.m_eType = OBJ_MAPEVENT;
    m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CInteractable::~CInteractable()
{

}


_int CInteractable::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);


    CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);


    return 0;
}


void  CInteractable::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
    else
        if( m_pBillboardCom != nullptr)
            m_pBillboardCom->LateUpdate_Component(fTimeDelta);
}


void    CInteractable::Render_GameObject()
{
    m_pTextureCom->Set_Texture();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

void CInteractable::Change_UVBuffer(const UVRect& tUVRect)
{
    m_pBuffer->Change_Buffer(tUVRect);
}

void CInteractable::Minus_HitToBreak()
{
    if (!m_isCanBreak)
        return;

    --m_iHitToBreak;

    if (m_iHitToBreak <= 0)
        m_isCanBreak = false;
    else
        return;

}

void CInteractable::Setting_Billborad()
{
    switch (m_eInteractType)
    {   // 빌보드 필요없는 것들은 생성 x  
    case Engine::INTERACT_TYPE::UNKNOWN:
    case Engine::INTERACT_TYPE::SWITCH:
    case Engine::INTERACT_TYPE::PHOTOSWITCH:
    case Engine::INTERACT_TYPE::SKULLSWITCH:
    case Engine::INTERACT_TYPE::WALLCHEST:
    case Engine::INTERACT_TYPE::ORBFIELD:
    case Engine::INTERACT_TYPE::BROKEWALL:
    case Engine::INTERACT_TYPE::_END:
        return;
    default:
        break;
    }

    CComponent* newCom = nullptr;

    // ============ 빌보드 컴포넌트 ===============

    newCom = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);
    if (newCom == nullptr)
    {
        MSG_BOX("CInteractable 세팅중 빌보드 컴포넌트 생성 실패");
        return;
    }

    m_pBillboardCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",newCom });

    // =======================================

}

void CInteractable::Setting_HitToBreak()
{
    switch (m_eInteractType)
    {
    case Engine::INTERACT_TYPE::ANTENAE: 
    case Engine::INTERACT_TYPE::ORB:
        m_iHitToBreak = 5;
        break;
    case Engine::INTERACT_TYPE::BARREL:
    case Engine::INTERACT_TYPE::BEERTABLE:
    case Engine::INTERACT_TYPE::BUSH:
    case Engine::INTERACT_TYPE::FLOWERBUSH:
    case Engine::INTERACT_TYPE::FLOWERPOT:
    case Engine::INTERACT_TYPE::LONGPOT:
    case Engine::INTERACT_TYPE::FATPOT:
    case Engine::INTERACT_TYPE::DUMMY:
    case Engine::INTERACT_TYPE::CORPSE:
    case Engine::INTERACT_TYPE::BOOK:
    case Engine::INTERACT_TYPE::POUCH:
    case Engine::INTERACT_TYPE::BROKEWALL:
        m_iHitToBreak = 1;
        break;
    default:
        return;
    }

    m_isCanBreak = true;

    return;
}

void CInteractable::Setting_HeadAmount()
{
    switch (m_eInteractType)
    {
    case Engine::INTERACT_TYPE::BEERTABLE:
        m_iHealAmount = 10;
        break;
    case Engine::INTERACT_TYPE::CHICKENTABLE:
        m_iHealAmount = 15;
        break;
    case Engine::INTERACT_TYPE::FLOWERBUSH: 
        m_iHealAmount = 3;
        break;
    case Engine::INTERACT_TYPE::WELL:
        m_iHealAmount = 5;
        break;
    default:
        return;
    }

    m_isCanInteraction = true;
}

void CInteractable::Setting_DropItemCount()
{
    switch (m_eInteractType)
    {
    case Engine::INTERACT_TYPE::BARREL:
        m_iDropItemCount = 2;
        break;
    case Engine::INTERACT_TYPE::CORPSE:
        m_iDropItemCount = 1;
        break;
    case Engine::INTERACT_TYPE::CHEST:
        m_iDropItemCount = 5;
        break;
    case Engine::INTERACT_TYPE::GOLDCHEST:
        m_iDropItemCount = 10;
        break;
    case Engine::INTERACT_TYPE::WALLCHEST:
        m_iDropItemCount = 5;
        break;
    default:    return;
    }
}

void CInteractable::Change_SpriteToLast_ByCollision()
{
    if (m_IsHaveEndSprite == true)
    {
        CMapObject::Change_UV_Scale(m_tUVRect[LAST_SPRITE]);
        if (m_pColliderCom != nullptr)
            m_pColliderCom->Set_ColliderType(COLLIDER_NONE);
    }
    else
        this->Set_Active(false);
}

//상호작용으로 인한 다음 스프라이트 재생
void CInteractable::Change_SpriteToLast_ByIneraction()
{
    if (m_IsHaveEndSprite == true)
        CMapObject::Change_UV_Scale(m_tUVRect[LAST_SPRITE]);
}

HRESULT CInteractable::Ready_GameObject()
{
    // 트랜스폼 , 버퍼 생성완료
    if (FAILED(CInteractable::Add_Component()))
        return E_FAIL;

    m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Interaction");

    return S_OK;
}


HRESULT CInteractable::Add_Component()
{
    if (FAILED(CMapObject::Add_Component()))
        return E_FAIL;

    // ----------------------   SPHERE 콜라이더 ----------------------
    CComponent* newCom = nullptr;
    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
    if (newCom == nullptr)
        return E_FAIL;

    m_pColliderCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", newCom });
    

    //------------------------------------------------------------------

}

CInteractable* CInteractable::Create(LPDIRECT3DDEVICE9 Device)
{
    CInteractable* pObj = new CInteractable(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CGroundDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }
  
    return pObj;
}

void CInteractable::Setting_EventObject(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale, const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const EVENT_PATH& EventPath)
{
    
    m_eInteractType = static_cast<INTERACT_TYPE>(EventPath);
    m_eCategory = eCat;
    m_ePlacement = ePlacement;

    //스프라이트가 만들어진 다음에 MapObject 세팅 실행해준다
    Setting_Sprite();
    //그 다음 컴포넌트 생성
    Setting_Component();
    //세부적으로 들어갈 세팅값? 나중에 필요할까봐 넣어두긴함.
    Setting_Detailed();


    CMapObject::Setting(tHandle,vPos, vRot, vScale, eCat, ePlacement, m_tUVRect[FIRST_SPRITE]);
    CGameObject::Set_Handle(tHandle);
   

    m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"Interaction");

    if (m_eInteractType == INTERACT_TYPE::CORPSE)
    {
        Play_Particle_Sparkle();
    }

}


void CInteractable::Setting_Sprite()
{
    string sFilePath = CPathFunction::Interaction_Json_Path_InEngine;



    ifstream is(sFilePath);

    nlohmann::ordered_json jLoad;

    //파일 열지못했다면 리턴
    if (!is.is_open())
        return;

    is >> jLoad;



    string Obj_Name = CEnumStringFunction::INTERACT_TYPE_Enum_to_String(m_eInteractType);

    SpriteRect FirstSprite;
    FirstSprite.Clear();



    for (auto& t : jLoad["frames"])
    {
        //맞는 파일이름을 찾게되면 그 정보를 들고온뒤
        if (t["filename"].get<string>() == Obj_Name)
        {
            FirstSprite.x = t["frame"]["x"];
            FirstSprite.y = t["frame"]["y"];
            FirstSprite.w = t["frame"]["w"];
            FirstSprite.h = t["frame"]["h"];



            FirstSprite.Mw = jLoad["meta"]["size"]["w"];
            FirstSprite.Mh = jLoad["meta"]["size"]["h"];

            m_tUVRect[FIRST_SPRITE].SettingFromSprite(FirstSprite);
        }

        if (t["filename"].get<string>() == Obj_Name + "LAST")
        {
            SpriteRect LastSprite;
            LastSprite.Clear();

            LastSprite.x = t["frame"]["x"];
            LastSprite.y = t["frame"]["y"];
            LastSprite.w = t["frame"]["w"];
            LastSprite.h = t["frame"]["h"];


            LastSprite.Mw = jLoad["meta"]["size"]["w"];
            LastSprite.Mh = jLoad["meta"]["size"]["h"];
            m_tUVRect[LAST_SPRITE].SettingFromSprite(LastSprite);

            m_IsHaveEndSprite = true;
        }
    }

}

void CInteractable::Setting_Component()
{
    Setting_Billborad();
}

void CInteractable::Setting_Detailed()
{
    Setting_HitToBreak();
    Setting_HeadAmount();
    Setting_DropItemCount();
}




void    CInteractable::Free()
{
    CGameObject::Free();
}

void CInteractable::Play_Particle_Sparkle()
{
    m_pSparkleParticleSystem = dynamic_cast<CParticleSystem_Sparkle*>(CObjectPoolingManager::GetInstance()->
        Get(OBJ_PARTICLE_SYSTEM_SPARKLE));
    m_pSparkleParticleSystem->Set_Target(this);
    m_pSparkleParticleSystem->Activate_ParticleSystem();
    m_pSparkleParticleSystem->Apply_Values_To_Particles();
}

void CInteractable::Remove_Particle_Sparkle()
{
    m_pSparkleParticleSystem->Set_ParticleSystemOff(true);
}
