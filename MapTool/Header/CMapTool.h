#pragma once
#include "CBase.h"
#include "CImGui.h"
#include "CGraphicDev.h"
#include "CProtoTypeManager.h"


class CCameraMan;

namespace Engine
{
    class CManagement;
}
class CMapTool : public CBase
{
private:

	explicit CMapTool();
	virtual ~CMapTool();

public:


    HRESULT                                 Ready_Maptool(); 
    int                                     Update_Maptool(const float& fTimeDelta); 
    void                                    LateUpdate_Maptool(const float& fTimeDelta);
    void                                    Render_Maptool();

private:

    HRESULT           Ready_DefaultSetting(LPDIRECT3DDEVICE9* mGraphicDev);
    HRESULT           Ready_ImGuiDemo(LPDIRECT3DDEVICE9 device);
    HRESULT           Ready_Scene(LPDIRECT3DDEVICE9 device);


    void              CheckFrame(const _float& fTimeDelta);

private:

    Engine::CGraphicDev* m_pDeviceClass;
    Engine::CManagement* m_pManagementClass;    
    LPDIRECT3DDEVICE9   m_pGraphicDev;
    
    CImGui* m_pImGui;

    _float m_fTimer;
    _int  m_FPS;

public:
    static CMapTool*    Create();      // 객체 생성 함수.


private:
    virtual void        Free(); // 객체 소멸을 담당하는 함수
};

