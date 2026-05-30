#pragma once

#include "CBase.h"
#include "Engine_Define.h"




namespace Engine
{
	class CCubeManager;
	class CTileManager;
	class CGameObject;
	class CMapObjectManager;
}

class CGridManager;

class CToolManager : public CBase
{
	DECLARE_SINGLETON(CToolManager)
private:

	explicit CToolManager();
	virtual ~CToolManager();

public:

	void	ToolManger_Update(const _float& DT);
	void	ToolManager_LateUpdate(const _float& DT);
	void	ToolManger_Render(LPDIRECT3DDEVICE9 device);

public:

	const FLOOR&	GetCurFloor() { return m_eCurFloor; }
	void			UpFloor();
	void			DownFloor();

	const _ushort&			GetSettingRoomNum() { return m_sSettingRoomNum; }


public:

	void	SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive);

public:

	void	MousePickingNoneClick();
	void	ObjectNonePicking();
	void	MapNonePicking();

	void	MousePickingClick();
	void	ObjectPicking();
	void	MapPicking();


	void	KeyUpdate();


	const MapToolActive& GetActiveList() { return m_tActivelist; }



public:
	
	void	SetTextureMode_CenterUV(const _vec2& vCenterUV);
	const _vec2& GetTextureMode_CenterUV() { return m_vCurCenterUV; }

public:

	void	Set_BuildType(const MAPTOOL_BUILDTYPE& eMode) { m_eBuildType = eMode; }
	void	Set_ShapeType(const MAPTOOL_SHAPETYPE& eMode) { m_eShapeType = eMode; }
	
	//타일 상태값 바꾸기
	void	Set_TileType(const TILETYPE& eType) { m_eTileType = eType; }

	void	Set_SettingRoomNum(const _ushort& sRoomNum);
	void	Set_Scale(const _vec3& vScale);

	void	Set_ToolMode(const TOOLTYPE& eToolType) { m_eToolType = eToolType; }

	void	Set_DefaultTileUV(const _vec2 UV);
	void	Set_DefaultCubeUV(const _vec2 UV);

public:

	const MAPTOOL_BUILDTYPE& Get_BuildType()		{ return m_eBuildType; }
	const MAPTOOL_SHAPETYPE& Get_ShapeType()		{ return m_eShapeType; }
	const TILETYPE& Get_TileType()					{ return  m_eTileType ; }
	const _vec3& Get_Scale()						{ return m_vScale; }
	const TOOLTYPE& Get_ToolType()					{ return m_eToolType; }

public:

	void				SetAllRender();
	void			    SetFloorRender();

public:
	void	Add_MapObject(const OBJTYPE& eType, CGameObject* pObj);
public:
	
	void	Init();

private:

	TOOLTYPE				m_eToolType;

	FLOOR					m_eCurFloor;

	//방		번호
	_ushort					m_sSettingRoomNum;
	//텍스처냐 설치냐
	MAPTOOL_BUILDTYPE		m_eBuildType;
	//타일이냐 큐브냐
	MAPTOOL_SHAPETYPE		m_eShapeType;

	_vec2					m_vCurCenterUV;

	_vec2					m_vDefaultCubeCenterUV;
	_vec2					m_vDefaultTileCenterUV;


	_vec3					m_vScale;

	//타일이 Floor인지 Celling인지 들고있을 변수
	TILETYPE				m_eTileType;


	CGridManager* m_pGridManager;
	CTileManager* m_pTileManager;
	CCubeManager* m_pCubeManager;
	CMapObjectManager* m_pMapObjectManager;

	MapToolActive		m_tActivelist;
			

private:
	virtual			void	Free();
};

