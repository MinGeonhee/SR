#pragma once
#pragma region 헤더모음

#include "CBase.h"
#include "CComponent.h"
#include "CTriCol.h"
#include "CTransform.h"
#include "CRectCol.h"
#include "CPlaneColl.h"
#include "CCamera.h"
#include "CRecTex.h"
#include "CTerrainTex.h"
#include "CTexture.h"
#include "CCubeCol.h"
#include "CSkyCube.h"
#include "CUIRecTex.h"
#include "CAAABBB.h"
#include "COBB.h"
#include "CSphere.h"
#include "CAABB.h"
#include "CGridCol.h"
#include "CTileTex.h"
#include "CCenterLineCol.h"
#include "CTerrainCol.h"
#include "CParticleRecTex.h"
#include "CParticleRecTex_Alpha.h"
#include "CParticleOnFloorRecTex.h"
#include "CBossAppearMeshTex.h"
#include "CAnimation.h"
#include "CMeshAnimation.h"
#include "CBillboard.h"
#include "CMonster.h"
#include "CParticle.h"
#include "CStatus.h"
#include "CUITileTex.h"
#include "CCubeTex.h"
#include "CWeaponController.h"
#include "CDoor_Component.h"
#include "CHoverItem.h"
#include "CDropItem.h"
#include "CMapObjectTex.h"
#include "CWallet.h"
#include "CBarrier.h"
#include "CBossEffectAnimation.h"
#pragma endregion


BEGIN(Engine)

class ENGINE_DLL CProtoTypeManager :
    public CBase
{
    DECLARE_SINGLETON(CProtoTypeManager)

private:
    explicit CProtoTypeManager();
    virtual  ~CProtoTypeManager();

 public:
    HRESULT         Ready_Prototype(PROTOTYPE _enumkey, CComponent* pComponent);
    CComponent*     Clone_Prototype(PROTOTYPE _enumkey);

private:
    CComponent*     Find_Prototype(PROTOTYPE _enumkey);

public:
    void			DeleteALL();

private:
    map<PROTOTYPE, CComponent*>     m_mapPrototype; // 원본 컴포넌트

private:
    virtual void    Free();
};

END