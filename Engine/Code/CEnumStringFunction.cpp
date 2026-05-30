#include "CEnumStringFunction.h"


//----오브젝트 타입-----
 string CEnumStringFunction::OBJTYPE_EnumToString(OBJTYPE eType)
{
    switch (eType)
    {
    case Engine::OBJ_PLAYER: return "OBJ_PLAYER";
    case Engine::OBJ_TILE: return "OBJ_TILE";
    case Engine::OBJ_CUBE: return "OBJ_CUBE";
    case Engine::OBJ_TERRAIN: return "OBJ_TERRAIN";
    case Engine::OBJ_GRID: return "OBJ_GRID";
    case Engine::OBJ_EVENTCUBE: return "OBJ_EVENTCUBE";
    case Engine::OBJ_MONSTER: return "OBJ_MONSTER";
    case Engine::OBJ_MONSTER_GOBLIN_HAMMER: return "OBJ_MONSTER_GOBLIN_HAMMER";
    case Engine::OBJ_MONSTER_GOBLIN_FIREBALL: return "OBJ_MONSTER_GOBLIN_FIREBALL";
    case Engine::OBJ_MONSTER_ORGE_AXE: return "OBJ_MONSTER_ORGE_AXE";
    case Engine::OBJ_MONSTER_RAT: return "OBJ_MONSTER_RAT";
    case Engine::OBJ_BULLET: return "OBJ_BULLET";
    case Engine::OBJ_DOOR: return "OBJ_DOOR";
    case Engine::OBJ_MAPDECOR: return "OBJ_MAPDECOR";
    case Engine::OBJ_MAPOBSTACLE: return "OBJ_MAPOBSTACLE";
    case Engine::OBJ_MAPEVENT: return "OBJ_MAPEVENT";
    case Engine::OBJ_ITEM: return "OBJ_ITEM";
    case Engine::OBJ_LIGHT: return "OBJ_LIGHT";
    case Engine::OBJ_PARTICLE: return "OBJ_PARTICLE";
    case Engine::OBJ_PARTICLE_SYSTEM: return "OBJ_PARTICLE_SYSTEM";
    case Engine::OBJ_PARTICLE_SYSTEM_BLOOD: return "OBJ_PARTICLE_SYSTEM_BLOOD";
    case Engine::OBJ_PARTICLE_SYSTEM_SPARKLE: return "OBJ_PARTICLE_SYSTEM_SPARKLE";
    case Engine::OBJ_UI: return "OBJ_UI";
    case Engine::OBJ_NONE: return "OBJ_NONE";
    case Engine::OBJ_END: return "OBJ_END";
    default:
    {
        static const string unknown = "OBJ_UNKNOWN";
        return unknown;
    }
    }
}

OBJTYPE CEnumStringFunction::OBJTYPE_StringToEnum(const string& str)
{
    if (str == "OBJ_PLAYER") return Engine::OBJ_PLAYER;
    if (str == "OBJ_TILE") return Engine::OBJ_TILE;
    if (str == "OBJ_CUBE") return Engine::OBJ_CUBE;
    if (str == "OBJ_TERRAIN") return Engine::OBJ_TERRAIN;
    if (str == "OBJ_GRID") return Engine::OBJ_GRID;
    if (str == "OBJ_EVENTCUBE") return Engine::OBJ_EVENTCUBE;
    if (str == "OBJ_MONSTER") return Engine::OBJ_MONSTER;
    if (str == "OBJ_MONSTER_GOBLIN_HAMMER") return Engine::OBJ_MONSTER_GOBLIN_HAMMER;
    if (str == "OBJ_MONSTER_GOBLIN_FIREBALL") return Engine::OBJ_MONSTER_GOBLIN_FIREBALL;
    if (str == "OBJ_MONSTER_ORGE_AXE") return Engine::OBJ_MONSTER_ORGE_AXE;
    if (str == "OBJ_MONSTER_RAT") return Engine::OBJ_MONSTER_RAT;
    if (str == "OBJ_BULLET") return Engine::OBJ_BULLET;
    if (str == "OBJ_DOOR") return Engine::OBJ_DOOR;
    if (str == "OBJ_MAPDECOR") return Engine::OBJ_MAPDECOR;
    if (str == "OBJ_MAPOBSTACLE") return Engine::OBJ_MAPOBSTACLE;
    if (str == "OBJ_MAPEVENT") return Engine::OBJ_MAPEVENT;
    if (str == "OBJ_ITEM") return Engine::OBJ_ITEM;
    if (str == "OBJ_LIGHT") return Engine::OBJ_LIGHT;
    if (str == "OBJ_PARTICLE") return Engine::OBJ_PARTICLE;
    if (str == "OBJ_PARTICLE_SYSTEM") return Engine::OBJ_PARTICLE_SYSTEM;
    if (str == "OBJ_PARTICLE_SYSTEM_BLOOD") return Engine::OBJ_PARTICLE_SYSTEM_BLOOD;
    if (str == "OBJ_PARTICLE_SYSTEM_SPARKLE") return Engine::OBJ_PARTICLE_SYSTEM_SPARKLE;
    if (str == "OBJ_UI") return Engine::OBJ_UI;
    if (str == "OBJ_NONE") return Engine::OBJ_NONE;
    if (str == "OBJ_END") return Engine::OBJ_END;

    return Engine::OBJ_NONE;
}
// -------------------


