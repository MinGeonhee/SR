#include "CPathFunction.h"
#include "CEnumStringFunction.h"


inline const std::string CPathFunction::MapDataJsonPath(SCENETYPE eType)
{
	string SceneStr= CEnumStringFunction::SCENETYPE_Enum_To_String(eType) + ".json";

	if (SceneStr == "UNKNOWN")
		return "UNKNOWN";

	
	

	string MapDataPath = "../Bin/Resource/MapData/";

	return MapDataPath + SceneStr;
}
