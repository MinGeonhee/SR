#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CSceneManager;
class ENGINE_DLL CUISystem :
    public CBase
{
    DECLARE_SINGLETON(CUISystem)

public:
    explicit CUISystem();
    virtual ~CUISystem();

public:
    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

private:
    CSceneManager* m_pScenemgr = nullptr;

    void    Free();
    void    Scene_Change_Button();
	void    UI_Open_Close();
    void    Interaction_UI();
};
 
END