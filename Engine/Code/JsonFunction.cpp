#include "JsonFunction.h"
#include "CEnumStringFunction.h"
#include "CPathFunction.h"

void JsonFunction::Save_ObjectHandle(nlohmann::ordered_json& j, const ObjectHandle& tHandle)
{
	j["HANDLE"] = {
		{"OBJORIGINTYPE", CEnumStringFunction::OBJORIGINTYPE_EnumToString(tHandle.m_eOriginType)} ,
		{"OBJTYPE", CEnumStringFunction::OBJTYPE_EnumToString(tHandle.m_eType)},
		{"FLOOR", CEnumStringFunction::FLOOR_EnumToString(tHandle.m_eFloor)},
		{"INDEX", tHandle.m_iIndex},
		{"EVENTINDEX", tHandle.m_sEventIndex },
		{"ROOMNUM", tHandle.m_sRoomNum},
		{"GENERATION", tHandle.m_iGeneration}
	};
}

void JsonFunction::Save_Info(nlohmann::ordered_json& j, const _vec3& vPos, const _vec3& vRot, const _vec3& vScale)
{
	j["POS"] = {
		{ "x" , vPos.x}, { "y", vPos.y } , { "z", vPos.z}
	};

	j["ROT"] = {
	{ "x" , vRot.x}, { "y", vRot.y } , { "z", vRot.z}
	};

	j["SCALE"] = {
	{ "x" , vScale.x}, { "y", vScale.y } , { "z", vScale.z}
	};
}

void JsonFunction::Save_String(nlohmann::ordered_json& j, const string& Key, const string& value)
{
	j[Key] = value;
	return;
}

void JsonFunction::Save_SpriteRect(nlohmann::ordered_json& j, const SpriteRect& tSpriteRect)
{
	j["SPRITE"] = {
	{ "x", tSpriteRect.x },
	{ "y", tSpriteRect.y },
	{ "w", tSpriteRect.w },
	{ "h", tSpriteRect.h },
	{ "Mw",tSpriteRect.Mw },
	{ "Mh",tSpriteRect.Mh}
	};
}

void JsonFunction::Save_EventPath(nlohmann::ordered_json& j, const EVENT_PATH& path)
{
	auto str = CEnumStringFunction::EVENT_PATH_EnumToString(path);
	j["EVENTPATH"] = str;
}

void JsonFunction::Save_CenterUV(nlohmann::ordered_json& j, const _vec2& vCenterUV)
{
	j["CENTERUV"] = { { "x" , vCenterUV.x } , { "y" , vCenterUV.y } };
	return;
}

void JsonFunction::Save_TileInfo_ForMapTool(nlohmann::ordered_json& j, const MAPTOOLTILE_INFO& tInfo)
{
	j["MAPTOOL_TILE_INFO"] = {
		{ "MAKEINDEX" , tInfo.iMakeIndex }, { "TARGETMAKEINDEX" , tInfo.iTargetMakeIndex} ,
		{ "ISONCUBE" , tInfo.IsCubeOnTop } , { "ISONCEILING" , tInfo.IsHaveCeiling }
	};

	return;
}

void JsonFunction::Save_Tile_Type(nlohmann::ordered_json& j, const TILETYPE& eType)
{
	j["TIELTYPE"] = CEnumStringFunction::TILETYPE_Enum_To_String(eType);

	return;
}

void JsonFunction::Save_Cube_CenterUV(nlohmann::ordered_json& j, const _vec2* vCenterUV)
{
	j["NAGATIVEZ"] = { vCenterUV[CUBE_FACE_NEGATIVE_Z].x , vCenterUV[CUBE_FACE_NEGATIVE_Z].y };
	j["POSITIVEZ"] = { vCenterUV[CUBE_FACE_POSITIVE_Z].x , vCenterUV[CUBE_FACE_POSITIVE_Z].y };
	
	j["NAGATIVEX"] = { vCenterUV[CUBE_FACE_NEGATIVE_X].x , vCenterUV[CUBE_FACE_NEGATIVE_X].y };
	j["POSITIVEX"] = { vCenterUV[CUBE_FACE_POSITIVE_X].x , vCenterUV[CUBE_FACE_POSITIVE_X].y };
	
	j["POSITIVEY"] = { vCenterUV[CUBE_FACE_POSITIVE_Y].x , vCenterUV[CUBE_FACE_POSITIVE_Y].y };

	//j["NAGATIVEY"] = { vCenterUV[CUBE_FACE_NEGATIVE_Y].x , vCenterUV[CUBE_FACE_NEGATIVE_Y].y };

}

