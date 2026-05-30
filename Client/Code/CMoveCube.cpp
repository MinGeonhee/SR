#include "pch.h"
#include "CMoveCube.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CStage.h"
#include "CSceneManager.h"
#include "CObjectManager.h"
#include "CManagement.h"
#include "CSoundManager.h"

CMoveCube::CMoveCube(LPDIRECT3DDEVICE9 Device)
	: CEventCube(Device) , m_vTargetPos() , m_fMoveSpeed(0) , m_isMoveEnd(false) , m_isHaveWallDecor(nullptr), m_pTargetWallDecor(nullptr)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_EVENTCUBE;
    m_eCubeEventType = CUBE_EVENT_TYPE::MOVE;
}

CMoveCube::CMoveCube(const CMoveCube& cpy)
    : CEventCube(cpy), m_vTargetPos(cpy.m_vTargetPos), m_fMoveSpeed(cpy.m_fMoveSpeed), m_isMoveEnd(cpy.m_isMoveEnd)
    , m_isHaveWallDecor(cpy.m_isHaveWallDecor), m_pTargetWallDecor(nullptr)
{
}

CMoveCube::~CMoveCube()
{
}

HRESULT		CMoveCube::Add_Component()
{			
    if (FAILED(CEventCube::Add_Component()))
        return E_FAIL;

    CComponent* newCom = nullptr;

    newCom = m_pBuffer = CLONE_PROTOTYPE(PROTO_CUBETEX, CCubeTex);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer" , newCom });
    m_pBuffer->Set_MyOwner(this);

    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider" , newCom });
    m_pColliderCom->Set_MyOwner(this);

    return S_OK;
			
}	
HRESULT     CMoveCube::Ready_GameObject()
{	
    if (FAILED(CMoveCube::Add_Component()))
        return E_FAIL;


    m_fMoveSpeed = 1.f;

    return S_OK;
}	

CMoveCube* CMoveCube::Create(LPDIRECT3DDEVICE9 Device)
{
    CMoveCube* pMoveCube = new CMoveCube(Device);

    if (FAILED(pMoveCube->Ready_GameObject()))
    {
        Safe_Release(pMoveCube);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pMoveCube;
}


_int        CMoveCube::Update_GameObject(const _float & fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::Update_GameObject(fTimeDelta);

        if (m_isMoveEnd == false)
        {
            MoveToTargetPos(fTimeDelta);
        }

    }

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX, this);

    return 0;
}			


void        CMoveCube::LateUpdate_GameObject(const _float & fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        //아직 큐브 움직임이 안끝났다면 체크하기
        if (m_isMoveEnd == false)
            CheckMoveEnd();
    }
}			

void        CMoveCube::Render_GameObject()
{
    //렌더 
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}


void CMoveCube::PlayEvent()
{
    CGameObject* pTargetPointCube = CObjectManager::GetInstance()->GetPositionCube(this->m_tHandle.m_sEventIndex);
    //절대 맵 데코왜에는 달지마..

    NULLPTR_RETURN(pTargetPointCube);

    CSoundManager::GetInstance()->SoundPlay(L"wall-slide.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

    //없다면?
    m_pTargetWallDecor = 
        static_cast<CMapObject*>(CObjectManager::GetInstance()->Get_EventIndex_Object(OBJTYPE::OBJ_MAPDECOR,this->m_tHandle.m_sEventIndex));
    
    if (m_pTargetWallDecor != nullptr)
    {
        m_isHaveWallDecor = true;
        m_pTargetWallDecor->Set_Event(true);
    }

    //1회성 이벤트들은 이벤트 발동시 True 로 바꿔주기

    if (m_isEventTriggered = true);
    m_IsEvent = true;

    m_vTargetPos = *pTargetPointCube->Get_Transform()->GetInfoPos();


    m_vDir = m_vTargetPos - *m_pTransform->GetInfoPos();

    D3DXVec3Normalize(&m_vDir,&m_vDir);


    
    auto Type = CSceneManager::GetInstance()->Get_CurrentSceneType();

    if (Type != SCENETYPE::STAGE1) return;
    CScene* pCurScene =  CSceneManager::GetInstance()->Get_CurrentScene();

    static_cast<CStage*>(pCurScene)->Plus_TreasueCount();
}

void CMoveCube::MoveToTargetPos(const _float& DT)
{
    auto MyPos = *m_pTransform->GetInfoPos();

    m_pTransform->Move_Pos(&m_vDir , m_fMoveSpeed , DT);

    if (m_isHaveWallDecor == true)
    {
        if (m_pTargetWallDecor != nullptr)
        {
            m_pTargetWallDecor->Get_Transform()->Move_Pos(&m_vDir, m_fMoveSpeed - 0.05f, DT);
        }
    }
}

void CMoveCube::CheckMoveEnd()
{
    const float EPSILON = 0.01f; // 허용 오차

    D3DXVECTOR3 vDiff = m_vTargetPos - *m_pTransform->GetInfoPos();

    // 두 위치값이 일치한다면
    if (D3DXVec3Length(&vDiff) < EPSILON)
    {
        CSoundManager::GetInstance()->SoundPlay(L"wall-stop.wav", EVENT_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

        m_isMoveEnd = true;
        m_IsEvent = false;

        if (m_isHaveWallDecor)
            m_pTargetWallDecor->Set_Event(false);
    }
    else
        return;
}




void		CMoveCube::Free()
{
	CGameObject::Free();
}