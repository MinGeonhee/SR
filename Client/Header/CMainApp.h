#pragma once

#include "CBase.h"
#include "CGraphicDev.h"

namespace Engine
{
    class CManagement;
}

class CMainApp :
    public CBase
{
private:
    explicit CMainApp(); // 무조건 명시적 형변환을 시키기 위해서 explicit을 붙임.
    virtual ~CMainApp();

public:
    HRESULT     Ready_MainApp(); // 우리가 평소에 쓰던 Initialize 같은 것.
    int         Update_MainApp(const float& fTimeDelta); // 우리가 쓰던 업데이트
    void        LateUpdate_MainApp(const float& fTimeDelta); // 
    void        Render_MainApp();
    void        ShowFPS(const float& fTimeDelta);
    // Update와 LateUpdate 문에 인자로 DeltaTime이 들어오게 된 이유가
    // 전에는 매 프레임 기준으로 움직임을 정했다면 이제는 일관된 시간값을 가지고 움직이게 하기 위함이다.

    
private:
    HRESULT     Ready_ProtoType();
    HRESULT     Ready_CollisionTable();
    HRESULT     Ready_DefaultSetting(LPDIRECT3DDEVICE9* mGraphicDev);
    HRESULT     Ready_SceneSetting();   // 여기서 SceneManager에서 사용할 Scene 함수들 등록해둠.
    HRESULT     Ready_TextureSetting();   // 여기서 SceneManager에서 사용할 Scene 함수들 등록해둠.

private:

    Engine::CGraphicDev* m_pDeviceClass;
    Engine::CManagement* m_pManagementClass;

    LPDIRECT3DDEVICE9   m_pGraphicDev;

private:
    float               timer = 0.f;
    int                 fps = 0;

public:
    static CMainApp* Create();      // 객체 생성 함수.

private:
    virtual void        Free(); // 객체 소멸을 담당하는 함수

};