void JsonFunction::Save_Cube_Type(nlohmann::ordered_json& j, const CUBE_TYPE& eType)
{
	j["CUBETYPE"] = CEnumStringFunction::CUBE_TYPE_Enum_To_String(eType);
	return;
}


const ObjectHandle& JsonFunction::Load_ObjectHandle(const nlohmann::json& j)
{
	ObjectHandle vHandle;

	vHandle.m_eOriginType = CEnumStringFunction::OBJORIGINTYPE_StringToEnum(j["HANDLE"]["OBJORIGINTYPE"].get<string>());
	vHandle.m_eType = CEnumStringFunction::OBJTYPE_StringToEnum(j["HANDLE"]["OBJTYPE"].get<string>());
	vHandle.m_eFloor = CEnumStringFunction::FLOOR_StringToEnum(j["HANDLE"]["FLOOR"].get<string>());

	vHandle.m_iIndex = j["HANDLE"]["INDEX"].get<_ushort>();
	vHandle.m_sEventIndex = j["HANDLE"]["EVENTINDEX"].get<_ushort>();
	vHandle.m_sRoomNum = j["HANDLE"]["GENERATION"].get<int>();
	vHandle.m_sRoomNum = j["HANDLE"]["ROOMNUM"].get<int>();

	return vHandle;
}

const SpriteRect& JsonFunction::Load_SpriteRect(const nlohmann::json& j)
{
	SpriteRect vSprite = { j["SPRITE"]["x"].get<float>(),
					   j["SPRITE"]["y"].get<float>(),
					   j["SPRITE"]["w"].get<float>(),
					   j["SPRITE"]["h"].get<float>(),
					   j["SPRITE"]["Mw"].get<float>(),
					   j["SPRITE"]["Mh"].get<float>() };

	return vSprite;
}

const _vec3& JsonFunction::Load_Info(const nlohmann::json& j, const INFO_TYPE& eType)
{
	_vec3 vInfo = {0.f,0.f,0.f};

	switch (eType)
	{
	case Engine::INFO_TYPE_POS:

		vInfo = { j["POS"]["x"].get<float>() , j["POS"]["y"].get<float>() ,j["POS"]["z"].get<float>() };

		break;
	case Engine::INFO_TYPE_ROT:

		vInfo = { j["ROT"]["x"].get<float>() , j["ROT"]["y"].get<float>() ,j["ROT"]["z"].get<float>() };

		break;
	case Engine::INFO_TYPE_SCALE:

		vInfo = { j["SCALE"]["x"].get<float>() , j["SCALE"]["y"].get<float>() ,j["SCALE"]["z"].get<float>() };
		
		break;
	default:
		return { 0.f,0.f,0.f };
	}

	return vInfo;
}

EVENT_PATH JsonFunction::Load_EventPath(const const nlohmann::json& j)
{
	if (!j.contains("EVENTPATH"))
		return EVENT_PATH::UNKNOWN;

	return CEnumStringFunction::EVENT_PATH_StringToEnum( j["EVENTPATH"].get<string>());
}

const _vec2& JsonFunction::Load_CenterUV(const nlohmann::json& j)
{
	if (!j.contains("CENTERUV"))
		return _vec2{ 0.f,0.f };
	if (!j["CENTERUV"].contains("x"))
		return _vec2{ 0.f,0.f };
	if (!j["CENTERUV"].contains("y"))
		return _vec2{ 0.f,0.f };

	return _vec2{ j["CENTERUV"]["x"].get<float>() ,  j["CENTERUV"]["y"].get<float>() };
}

