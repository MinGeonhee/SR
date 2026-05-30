#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CPathFunction
{
public:
	static inline const std::wstring PlayerStart_Pos_Rot_Json = L"../Bin/Resource/MapData/PlayerStart.json";

	static inline const std::string MapDataJsonPath(SCENETYPE eType);


public:
	// 맵툴용 
	//그라운드 데코  Json png
	static inline const std::string GroundDecor_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Decor/Ground_Decor.json";
	static inline const std::wstring GroundDecor_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Decor/Ground_Decor.png";
	//벽      데코 Json png
	static inline const std::string WallDecor_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Decor/Wall_Decor.json";
	static inline const std::wstring WallDecor_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Decor/Wall_Decor.png";


	//방해물  
	static inline const std::string Obstacle_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Obstacle/Obstacle.json";
	static inline const std::wstring Obstacle_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Obstacle/Obstacle.png";

	//이벤트 Json png

	// 그라운드 이벤트 
	static inline const std::string GroundEvent_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Event/Ground_Event.json";
	static inline const std::wstring GroundEvent_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Event/Ground_Event.png";
	//벽    이벤트  Json png
	static inline const std::string WallEvent_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Event/Wall_Event.json";
	static inline const std::wstring WallEvent_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Event/Wall_Event.png";


	// 클라이언트용
	static inline const std::string GroundDecor_Json_Path_InClient = "../Bin/Resource/Texture/Object/Decor/Ground_Decor.json";
	static inline const std::wstring GroundDecor_Png_Path_InClient = L"../Bin/Resource/Texture/Object/Decor/Ground_Decor.png";
	//벽		데코 Json png
	static inline const std::string WallDecor_Json_Path_InClient = "../Bin/Resource/Texture/Object/Decor/Wall_Decor.json";
	static inline const std::wstring WallDecor_Png_Path_InClient = L"../Bin/Resource/Texture/Object/Decor/Wall_Decor.png";


	//방해물  
	static inline const std::string Obstacle_Json_Path_InClient = "../Bin/Resource/Texture/Object/Obstacle/Obstacle.json";
	static inline const std::wstring Obstacle_Png_Path_InClient = L"../Bin/Resource/Texture/Object/Obstacle/Obstacle.png";

	//이벤트 Json png

	// Interect Event 관련 클라에서 쓸떄 png json 파일
	static inline const std::string Interaction_Json_Path_InClient = "../Bin/Resource/Texture/Object/Event/Interaction/Interaction.json";
	static inline const std::wstring Interaction_Png_Path_InClient = L"../Bin/Resource/Texture/Object/Event/Interaction/Interaction.png";

	// Interect Event 관련 클라에서 쓸떄 png json 파일
	static inline const std::string Interaction_Json_Path_InEngine = "../../Client/Bin/Resource/Texture/Object/Event/Interaction/Interaction.json";
	static inline const std::wstring Interaction_Png_Path_InEngine = L"../../Client/Bin/Resource/Texture/Object/Event/Interaction/Interaction.png";


	//맵툴에서 찍을 아이템 아틸라스 png파일 위치와 json
	static inline const std::string MapToolItem_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Item//MapTool_Item.json";
	static inline const std::wstring MapToolItem_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Item//MapTool_Item.png";

	//맵툴에서 찍을 몬스터 아틸라스 png파일 위치와 json
	static inline const std::string MapToolMonster_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Monster/MapTool_Monster.json";
	static inline const std::wstring MapToolMonster_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Monster/MapTool_Monster.png";
	//2
	static inline const std::string MapToolMonster2_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Monster/MapTool_Monster2.json";
	static inline const std::wstring MapToolMonster2_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Monster/MapTool_Monster2.png";


	//맵툴에서 찍을 DOOR 아틸라스 png파일 위치와 json
	static inline const std::string MapToolDoor_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Door/MapTool_Door.json";
	static inline const std::wstring MapToolDoor_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Door/MapTool_Door.png";

	//맵툴에서 찍을 LightObject 아틸라스 png파일 위치와 json
	static inline const std::string MapToolLight_Json_Path_InMapTool = "../../Client/Bin/Resource/Texture/Object/Light/MapTool_Light.json";
	static inline const std::wstring MapToolLight_Png_Path_InMapTool = L"../../Client/Bin/Resource/Texture/Object/Light/MapTool_Light.png";

};

END