// 오리진 오브젝트 타입
 std::string CEnumStringFunction::OBJORIGINTYPE_EnumToString(const OBJORIGINTYPE& eType)
{
    switch (eType)
    {
    case Engine::ORIGIN_STATIC: return "ORIGIN_STATIC";
    case Engine::ORIGIN_DYNAMIC: return "ORIGIN_DYNAMIC";
        break;
    default: return "UNKNOWN";
    }
}


 OBJORIGINTYPE CEnumStringFunction::OBJORIGINTYPE_StringToEnum(const std::string& str)
{
    if (str == "ORIGIN_STATIC") return ORIGIN_STATIC;
    if (str == "ORIGIN_DYNAMIC") return ORIGIN_DYNAMIC;

    
    return ORIGIN_STATIC;
}
// -------------------

//  층 
 std::string CEnumStringFunction::FLOOR_EnumToString(const FLOOR& eType)
{
    switch (eType)
    {
    case Engine::FLOOR_B3: return "FLOOR_B3";
    case Engine::FLOOR_B2: return "FLOOR_B2";
    case Engine::FLOOR_B1: return "FLOOR_B1";
    case Engine::FLOOR_GROUND:return "FLOOR_GROUND";
    case Engine::FLOOR_1F: return "FLOOR_1F";
    case Engine::FLOOR_2F:return "FLOOR_2F";
    case Engine::FLOOR_3F:return "FLOOR_3F";
    default: return "UNKNOWN";
    }
}

 FLOOR CEnumStringFunction::FLOOR_StringToEnum(const std::string& str)
{
    if (str == "FLOOR_B3") return FLOOR_B3;
    if (str == "FLOOR_B2") return FLOOR_B2;
    if (str == "FLOOR_B1") return FLOOR_B1;
    if (str == "FLOOR_GROUND") return FLOOR_GROUND;
    if (str == "FLOOR_1F") return FLOOR_1F;
    if (str == "FLOOR_2F") return FLOOR_2F;
    if (str == "FLOOR_3F") return FLOOR_3F;

    return FLOOR::FLOOR_END;
}
 std::string CEnumStringFunction::TILETYPE_Enum_To_String(const TILETYPE& eType)
 {
     switch (eType)
     {
     case Engine::TILE_FLOOR: return "TILE_FLOOR";
     case Engine::TILE_CEILING: return "TILE_CEILING";
     default: return "UNKNOWN";
     }
 }
 TILETYPE CEnumStringFunction::TILETYPE_String_To_Enum(const std::string& str)
 {
     if (str == "TILE_FLOOR")   return TILE_FLOOR;
     if (str == "TILE_CEILING") return TILE_CEILING;

     
     return TILETYPE::TILE_END;
 }
 std::string CEnumStringFunction::CUBE_TYPE_Enum_To_String(const CUBE_TYPE& eType)
 {
     switch (eType)
     {
     case Engine::CUBE_NONE:  return "CUBE_NONE";
     case Engine::CUBE_EVENT: return "CUBE_EVENT";
     case Engine::CUBE_MAPTOOL: return "CUBE_MAPTOOL";
     case Engine::CUBE_CREATURECANTGO: return "CUBE_CREATURECANTGO";
     default:   return "CUBE_UNKNOWN";
     }

    return "CUBE_UNKNOWN";
 }
 CUBE_TYPE CEnumStringFunction::CUBE_TYPE_String_To_Enum(const std::string& str)
 {
     if (str == "CUBE_NONE") { return CUBE_TYPE::CUBE_NONE; }
     if (str == "CUBE_EVENT") { return CUBE_TYPE::CUBE_EVENT; }
     if (str == "CUBE_MAPTOOL") { return CUBE_TYPE::CUBE_MAPTOOL; }
     if (str == "CUBE_CREATURECANTGO") { return CUBE_TYPE::CUBE_CREATURECANTGO; }
     if (str == "CUBE_UNKNOWN") { return CUBE_TYPE::CUBE_UNKNOWN; }

     return CUBE_TYPE::CUBE_UNKNOWN;
 }