const MAPTOOLTILE_INFO& JsonFunction::Load_MapToolTile_Info(const nlohmann::json& j)
{
	MAPTOOLTILE_INFO tInfo = { 0 , 0  , false , false };

	if (!j.contains("MAPTOOL_TILE_INFO"))
		return tInfo;
	if (!j["MAPTOOL_TILE_INFO"].contains("MAKEINDEX"))
		return tInfo;
	if (!j["MAPTOOL_TILE_INFO"].contains("TARGETMAKEINDEX"))
		return tInfo;
	if (!j["MAPTOOL_TILE_INFO"].contains("ISONCUBE"))
		return tInfo;
	if (!j["MAPTOOL_TILE_INFO"].contains("ISONCEILING"))
		return tInfo;

	tInfo.iMakeIndex = j["MAPTOOL_TILE_INFO"]["MAKEINDEX"].get<int>();
	tInfo.iTargetMakeIndex = j["MAPTOOL_TILE_INFO"]["TARGETMAKEINDEX"].get<int>();
	tInfo.IsCubeOnTop = j["MAPTOOL_TILE_INFO"]["ISONCUBE"].get<bool>();
	tInfo.IsHaveCeiling = j["MAPTOOL_TILE_INFO"]["ISONCEILING"].get<bool>();

	return tInfo;
}

const void JsonFunction::Load_Cube_CenterUV(const nlohmann::json& j, _vec2* vCenterUV)
{
	if (!j.contains("NAGATIVEZ"))
		return ;
	if (!j.contains("POSITIVEZ"))
		return ;
	if (!j.contains("NAGATIVEX"))
		return ;
	if (!j.contains("POSITIVEX"))
		return ;
	//if (!j.contains("NAGATIVEY"))
	//	return ;
	if (!j.contains("POSITIVEY"))
		return ;

	vCenterUV[CUBE_FACE::CUBE_FACE_NEGATIVE_Z] = { j.at("NAGATIVEZ")[0].get<float>(), j.at("NAGATIVEZ")[1].get<float>() };
	vCenterUV[CUBE_FACE::CUBE_FACE_POSITIVE_Z] = { j.at("POSITIVEZ")[0].get<float>(), j.at("POSITIVEZ")[1].get<float>() };

	vCenterUV[CUBE_FACE::CUBE_FACE_NEGATIVE_X] = { j.at("NAGATIVEX")[0].get<float>(), j.at("NAGATIVEX")[1].get<float>() };
	vCenterUV[CUBE_FACE::CUBE_FACE_POSITIVE_X] = { j.at("POSITIVEX")[0].get<float>(), j.at("POSITIVEX")[1].get<float>() };

	vCenterUV[CUBE_FACE::CUBE_FACE_POSITIVE_Y] = { j.at("POSITIVEY")[0].get<float>(), j.at("POSITIVEY")[1].get<float>() };

	//vCenterUV[CUBE_FACE::CUBE_FACE_NEGATIVE_Y] = { j.at("NAGATIVEY")[0].get<float>(), j.at("NAGATIVEY")[1].get<float>() };

	return;
}

const CUBE_TYPE& JsonFunction::Load_Cube_Type(const nlohmann::json& j)
{
	if (!j.contains("CUBETYPE"))
		return CUBE_TYPE::CUBE_UNKNOWN;

	return CEnumStringFunction::CUBE_TYPE_String_To_Enum(j["CUBETYPE"].get<string>());
}

const void JsonFunction::Load_Cube_VerticalFilp(const nlohmann::json& j, bool* isVerticalFilp)
{
	if (j.contains("VERTICAL_FLIP")) {
		const auto& flipData = j["VERTICAL_FLIP"];

		isVerticalFilp[CUBE_FACE_NEGATIVE_Z] = flipData.value("-Z", false);
		isVerticalFilp[CUBE_FACE_POSITIVE_Z] = flipData.value("+Z", false);
		isVerticalFilp[CUBE_FACE_NEGATIVE_X] = flipData.value("-X", false);
		isVerticalFilp[CUBE_FACE_POSITIVE_X] = flipData.value("+X", false);
		isVerticalFilp[CUBE_FACE_POSITIVE_Y] = flipData.value("+Y", false);
	}
}

