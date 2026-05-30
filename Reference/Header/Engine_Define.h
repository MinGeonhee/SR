#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#include <process.h>

#include <random>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <array>
#include <variant>
#include <queue>
#include <atomic>
#include <math.h>
#include <unordered_set>

#include <unordered_map>
#include <ctime>

#include "Engine_Macro.h"
#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include <crtdbg.h>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <io.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib") 

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>


#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Engine;

#endif // Engine_Define_h__