//-----------------


 string CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(MAPOBJECT_CATEGORY eType)
{
	switch (eType)
	{
	case MAPOBJECT_CATEGORY::DECOR:          return "DECOR";
	case MAPOBJECT_CATEGORY::OBSTACLE:       return "OBSTACLE";
	case MAPOBJECT_CATEGORY::EVENT:          return "EVENT";
    case MAPOBJECT_CATEGORY::ITEM:           return "ITEM";
    case MAPOBJECT_CATEGORY::MONSTER:        return "MONSTER";
    case MAPOBJECT_CATEGORY::DOOR:           return "DOOR";
    case MAPOBJECT_CATEGORY::LIGHT:           return "LIGHT";
	default:
		return "UNKNOWN";
	}
	return "UNKNOWN";
}

 MAPOBJECT_CATEGORY CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(const std::string& str)
{
	if (str == "DECOR")         return MAPOBJECT_CATEGORY::DECOR;
	if (str == "OBSTACLE")      return MAPOBJECT_CATEGORY::OBSTACLE;
	if (str == "EVENT")         return MAPOBJECT_CATEGORY::EVENT;
    if (str == "ITEM")          return MAPOBJECT_CATEGORY::ITEM;
    if (str == "MONSTER")       return MAPOBJECT_CATEGORY::MONSTER;
    if (str == "DOOR")          return MAPOBJECT_CATEGORY::DOOR;
    if (str == "LIGHT")          return MAPOBJECT_CATEGORY::LIGHT;

	return MAPOBJECT_CATEGORY::_END; // fallback
}

 string CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(const MAPOBJECT_PLACEMENT& eType)
{
	switch (eType)
	{
	case MAPOBJECT_PLACEMENT::GROUND: return "GROUND";
	case MAPOBJECT_PLACEMENT::WALL: return "WALL";
	default:
		return "UNKNOWN";
	}
	return "UNKNOWN";
}

 MAPOBJECT_PLACEMENT CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(const std::string& str)
{
	if (str == "GROUND")     return MAPOBJECT_PLACEMENT::GROUND;
	if (str == "WALL") return MAPOBJECT_PLACEMENT::WALL;

	return MAPOBJECT_PLACEMENT::_END;
}

 std::string CEnumStringFunction::EVENT_PATH_EnumToString(const EVENT_PATH& eType)
{
    switch (eType)
    {
    case Engine::EVENT_PATH::ANTENAE:        return "ANTENAE";
    case Engine::EVENT_PATH::BARREL:         return "BARREL";
    case Engine::EVENT_PATH::BEERTABLE:      return "BEERTABLE";
    case Engine::EVENT_PATH::CHICKENTABLE:   return "CHICKENTABLE";
    case Engine::EVENT_PATH::BUSH:           return "BUSH";
    case Engine::EVENT_PATH::FLOWERBUSH:     return "FLOWERBUSH";
    case Engine::EVENT_PATH::FLOWERPOT:      return "FLOWERPOT";
    case Engine::EVENT_PATH::LONGPOT:        return "LONGPOT";
    case Engine::EVENT_PATH::FATPOT:         return "FATPOT";
    case Engine::EVENT_PATH::DUMMY:          return "DUMMY";
    case Engine::EVENT_PATH::SWITCH:         return "SWITCH";
    case Engine::EVENT_PATH::PHOTOSWITCH:    return "PHOTOSWITCH";
    case Engine::EVENT_PATH::SKULLSWITCH:    return "SKULLSWITCH";
    case Engine::EVENT_PATH::CORPSE:         return "CORPSE";
    case Engine::EVENT_PATH::CHEST:          return "CHEST";
    case Engine::EVENT_PATH::GOLDCHEST:      return "GOLDCHEST";
    case Engine::EVENT_PATH::WALLCHEST:      return "WALLCHEST";
    case Engine::EVENT_PATH::ORB:            return "ORB";
    case Engine::EVENT_PATH::ORBFIELD:       return "ORBFIELD";
    case Engine::EVENT_PATH::BOOK:           return "BOOK";
    case Engine::EVENT_PATH::POUCH:          return "POUCH";
    case Engine::EVENT_PATH::WELL:           return "WELL";
    case Engine::EVENT_PATH::BROKEWALL:      return "BROKEWALL";

    case Engine::EVENT_PATH::INTERECT_END:   return "INTERECT_END";

    case Engine::EVENT_PATH::CHAOS:          return "CHAOS";

    default:                                 return "UNKNOWN";
    }
}

 EVENT_PATH CEnumStringFunction::EVENT_PATH_StringToEnum(const std::string& str)
{

    if (str == "ANTENAE")        return EVENT_PATH::ANTENAE;
    if (str == "BARREL")         return EVENT_PATH::BARREL;
    if (str == "BEERTABLE")      return EVENT_PATH::BEERTABLE;
    if (str == "CHICKENTABLE")   return EVENT_PATH::CHICKENTABLE;
    if (str == "BUSH")           return EVENT_PATH::BUSH;
    if (str == "FLOWERBUSH")     return EVENT_PATH::FLOWERBUSH;
    if (str == "FLOWERPOT")      return EVENT_PATH::FLOWERPOT;
    if (str == "LONGPOT")        return EVENT_PATH::LONGPOT;
    if (str == "FATPOT")         return EVENT_PATH::FATPOT;
    if (str == "DUMMY")          return EVENT_PATH::DUMMY;
    if (str == "SWITCH")         return EVENT_PATH::SWITCH;
    if (str == "PHOTOSWITCH")    return EVENT_PATH::PHOTOSWITCH;
    if (str == "SKULLSWITCH")    return EVENT_PATH::SKULLSWITCH;
    if (str == "CORPSE")         return EVENT_PATH::CORPSE;
    if (str == "CHEST")          return EVENT_PATH::CHEST;
    if (str == "GOLDCHEST")      return EVENT_PATH::GOLDCHEST;
    if (str == "WALLCHEST")      return EVENT_PATH::WALLCHEST;
    if (str == "ORB")            return EVENT_PATH::ORB;
    if (str == "ORBFIELD")       return EVENT_PATH::ORBFIELD;
    if (str == "BOOK")           return EVENT_PATH::BOOK;
    if (str == "POUCH")          return EVENT_PATH::POUCH;
    if (str == "WELL")           return EVENT_PATH::WELL;
    if (str == "BROKEWALL")      return EVENT_PATH::BROKEWALL;

    if (str == "CHAOS")          return EVENT_PATH::CHAOS;


    
    return EVENT_PATH::UNKNOWN;
}

 INTERACT_TYPE CEnumStringFunction::INTERACT_TYPE_String_to_Enum(const std::string& str)
{
    if (str == "ANTENAE")        return INTERACT_TYPE::ANTENAE;
    if (str == "BARREL")         return INTERACT_TYPE::BARREL;
    if (str == "BEERTABLE")      return INTERACT_TYPE::BEERTABLE;
    if (str == "CHICKENTABLE")   return INTERACT_TYPE::CHICKENTABLE;
    if (str == "BUSH")           return INTERACT_TYPE::BUSH;
    if (str == "FLOWERBUSH")     return INTERACT_TYPE::FLOWERBUSH;
    if (str == "FLOWERPOT")      return INTERACT_TYPE::FLOWERPOT;
    if (str == "LONGPOT")        return INTERACT_TYPE::LONGPOT;
    if (str == "FATPOT")         return INTERACT_TYPE::FATPOT;
    if (str == "DUMMY")          return INTERACT_TYPE::DUMMY;
    if (str == "SWITCH")         return INTERACT_TYPE::SWITCH;
    if (str == "PHOTOSWITCH")    return INTERACT_TYPE::PHOTOSWITCH;
    if (str == "SKULLSWITCH")    return INTERACT_TYPE::SKULLSWITCH;
    if (str == "CORPSE")         return INTERACT_TYPE::CORPSE;
    if (str == "CHEST")          return INTERACT_TYPE::CHEST;
    if (str == "GOLDCHEST")      return INTERACT_TYPE::GOLDCHEST;
    if (str == "WALLCHEST")      return INTERACT_TYPE::WALLCHEST;
    if (str == "ORB")            return INTERACT_TYPE::ORB;
    if (str == "ORBFIELD")       return INTERACT_TYPE::ORBFIELD;
    if (str == "BOOK")           return INTERACT_TYPE::BOOK;
    if (str == "POUCH")          return INTERACT_TYPE::POUCH;
    if (str == "WELL")           return INTERACT_TYPE::WELL;
    if (str == "BROKEWALL")      return INTERACT_TYPE::BROKEWALL;

    return INTERACT_TYPE::UNKNOWN;
}

