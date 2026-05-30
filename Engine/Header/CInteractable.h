#pragma once
#include "CEventObject.h"
#include "CParticleSystem_Sparkle.h"

BEGIN(Engine)


class CBillboard;
class CTexture;
class CStatus;

class ENGINE_DLL CInteractable :
    public CEventObject
{
private:
    explicit CInteractable(LPDIRECT3DDEVICE9);
    explicit CInteractable(const CInteractable& cpy);
    virtual ~CInteractable();


private:
    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();

public:
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void        Change_UVBuffer(const UVRect& tUVRect);

    void        Set_HaveLastSprite(const _bool is) { m_IsHaveEndSprite = is;}
    void        Set_IsLastSprite(const _bool is) { m_IsLastSprite = is; }
    
    void        Set_UVRectFirst(const UVRect& tUVRect) { m_tUVRect[FIRST_SPRITE] = tUVRect; }
    void        Set_UVRectLast(const UVRect& tUVRect) { m_tUVRect[LAST_SPRITE] = tUVRect; }
    void        Set_InteractType(const INTERACT_TYPE& eType){ m_eInteractType = eType;}
    void        Set_HitToBreak(const int& amount) { m_iHitToBreak = amount; }
    void        Set_HealAmount(const int& amount) { m_iHealAmount = amount; }
    void        Set_DropItemCount(const _int& iAmount) { m_iDropItemCount = iAmount; }
         
    void        Minus_HitToBreak();

public:

    const _bool& Get_IsLastSprite() { return m_IsLastSprite; }
    const _bool& Get_IsHaveLastSprite() { return m_IsHaveEndSprite; }

    const UVRect& GetFirstUVRect() { return m_tUVRect[FIRST_SPRITE]; }
    const UVRect& GetLastUVRect() { return m_tUVRect[LAST_SPRITE]; }
    const INTERACT_TYPE& GetInteractType() { return m_eInteractType; }

    const int& Get_HitToBreak() { return m_iHitToBreak; }
    const int& Get_HeadlAmount() { return m_iHealAmount; }

    const int& Get_DropItemAmount() { return m_iDropItemCount; }

public:

    //힐을 할 수 있냐 없냐
    const _bool&        Get_IsCanInteraction() { return m_isCanInteraction; }
    //깨질 수 있냐 없냐
    const _bool&        Get_IsCanBreak() { return m_isCanBreak; }



    void                Set_IsCanInteraction(const _bool isCan) { m_isCanInteraction = isCan; }
    void                Set_IsCanBreak(const _bool isCan) { m_isCanBreak = isCan; }

    void                Set_OrbCount(const int count) { m_iOrbCount = count; }
    void                Minus_OrbCount() { m_iOrbCount--; }
    _int                Get_OrbCount() { return m_iOrbCount; }
public:


    static CInteractable* Create(LPDIRECT3DDEVICE9 Device);
    void                  Setting_EventObject(const ObjectHandle& tHandle, const _vec3& vPos,const _vec3& vRot, const _vec3 vScale,
                          const MAPOBJECT_CATEGORY& eCat,const MAPOBJECT_PLACEMENT& ePlacement, const EVENT_PATH& EventPath);
    
    void                  Setting_Sprite();
    void                  Setting_Component();
    void                  Setting_Detailed();

    // 컴포넌트 관련
    void        Setting_Billborad();

    // 스탯관련
    void        Setting_HitToBreak();
    void        Setting_HeadAmount();
    void        Setting_DropItemCount();

public:
    void        Change_SpriteToLast_ByCollision();
    void        Change_SpriteToLast_ByIneraction();

protected:

    //빌보드 적용
    CBillboard*     m_pBillboardCom;
    CTexture*       m_pTexture;
    CStatus*        m_pStatus;

    INTERACT_TYPE   m_eInteractType;


    UVRect          m_tUVRect[END_SPRITE];

    bool            m_IsHaveEndSprite;
    bool            m_IsLastSprite;
   
    int               m_iDropItemCount;
    
    int               m_iHitToBreak;
    bool              m_isCanBreak;  //부셔지냐 안부셔지냐

    int               m_iHealAmount;

    bool              m_isCanInteraction;

    int               m_iOrbCount;

private:

    virtual    void     Free();

private:
    CParticleSystem_Sparkle* m_pSparkleParticleSystem = nullptr;
    void    Play_Particle_Sparkle();
public:
    void    Remove_Particle_Sparkle();
};

END