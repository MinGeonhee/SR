#pragma once
#include "ImGuiFileDialog.h"
#include "CBase.h"
#include "Engine_Define.h"
#include "CProtoTypeManager.h"


struct TexTileImageUV
{
	ImTextureID texID;
	_vec2	  vCenterUV;
};

namespace Engine
{
	class CMapToolCube;
	class CMapToolTile;
	class CMapToolMapObject;
	class CTile;
}


class CImGui : public CBase
{
DECLARE_SINGLETON(CImGui)

private:

	CImGui();
	~CImGui();

public:
	void	Init(LPDIRECT3DDEVICE9 device);
	_int	Update_ImGui(const float& fTimeDelta);
	void	LateUpdate_ImGui();
	void	Render_ImGui();

private:

	//메인 윈도우
	void				MainWindow();
	void				EventIndexWindow();
	void				EventExplainWindow();

	// 큐브 타일 모드 전용
	void				MapToolWindow();

	void				TextureWindow();
	void				FixInfoWindow(CGameObject* pObj);


	// 맵 오브젝트 모드 전용
	void				ObjectToolWindow();


	void				RenderAllSprite();
	void				CurrentSprite_Render();
	void				CurrentSprite_ImageRender(const SpriteRect& vSptriteRect );
	void				CurrentCategori_Render();
	void				CurrentPlacement_Render();
	

	//
private:
	//기능관련
	void				RenderLine();
	void				Default_Setting_Render(const char* Text);

	void				File_Menu_Render();
	void				Active_Mode();
	void				Render_Mode();

	void				Settting_Floor_Render();

	//타일 큐브 관련
	void				ObjectInfo_Render();
	void				TileInfo_Render(CMapToolTile* pTile);
	void				CubeInfo_Render(CMapToolCube* pCube);
	void				MapObjectInfo_Render(CMapToolMapObject* pMapObject);

	void				DefaultInfo_Render(CGameObject* pObj);

	void				Setting_BuildMode_Render();

	void				Setting_ShapeType_Render();

	void				Setting_TileType_Render();

	void				Setting_RoomNum_Render();

	void				MousePos_Render();

	void				Setting_Texture_Render();
	void				TileTex_Render();


	void				Render_Type(const char* Title , const char* Value);
public:


	//Info 관련 출력기능
	void				Position_Render(const _vec3& vPos);
	void				Rotation_Render(const _vec3& vRot);
	

	void				Int_Render(string str, const _int& Num, _vec4 vColor = { 1.f,0.f,0.f,1.f });


	//타입별 str 뱉어주는 기능
	const char*			Get_FloorStr(const FLOOR& eFloor);
	const char*			Get_TileTypeStr(const TILETYPE& eType);
	const char*			Get_ShapeTypeStr(const MAPTOOL_SHAPETYPE& eType);
	const char*			Get_CubeTypeStr(const CUBE_TYPE& eType);
	const char*			Get_CategoriStr(const MAPOBJECT_CATEGORY& eType);
	const char*			Get_PlacementStr(const MAPOBJECT_PLACEMENT& eType);


	void				Render_Atlas(CTexture* pTexture , const string& pPathJson);
	void				Render_Atlas_Item(CTexture* pTexture , const string& pPathJson , string strType);

public:

	void				Save_MapData(const string& sFileName);
	void				Load_MapData();
	
	vector<string>		Lead_Folder(const string& FloderPath);
public:

	void				Add_ImGuiImageUV(TexTileImageUV* pTexImageUV);

private:

	TOOLTYPE				m_eMapToolType;
	FLOOR					m_eCurFloor;
	vector<TexTileImageUV*>	m_vImageTileUV;

	CGameObject*			m_pFixTarget;
	CGameObject*			m_pCameraMan;

	_bool					m_IsTileTypeRender;
	_bool					m_IsShapeTypeRender;


	_int					m_iSettingRoomNum;
	_int					m_iFixRoomNum;
	_int					m_iFix_EventIndexNum;
	_int					m_iFix_SwitchDoorCount;
	_int					m_iFix_EventBufferNum;

	LPDIRECT3DDEVICE9		m_pGraphicDevice;
	ImVec4					clear_color;
	
	_bool					m_ShowTextureWindow;
	_bool					m_ShowFixInfoWindow;


	string					m_sFileName;

	virtual	void			Free();
};

