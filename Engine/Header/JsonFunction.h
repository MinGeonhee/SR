#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL JsonFunction
{
public:
	static void Save_ObjectHandle(nlohmann::ordered_json& j , const ObjectHandle& tHandle );
	static void Save_Info(nlohmann::ordered_json& j, const _vec3& vPos, const _vec3& vRot, const _vec3& vScale);
	static void Save_String(nlohmann::ordered_json& j, const string& Key, const string& value);
	static void Save_SpriteRect(nlohmann::ordered_json& j , const SpriteRect& tSpriteRect);
	static void Save_EventPath(nlohmann::ordered_json& j, const EVENT_PATH& path);
	static void	Save_CenterUV(nlohmann::ordered_json& j, const _vec2& vCenterUV);
	static void Save_TileInfo_ForMapTool(nlohmann::ordered_json& j, const MAPTOOLTILE_INFO& tInfo);
	static void	Save_Tile_Type(nlohmann::ordered_json& j, const TILETYPE& eType);
	static void	Save_Cube_CenterUV(nlohmann::ordered_json& j, const _vec2* vCenterUV);
	static void Save_Cube_Type(nlohmann::ordered_json& j, const CUBE_TYPE& eType);
	static void	Save_Cube_VerticalFilp(nlohmann::ordered_json& j,bool* isVerticalFilp);
	static void	Save_Cube_HorizontalFilp(nlohmann::ordered_json& j,bool* isHorizontalFilp);

	static const ObjectHandle& Load_ObjectHandle(const nlohmann::json& j);
	static const SpriteRect& Load_SpriteRect(const nlohmann::json& j);
	static const _vec3& Load_Info(const nlohmann::json& j, const INFO_TYPE& eType);
	static EVENT_PATH Load_EventPath(const  nlohmann::json& j);
	static const _vec2& Load_CenterUV(const nlohmann::json& j);
	static const MAPTOOLTILE_INFO& Load_MapToolTile_Info(const nlohmann::json& j);
	static const TILETYPE& Load_TileType(const nlohmann::json& j);
	static const void    Load_Cube_CenterUV(const nlohmann::json& j, _vec2* vCenterUV);
	static const CUBE_TYPE&  Load_Cube_Type(const nlohmann::json& j);

	static const void Load_Cube_VerticalFilp(const nlohmann::json& j, bool* isVerticalFilp);
	static const void Load_Cube_HorizontalFilp(const nlohmann::json& j, bool* isHorizontalFilp);


	static void	SettingCurHandleIndexNum(int* index, const json& j);
	//static const _vec3& Setting_SpriteRect(nlohmann::ordered_json& j, SpriteRect* pSpriteRect);
	
	static const bool& HasLastSptrite(const json& j, const std::string& BaseName);


	static void SetPlayerStartAndRot(SCENETYPE eType  , _vec3* vPos, _vec3* vRot);
};


END