std::string CEnumStringFunction::INTERACT_TYPE_Enum_to_String(const INTERACT_TYPE& eType)
{
    switch (eType)
    {
    case Engine::INTERACT_TYPE::ANTENAE:        return "ANTENAE";
    case Engine::INTERACT_TYPE::BARREL:         return "BARREL";
    case Engine::INTERACT_TYPE::BEERTABLE:      return "BEERTABLE";
    case Engine::INTERACT_TYPE::CHICKENTABLE:   return "CHICKENTABLE";
    case Engine::INTERACT_TYPE::BUSH:           return "BUSH";
    case Engine::INTERACT_TYPE::FLOWERBUSH:     return "FLOWERBUSH";
    case Engine::INTERACT_TYPE::FLOWERPOT:      return "FLOWERPOT";
    case Engine::INTERACT_TYPE::LONGPOT:        return "LONGPOT";
    case Engine::INTERACT_TYPE::FATPOT:         return "FATPOT";
    case Engine::INTERACT_TYPE::DUMMY:          return "DUMMY";
    case Engine::INTERACT_TYPE::SWITCH:         return "SWITCH";
    case Engine::INTERACT_TYPE::PHOTOSWITCH:    return "PHOTOSWITCH";
    case Engine::INTERACT_TYPE::SKULLSWITCH:    return "SKULLSWITCH";
    case Engine::INTERACT_TYPE::CORPSE:         return "CORPSE";
    case Engine::INTERACT_TYPE::CHEST:          return "CHEST";
    case Engine::INTERACT_TYPE::GOLDCHEST:      return "GOLDCHEST";
    case Engine::INTERACT_TYPE::WALLCHEST:      return "WALLCHEST";
    case Engine::INTERACT_TYPE::ORB:            return "ORB";
    case Engine::INTERACT_TYPE::ORBFIELD:       return "ORBFIELD";
    case Engine::INTERACT_TYPE::BOOK:           return "BOOK";
    case Engine::INTERACT_TYPE::POUCH:          return "POUCH";
    case Engine::INTERACT_TYPE::WELL:           return "WELL";
    case Engine::INTERACT_TYPE::BROKEWALL:      return "BROKEWALL";
    }
    
    return "UNKNOWN";
}

