#pragma once
#include "CBase.h"
#include "CImGuiDemo.h"
#include "CGraphicDev.h"
#include "CGrid.h"

namespace Engine
{
    class CManagement;
}

class CEffectTool : public CBase
{
private:

	explicit CEffectTool();
	virtual ~CEffectTool();

public:
    HRESULT     Ready_EffectTool(); // 우리가 평소에 쓰던 Initialize 같은 것.
    int         Update_EffectTool(const float& fTimeDelta); // 우리가 쓰던 업데이트
    void        LateUpdate_EffectTool(const float& fTimeDelta); // 
    void        Render_EffectTool();
    // Update와 LateUpdate 문에 인자로 DeltaTime이 들어오게 된 이유가
    // 전에는 매 프레임 기준으로 움직임을 정했다면 이제는 일관된 시간값을 가지고 움직이게 하기 위함이다.

private:

    HRESULT     Ready_DefaultSetting(LPDIRECT3DDEVICE9* mGraphicDev);
    HRESULT     Ready_Scene(LPDIRECT3DDEVICE9 mGraphicDev);
    HRESULT     Ready_ImGuiDemo(LPDIRECT3DDEVICE9 device);

private:
    Engine::CGraphicDev* m_pDeviceClass;
    Engine::CManagement* m_pManagementClass;

    LPDIRECT3DDEVICE9   m_pGraphicDev;

    CImGuiDemo* m_pImGui;
    
    int fps = 0;
    float timer = 0.f;

public:
    static CEffectTool*    Create();      // 객체 생성 함수.
    void ShowFPS(const float& fTimeDelta);

private:
    virtual void        Free(); // 객체 소멸을 담당하는 함수
};