const void JsonFunction::Load_Cube_HorizontalFilp(const nlohmann::json& j, bool* isHorizontalFilp)
{
	if (j.contains("HORIZONTAL_FLIP")) {
		const auto& flipData = j["HORIZONTAL_FLIP"];

		isHorizontalFilp[CUBE_FACE_NEGATIVE_Z] = flipData.value("-Z", false);
		isHorizontalFilp[CUBE_FACE_POSITIVE_Z] = flipData.value("+Z", false);
		isHorizontalFilp[CUBE_FACE_NEGATIVE_X] = flipData.value("-X", false);
		isHorizontalFilp[CUBE_FACE_POSITIVE_X] = flipData.value("+X", false);
		isHorizontalFilp[CUBE_FACE_POSITIVE_Y] = flipData.value("+Y", false);
	}
}


void JsonFunction::Save_Cube_VerticalFilp(nlohmann::ordered_json& j,bool* isVerticalFilp)
{
	j["VERTICAL_FLIP"] = {
		{ "-Z" ,  isVerticalFilp[CUBE_FACE_NEGATIVE_Z]},
		{ "+Z" ,  isVerticalFilp[CUBE_FACE_POSITIVE_Z]},

		{ "-X" ,  isVerticalFilp[CUBE_FACE_NEGATIVE_X]},
		{ "+X" ,  isVerticalFilp[CUBE_FACE_POSITIVE_X]},

		{ "+Y" ,  isVerticalFilp[CUBE_FACE_POSITIVE_Y]},
	};
}

void JsonFunction::Save_Cube_HorizontalFilp(nlohmann::ordered_json& j, bool* isHorizontalFilp)
{
	j["HORIZONTAL_FLIP"] = {
		{ "-Z" ,  isHorizontalFilp[CUBE_FACE_NEGATIVE_Z]},
		{ "+Z" ,  isHorizontalFilp[CUBE_FACE_POSITIVE_Z]},

		{ "-X" ,  isHorizontalFilp[CUBE_FACE_NEGATIVE_X]},
		{ "+X" ,  isHorizontalFilp[CUBE_FACE_POSITIVE_X]},

		{ "+Y" ,  isHorizontalFilp[CUBE_FACE_POSITIVE_Y]},
	};
}

const TILETYPE& JsonFunction::Load_TileType(const nlohmann::json& j)
{
	if (!j.contains("TIELTYPE"))
		return TILETYPE::TILE_END;
	
	return CEnumStringFunction::TILETYPE_String_To_Enum(j["TIELTYPE"].get<string>());
}

void JsonFunction::SettingCurHandleIndexNum(int* index, const json& j)
{
	if (!j.contains("HANDLE"))
		return;
	if (!j["HANDLE"].contains("INDEX"))
		return;

	if ( *index  > j["HANDLE"]["INDEX"].get<int>())
		return;

	*index = j["HANDLE"]["INDEX"].get<int>();
} 


const bool& JsonFunction::HasLastSptrite(const json& j, const std::string& BaseName)
{
	std::string lastName = BaseName + "LAST";

	if (!j.contains("frames"))
		return false;


	for (auto& frame : j["frames"])
	{
		if (!frame.contains("filename"))
			return false;

		if (frame["filename"].get<string>() == lastName)
			return true;
	}

	return false;
}

void JsonFunction::SetPlayerStartAndRot(SCENETYPE eType,_vec3* vPos, _vec3* vRot)
{
	wstring str = CPathFunction::PlayerStart_Pos_Rot_Json;


	ifstream is(str);

	if (is.is_open() == false)
		return;

	nlohmann::json j;

	is >> j;

	string SceneStr = CEnumStringFunction::SCENETYPE_Enum_To_String(eType);

	if (SceneStr == "UNKNOWN")
		return;

	_vec3 POS; _vec3 ROT;

	if (j.contains(SceneStr) == false)
		return;

	vPos->x = j[SceneStr]["POS"]["x"].get<float>();
	vPos->y = j[SceneStr]["POS"]["y"].get<float>();
	vPos->z = j[SceneStr]["POS"]["z"].get<float>();

	vRot->x = j[SceneStr]["ROT"]["x"].get<float>();
	vRot->y = j[SceneStr]["ROT"]["y"].get<float>();
	vRot->z = j[SceneStr]["ROT"]["z"].get<float>();

}