ITEMTYPE CEnumStringFunction::ITEMTYPE_String_To_Enum(const std::string& str)
{
    if (str == "ITEM_BERRIES")         return Engine::ITEM_BERRIES;
    if (str == "ITEM_HEALTH_SHARD")    return Engine::ITEM_HEALTH_SHARD;
    if (str == "ITEM_ARMOR_SHARD")     return Engine::ITEM_ARMOR_SHARD;
    if (str == "ITEM_GREATER_POTION")  return Engine::ITEM_GREATER_POTION;
    if (str == "ITEM_BROZE_COINS")     return Engine::ITEM_BROZE_COINS;
    if (str == "ITEM_SILVER_COINS")    return Engine::ITEM_SILVER_COINS;
    if (str == "ITEM_GOLD_COINS")      return Engine::ITEM_GOLD_COINS;
    if (str == "ITEM_COIN_PURSE")      return Engine::ITEM_COIN_PURSE;
    if (str == "ITEM_GOLD_CHALICE")    return Engine::ITEM_GOLD_CHALICE;
    if (str == "ITEM_FIREBALL_RING")   return Engine::ITEM_FIREBALL_RING;
    if (str == "ITEM_ICEWAND")         return Engine::ITEM_ICEWAND;
    if (str == "ITEM_GUN")             return Engine::ITEM_GUN;
    if (str == "ITEM_LOW_MANA")        return Engine::ITEM_LOW_MANA;
    if (str == "ITEM_FIRE_SHARD")      return Engine::ITEM_FIRE_SHARD;
    if (str == "ITEM_ICE_SHARD")       return Engine::ITEM_ICE_SHARD;
    if (str == "ITEM_GUN_BULLET")      return Engine::ITEM_GUN_BULLET;
    if (str == "ITEM_BRONZE_KEY")      return Engine::ITEM_BRONZE_KEY;
    if (str == "ITEM_SILVER_KEY")      return Engine::ITEM_SILVER_KEY;
    if (str == "ITEM_GOLD_KEY")        return Engine::ITEM_GOLD_KEY;
    if (str == "ITEM_TREASURE_KEY")    return Engine::ITEM_TREASURE_KEY;

    return Engine::ITEMTYPE::ITEM_NONE; // UNKNOWN or default
}

