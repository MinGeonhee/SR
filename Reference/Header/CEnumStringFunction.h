#pragma once


#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEnumStringFunction
{
public:
	static  std::string OBJTYPE_EnumToString(OBJTYPE eType);
	static OBJTYPE OBJTYPE_StringToEnum(const std::string& str);

	static  std::string OBJORIGINTYPE_EnumToString(const OBJORIGINTYPE& eType);
	static  OBJORIGINTYPE OBJORIGINTYPE_StringToEnum(const std::string& str);

	static  std::string FLOOR_EnumToString(const FLOOR& eType);
	static  FLOOR FLOOR_StringToEnum(const std::string& str);

	static std::string TILETYPE_Enum_To_String(const TILETYPE& eType);
	static TILETYPE		TILETYPE_String_To_Enum(const std::string& str);


	static std::string CUBE_TYPE_Enum_To_String(const CUBE_TYPE& eType);
	static CUBE_TYPE	CUBE_TYPE_String_To_Enum(const std::string& str);


	static  std::string MAPOBJECT_CATEGORI_EnumToString(MAPOBJECT_CATEGORY eType);
	static  MAPOBJECT_CATEGORY MAPOBJECT_CATEGORI_StringToEnum(const std::string& str);


	static  std::string MAPOBJECT_PLACEMENT_EnumToString(const MAPOBJECT_PLACEMENT& eType);
	static  MAPOBJECT_PLACEMENT MAPOBJECT_PLACEMENT_StringToEnum(const std::string& str);



	static  std::string EVENT_PATH_EnumToString(const EVENT_PATH& eType);
	static  EVENT_PATH EVENT_PATH_StringToEnum(const std::string& str);

	static  INTERACT_TYPE INTERACT_TYPE_String_to_Enum(const std::string& str);
	static  std::string INTERACT_TYPE_Enum_to_String(const INTERACT_TYPE& eType);


	static ITEMTYPE	ITEMTYPE_String_To_Enum(const std::string& str);
	static std::string ITEMTYPE_Enum_To_String(const ITEMTYPE& eType);


	static MONSTER_TYPE	MONSTER_TYPE_String_To_Enum(const std::string& str);
	static std::string 	MONSTER_TYPE_Enum_To_String(const MONSTER_TYPE& eType);


	static OBJTYPE MONSTER_TYPE_TO_OBJ_TYPE(const MONSTER_TYPE& eType);
	static OBJTYPE CATEGORI_To_OBJTYPE(const MAPOBJECT_CATEGORY& eType);


	static const CUBE_EVENT_TYPE& Handle_Event_Index_To_CUBE_EVENT_TYPE(const _int& index);


	static const DOORTYPE& DOORTTYPE_Stirng_To_Enum(const std::string& str);
	static const std::string DOORTYPE_Enum_To_String(const DOORTYPE& eType);

	static const LIGHT_OBJECT_TYPE LIGHT_OBJECT_TYPE_String_To_Enum(const std::string& str);
	static const std::string	   LIGHT_OBJECT_TYPE_Enum_To_String(const LIGHT_OBJECT_TYPE& eType);



	static const SCENETYPE		   SCENETYPE_String_To_Enum(const std::string& str);
	static const std::string	   SCENETYPE_Enum_To_String(const SCENETYPE& eType);
};

END