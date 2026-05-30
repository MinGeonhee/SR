#include "CEyeOfChaos_Eye.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CObjectManager.h"

CEyeOfChaos_Eye::CEyeOfChaos_Eye(LPDIRECT3DDEVICE9 Device)
    : CGameObject(Device) , m_pAnimationCom(nullptr) , m_vControlPos()
{
    m_ePlacementType = MAPOBJECT_PLACEMENT::WALL;
}

CEyeOfChaos_Eye::CEyeOfChaos_Eye(const CEyeOfChaos_Eye& cpy)
    : CGameObject(cpy) , m_vControlPos(cpy.m_vControlPos) , m_pAnimationCom(nullptr)
{
    m_ePlacementType = MAPOBJECT_PLACEMENT::WALL;
}

CEyeOfChaos_Eye::~CEyeOfChaos_Eye()
{
}

void CEyeOfChaos_Eye::MoveLogic(float DT)
{
    if (m_pTarget->Get_Active() == false)
        return;

    _vec3 vTargetPos = *m_pTarget->Get_Transform()->GetInfoPos();
    _vec3 MyPos = *m_pTransform->GetInfoPos();

    _vec3 vDir = vTargetPos - MyPos;
    

    float Length = D3DXVec3LengthSq(&vDir);
    
    if (Length > 20)
        return;

}

void CEyeOfChaos_Eye::SetControlPoint(float x, float y, float z)
{
    m_vControlPos = { x,y,z };
}

HRESULT CEyeOfChaos_Eye::Ready_GameObject()
{
    if (FAILED(CEyeOfChaos_Eye::Add_Component()))
        return E_FAIL;
    
    m_pTarget = CObjectManager::GetInstance()->Get(OBJ_PLAYER);

    if (m_pTarget == nullptr)
        return E_FAIL;

    m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"EYEOFCHAOS");


    m_pAnimationCom->SetGrid(4, 4); // 열행 ㅋㅋ 열행이러고있네

    m_pAnimationCom->SetRow(0);      //

    m_pAnimationCom->SetCol(1, 1);      // 맨 위 행
    m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
    m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);

    m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
    m_pAnimationCom->PlayAnimation();



    m_pAnimationCom->Change_Buffer(m_ePlacementType);

    return S_OK;
}

HRESULT CEyeOfChaos_Eye::Add_Component()
{
    //딱 트랜스폼만 있으면될듯?

    CComponent* newCom = nullptr;

    newCom = m_pTransform = CLONE_PROTOTYPE(PROTOTYPE::PROTO_TRANS, CTransform);
    if (newCom == nullptr)
        return E_FAIL;
    newCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans", newCom });
    
    newCom = m_pAnimationCom = CLONE_PROTOTYPE(PROTOTYPE::PROTO_ANIMATION, CAnimation);
    if (newCom == nullptr)
        return E_FAIL;
    newCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", newCom });


    return S_OK;
}


_int CEyeOfChaos_Eye::Update_GameObject(const _float& fTimeDelta)
{
    MoveLogic(fTimeDelta);

    CGameObject::Update_GameObject(fTimeDelta);

    return 0;
}

void CEyeOfChaos_Eye::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CEyeOfChaos_Eye::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pTextureCom->Set_Texture();
    m_pAnimationCom->Render_Buffer();
}

CEyeOfChaos_Eye* CEyeOfChaos_Eye::Create(LPDIRECT3DDEVICE9 Device)
{
    CEyeOfChaos_Eye* pObj = new CEyeOfChaos_Eye(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CEyeOfChaos_Eye Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}


void CEyeOfChaos_Eye::Free()
{
    CGameObject::Free();
}