std::string CEnumStringFunction::ITEMTYPE_Enum_To_String(const ITEMTYPE& eType)
{
    switch (eType)
    {
    case Engine::ITEM_BERRIES:        return "ITEM_BERRIES";
    case Engine::ITEM_HEALTH_SHARD:   return "ITEM_HEALTH_SHARD";
    case Engine::ITEM_ARMOR_SHARD:    return "ITEM_ARMOR_SHARD";
    case Engine::ITEM_GREATER_POTION: return "ITEM_GREATER_POTION";
    case Engine::ITEM_BROZE_COINS:    return "ITEM_BROZE_COINS";
    case Engine::ITEM_SILVER_COINS:   return "ITEM_SILVER_COINS";
    case Engine::ITEM_GOLD_COINS:     return "ITEM_GOLD_COINS";
    case Engine::ITEM_COIN_PURSE:     return "ITEM_COIN_PURSE";
    case Engine::ITEM_GOLD_CHALICE:   return "ITEM_GOLD_CHALICE";
    case Engine::ITEM_FIREBALL_RING:  return "ITEM_FIREBALL_RING";
    case Engine::ITEM_ICEWAND:        return "ITEM_ICEWAND";
    case Engine::ITEM_GUN:            return "ITEM_GUN";
    case Engine::ITEM_LOW_MANA:       return "ITEM_LOW_MANA";
    case Engine::ITEM_FIRE_SHARD:     return "ITEM_FIRE_SHARD";
    case Engine::ITEM_ICE_SHARD:      return "ITEM_ICE_SHARD";
    case Engine::ITEM_GUN_BULLET:     return "ITEM_GUN_BULLET";
    case Engine::ITEM_BRONZE_KEY:     return "ITEM_BRONZE_KEY";
    case Engine::ITEM_SILVER_KEY:     return "ITEM_SILVER_KEY";
    case Engine::ITEM_GOLD_KEY:       return "ITEM_GOLD_KEY";
    case Engine::ITEM_TREASURE_KEY:   return "ITEM_TREASURE_KEY";
    default:                          return "UNKNOWN";
    }
}

MONSTER_TYPE CEnumStringFunction::MONSTER_TYPE_String_To_Enum(const std::string& str)
{
    

    if (str == "MONSTER_GOBLIN_HAMMER")     return Engine::MONSTER_TYPE::MONSTER_GOBLIN_HAMMER;
    if (str == "MONSTER_GOBLIN_FIREBALL")   return Engine::MONSTER_TYPE::MONSTER_GOBLIN_FIREBALL;
    if (str == "MONSTER_ORGE_AXE")          return Engine::MONSTER_TYPE::MONSTER_ORGE_AXE;
    if (str == "MONSTER_WIZARD")          return Engine::MONSTER_TYPE::MONSTER_WIZARD;
    if (str == "MONSTER_SKELETON_SWORD")          return Engine::MONSTER_TYPE::MONSTER_SKELETON_SWORD;
    if (str == "MONSTER_RAT")               return Engine::MONSTER_TYPE::MONSTER_RAT;

    return MONSTER_TYPE::MONSTER_END;
}

