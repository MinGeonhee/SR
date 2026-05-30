#pragma once
#include "CBase.h"
#include "imgui.h"
#include "Engine_Define.h"

class CImGuiDemo : public CBase
{
public:

	CImGuiDemo();
	~CImGuiDemo();

public:
	void	Init(LPDIRECT3DDEVICE9 device);
	_int	Update_ImGui(const float& fTimeDelta);
	void	LateUpdate_ImGui();
	void	Render_ImGui();

private:
	void	Test();

private:
	LPDIRECT3DDEVICE9		m_pGraphicDevice;
	bool					show_demo_window;
	bool					show_another_window ;
	ImVec4					clear_color;


	virtual	void			Free();
};