std::string CEnumStringFunction::MONSTER_TYPE_Enum_To_String(const MONSTER_TYPE& eType)
{
    switch (eType)
    {
    case Engine::MONSTER_GOBLIN_HAMMER:     return "MONSTER_GOBLIN_HAMMER";
    case Engine::MONSTER_GOBLIN_FIREBALL:   return "MONSTER_GOBLIN_FIREBALL";
    case Engine::MONSTER_ORGE_AXE:          return "MONSTER_ORGE_AXE";
    case Engine::MONSTER_RAT:               return "MONSTER_RAT";
    case Engine::MONSTER_SKELETON_SWORD:    return "MONSTER_SKELETON_SWORD";
    case Engine::MONSTER_WIZARD:            return "MONSTER_WIZARD";
    default:                                return "UNKNONW";
    }

    return "UNKNONW";
}

OBJTYPE CEnumStringFunction::MONSTER_TYPE_TO_OBJ_TYPE(const MONSTER_TYPE& eType)
{
    switch (eType)
    {
    case Engine::MONSTER_GOBLIN_HAMMER:     return OBJTYPE::OBJ_MONSTER_GOBLIN_HAMMER;
    case Engine::MONSTER_GOBLIN_FIREBALL:   return OBJTYPE::OBJ_MONSTER_GOBLIN_FIREBALL;
    case Engine::MONSTER_ORGE_AXE:          return OBJTYPE::OBJ_MONSTER_ORGE_AXE;
    case Engine::MONSTER_RAT:               return OBJTYPE::OBJ_MONSTER_RAT;
    case Engine::MONSTER_WIZARD:            return OBJTYPE::OBJ_MONSTER_WIZARD;
    case Engine::MONSTER_SKELETON_SWORD:    return OBJTYPE::OBJ_MONSTER_SKELETON_SWORD;
    default:                                return OBJ_NONE;
    }
}

OBJTYPE CEnumStringFunction::CATEGORI_To_OBJTYPE(const MAPOBJECT_CATEGORY& eType)
{
    switch (eType)
    {
    case Engine::MAPOBJECT_CATEGORY::DECOR:     return OBJTYPE::OBJ_MAPDECOR;
    case Engine::MAPOBJECT_CATEGORY::OBSTACLE:  return OBJTYPE::OBJ_MAPOBSTACLE;
    case Engine::MAPOBJECT_CATEGORY::EVENT:     return OBJTYPE::OBJ_MAPEVENT;
    case Engine::MAPOBJECT_CATEGORY::ITEM:      return OBJTYPE::OBJ_ITEM;
    case Engine::MAPOBJECT_CATEGORY::MONSTER:   return OBJTYPE::OBJ_MONSTER;
    default:    return OBJTYPE::OBJ_NONE;
    }

    return OBJTYPE::OBJ_NONE;
}

const CUBE_EVENT_TYPE& CEnumStringFunction::Handle_Event_Index_To_CUBE_EVENT_TYPE(const _int& index)
{
    //7 번은 단순 그냥 공격맞으면 부셔지는 큐브
    if (index == 7)                     return CUBE_EVENT_TYPE::DESTRUCTIBLE_ATTACK;
    
    if (101 <= index && index <= 200)   return CUBE_EVENT_TYPE::PORTAL;
    if (201 <= index && index <= 300)   return CUBE_EVENT_TYPE::MOVE;
    if (401 <= index && index <= 500)   return CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC;

    // 포탈관련 Position Move 1101~1200  관련 Position 1201~1300
    if (1001 <= index && index <= 1300)   return CUBE_EVENT_TYPE::POSITION;

    return CUBE_EVENT_TYPE::_END;
}

const DOORTYPE& CEnumStringFunction::DOORTTYPE_Stirng_To_Enum(const std::string& str)
{
    if (str == "DEFAULT_WOOD_DOOR")         return Engine::DOORTYPE::DEFAULT_WOOD_DOOR;
    if (str == "DEFAULT_IRON_DOOR")         return Engine::DOORTYPE::DEFAULT_IRON_DOOR;
    if (str == "SWITCH_WOOD_DOOR")          return Engine::DOORTYPE::SWITCH_WOOD_DOOR;
    if (str == "SWITCH_IRON_DOOR")          return Engine::DOORTYPE::SWITCH_IRON_DOOR;
    if (str == "BRONZE_DOOR")               return Engine::DOORTYPE::BRONZE_DOOR;
    if (str == "SILVER_DOOR")               return Engine::DOORTYPE::SILVER_DOOR;
    if (str == "GOLD_DOOR")                 return Engine::DOORTYPE::GOLD_DOOR;
    if (str == "BOSS_DOOR")                 return Engine::DOORTYPE::BOSS_DOOR;


    return Engine::DOORTYPE::NONE_DOOR;
}

const std::string CEnumStringFunction::DOORTYPE_Enum_To_String(const DOORTYPE& eType)
{
    switch (eType)
    {
    case Engine::DEFAULT_WOOD_DOOR: return "DEFAULT_WOOD_DOOR";
    case Engine::DEFAULT_IRON_DOOR: return "DEFAULT_IRON_DOOR";
    case Engine::SWITCH_WOOD_DOOR:  return "SWITCH_WOOD_DOOR";
    case Engine::SWITCH_IRON_DOOR:  return "SWITCH_IRON_DOOR";
    case Engine::BRONZE_DOOR:       return "BRONZE_DOOR";
    case Engine::SILVER_DOOR:       return "SILVER_DOOR";
    case Engine::GOLD_DOOR:         return "GOLD_DOOR";
    case Engine::BOSS_DOOR:         return "BOSS_DOOR";
    default:                        return "UNKNOWN";
    }
}

const LIGHT_OBJECT_TYPE CEnumStringFunction::LIGHT_OBJECT_TYPE_String_To_Enum(const std::string& str)
{
    if (str == "CANDLE")            return Engine::LIGHT_OBJECT_TYPE::CANDLE;
    if (str == "CANDLETRIPLE")      return Engine::LIGHT_OBJECT_TYPE::CANDLETRIPLE;
    if (str == "WALLTORCH")         return Engine::LIGHT_OBJECT_TYPE::WALLTORCH;
    if (str == "GROUNDTORCH")       return Engine::LIGHT_OBJECT_TYPE::GROUNDTORCH;
    if (str == "HANGINGBRAZIER")    return Engine::LIGHT_OBJECT_TYPE::HANGINGBRAZIER;
    if (str == "STATUE")            return Engine::LIGHT_OBJECT_TYPE::STATUE;
    if (str == "CHANDELIER")        return Engine::LIGHT_OBJECT_TYPE::CHANDELIER;
    if (str == "FIREALTAR")         return Engine::LIGHT_OBJECT_TYPE::FIREALTAR;

    return Engine::LIGHT_OBJECT_TYPE::UNKNOWN;
}

const std::string CEnumStringFunction::LIGHT_OBJECT_TYPE_Enum_To_String(const LIGHT_OBJECT_TYPE& eType)
{
    switch (eType)
    {
    case Engine::LIGHT_OBJECT_TYPE::CANDLE:             return "CANDLE";
    case Engine::LIGHT_OBJECT_TYPE::CANDLETRIPLE:       return "CANDLETRIPLE";
    case Engine::LIGHT_OBJECT_TYPE::WALLTORCH:          return "WALLTORCH";
    case Engine::LIGHT_OBJECT_TYPE::GROUNDTORCH:        return "GROUNDTORCH";
    case Engine::LIGHT_OBJECT_TYPE::HANGINGBRAZIER:     return "HANGINGBRAZIER";
    case Engine::LIGHT_OBJECT_TYPE::STATUE:             return "STATUE";
    case Engine::LIGHT_OBJECT_TYPE::CHANDELIER:         return "CHANDELIER";
    case Engine::LIGHT_OBJECT_TYPE::FIREALTAR:          return "FIREALTAR";
    default:  return "UNKNOWN";
    }
}

const SCENETYPE CEnumStringFunction::SCENETYPE_String_To_Enum(const std::string& str)
{
    if (str == "TUTORIAL") return Engine::SCENETYPE::TUTORIAL;
    if (str == "STAGE1") return Engine::SCENETYPE::STAGE1;
    if (str == "STAGE2") return Engine::SCENETYPE::STAGE2;
    if (str == "BOSS") return Engine::SCENETYPE::BOSS;
    if (str == "BOSS2") return Engine::SCENETYPE::BOSS2;

    return SCENETYPE::SCENE_END;
}

const std::string CEnumStringFunction::SCENETYPE_Enum_To_String(const SCENETYPE& eType)
{
    switch (eType)
    {
    case Engine::TUTORIAL:return "TUTORIAL";
    case Engine::STAGE1: return "STAGE1";
    case Engine::STAGE2: return "STAGE2";
    case Engine::BOSS: return "BOSS";
    case Engine::BOSS2: return "BOSS2";
    default: return "UNKNOWN";
    }

    return "UNKNOWN";
}
