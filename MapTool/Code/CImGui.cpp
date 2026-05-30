#include "pch.h"
#include "CImGui.h"
#include "CTileManager.h"
#include "CMapObjectManager.h"
#include "CResourceManager.h"
#include "CEnumStringFunction.h"
#include "CPathFunction.h"
#include "CToolManager.h"
#include "CTile.h"
#include "CTexture.h"
#include "CMapToolCube.h"
#include "CMapToolMonster.h"
#include "CMapToolTile.h"
#include "CMapToolItem.h"
#include "CMapToolLight.h"
#include "CMapToolEventObject.h"
#include "CCubeManager.h"
#include "CObjectManager.h"
#include "CEventCube.h"
#include "CMapToolDoor.h"
#include "CTimeManager.h"
#include "CCollisionManager.h"

IMPLEMENT_SINGLETON(CImGui)

#pragma region 생성 소멸자
CImGui::CImGui()
    : m_pGraphicDevice(nullptr), clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f))  , m_eCurFloor(FLOOR_B3)
     , m_ShowTextureWindow(false) , m_iSettingRoomNum(0) , m_IsTileTypeRender(false), m_IsShapeTypeRender(false)
    , m_ShowFixInfoWindow(false) , m_pFixTarget(nullptr) , m_iFixRoomNum(0) , m_iFix_EventIndexNum(0) , m_eMapToolType(TOOLTYPE::TOOL_NONE)
    , m_iFix_SwitchDoorCount(1) , m_iFix_EventBufferNum(0)
{
    m_sFileName = "MyDefaultName.json";
}

CImGui::~CImGui()
{
}
#pragma endregion

#pragma region 초기화 업데이트 렌더
void CImGui::Init(LPDIRECT3DDEVICE9 device)
{
    //디바이스 얻어오고 레퍼런스 카운터 체크
    m_pGraphicDevice = device;
    m_pGraphicDevice->AddRef();

    //DPI(화면 배율)에 따라 UI 요소의 크기를 조정하기 위한 스케일 팩터를 구합니다.
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)



    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDevice);



}
_int CImGui::Update_ImGui(const float& fTimeDelta)
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    m_eCurFloor = CToolManager::GetInstance()->GetCurFloor();

    
    //ImGui::Begin("Tool");
    MainWindow();
    EventIndexWindow();
   //ImGui::End();

    return 0;
}
void CImGui::LateUpdate_ImGui()
{
}
void CImGui::Render_ImGui()
{
    m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}
#pragma endregion

//메인 화면
void CImGui::MainWindow()
{
    ImGui::Begin("MainWindow", nullptr,ImGuiWindowFlags_MenuBar); // 메인 윈도우 시작

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tool Mode"))
        {
            if (ImGui::MenuItem("Map Tool", nullptr))
            {
                CToolManager::GetInstance()->Set_ToolMode(TOOLTYPE::TOOL_MAP);
            }
            if (ImGui::MenuItem("Object Tool", nullptr))
            {
                CToolManager::GetInstance()->Set_ToolMode(TOOLTYPE::TOOL_OBJECT);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


    if (CToolManager::GetInstance()->Get_ToolType() == TOOLTYPE::TOOL_MAP)
        MapToolWindow();
    else if (CToolManager::GetInstance()->Get_ToolType() == TOOLTYPE::TOOL_OBJECT)
        ObjectToolWindow();

    ImGui::End(); // 메인 윈도우 종료
}

void CImGui::EventIndexWindow()
{
    ImGui::Begin("EventIndexWindow");

    ImGui::Text("Event Index Window");

    auto Mgr = CCubeManager::GetInstance();
    auto MapObjMgr = CMapObjectManager::GetInstance();

    auto CubeList = Mgr -> GetCubeList();
    

    std::vector<CGameObject*> portalList;
    std::vector<CGameObject*> MoveList;
    std::vector<CGameObject*>  SwitchList;
    std::vector<CGameObject*>* EventObjectList;
    std::vector<CGameObject*>* DoorList;


    // --- PortalCube 그룹 (100~200) ---
    if (ImGui::CollapsingHeader("PortalCube (100~200)", ImGuiTreeNodeFlags_DefaultOpen))
    {
        portalList.clear();


        for (int i = 0; i < FLOOR::FLOOR_END; ++i)
        {
            for (auto Obj : CubeList[i])
            {
                if (!Obj) continue;
                if (static_cast<CMapToolCube*>(Obj)->GetCubeType() != CUBE_TYPE::CUBE_EVENT) continue;
                int eventIndex = Obj->Get_HandleEventIndex();
                if (eventIndex < 100 || eventIndex >= 200) continue;

                portalList.push_back(Obj);
            }
        }

        std::sort(portalList.begin(), portalList.end(), [](CGameObject* a, CGameObject* b) {
            return a->Get_HandleEventIndex() > b->Get_HandleEventIndex();
            });

        for (auto Portal : portalList)
        {
            int eventIndex = Portal->Get_HandleEventIndex();

            int linkedIndex = eventIndex + 1000;

            bool isLinked = CCubeManager::GetInstance()->IsHaveLinkedEventHandle(linkedIndex);

            if (isLinked == true)
                ImGui::Text("Index: %d | Status: Linked", eventIndex);
            if (isLinked == false)
                ImGui::Text("Index: %d | Status: Working", eventIndex);
        }

    }



    // --- MoveCube 그룹 (200~300) ---
    if (ImGui::CollapsingHeader("MoveCube (200~300)", ImGuiTreeNodeFlags_DefaultOpen))
    {
        
        MoveList.clear();

        for (int i = 0; i < FLOOR::FLOOR_END; ++i)
        {
            for (auto Obj : CubeList[i])
            {
                if (!Obj) continue;
                if (static_cast<CMapToolCube*>(Obj)->GetCubeType() != CUBE_TYPE::CUBE_EVENT) continue;
                int eventIndex = Obj->Get_HandleEventIndex();
                if (eventIndex < 200 || eventIndex >= 300) continue;

                MoveList.push_back(Obj);
            }
        }
        std::sort(MoveList.begin(), MoveList.end(), [](CGameObject* a, CGameObject* b) {
            return a->Get_HandleEventIndex() > b->Get_HandleEventIndex();
            });


        for (auto Move : MoveList)
        {
            int eventIndex = Move->Get_HandleEventIndex();

            int linkedIndex = eventIndex + 1000;

            bool isLinked = CCubeManager::GetInstance()->IsHaveLinkedEventHandle(linkedIndex);

            if (isLinked == true)
                ImGui::Text("Index: %d | Status: Linked", eventIndex);
            if (isLinked == false)
                ImGui::Text("Index: %d | Status: Working", eventIndex);
        }
    }

    // --- Door --- 관련
    //if (ImGui::CollapsingHeader("Switch Door ", ImGuiTreeNodeFlags_DefaultOpen))
    //{
    //    DoorList = MapObjMgr->GetMapObjectList(MAPOBJECT_CATEGORY::DOOR);
    //    EventObjectList = MapObjMgr->GetMapObjectList(MAPOBJECT_CATEGORY::EVENT);

    //    for (int i = 0; i < FLOOR::FLOOR_END; ++i)
    //    {

    //    }
    //}


    ImGui::End();
}

void CImGui::EventExplainWindow()
{
    ImGui::Begin("EventExplainWindow");

    if (ImGui::CollapsingHeader("DOOR && SWITCH", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "<< Door && SWITCH>>");

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 1 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => BRONZE_DOOR");


        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 2 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => SILVER_DOOR");


        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 3 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => GOLD_DOOR");

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 4 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => BOSS_DOOR");

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 5 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => DEFAULT_WOOD_DOOR");


        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 6 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => DEFAULT_IRON_DOOR");


        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " SWITCH ( 501 ~ 600 ) SWITCH_DOOR ( 1501 ~ 1600 ) ");
        ImGui::Text("    -- Linked With --    ");
        ImGui::TextColored(ImVec4(0.f, 1.f, 0.1f, 1.f), "SWITCH_DOOR ( 1501 ~ 1600 )");
    }

    if (ImGui::CollapsingHeader("CUBE", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "<< CUBE >>");

        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "<<Event Handle Index>>");

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 7 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => ATTACK_BROKE_CUBE ");

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 401 ~ 500 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => GIMIC_BROKE_CUBE ");


        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " PORTALCUBE ( 101 ~ 200 ) ");
        ImGui::Text("    -- Linked With --    ");
        ImGui::TextColored(ImVec4(0.f, 1.f, 0.1f, 1.f), " POSITION_CUBE ( 1101 ~ 1200 )");

        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " MOVECUBE ( 201 ~ 300 ) ");
        ImGui::Text("    -- Linked With --    ");
        ImGui::TextColored(ImVec4(0.f, 1.f, 0.1f, 1.f), " POSITION_CUBE ( 1101 ~ 1200 )");
    }

    if (ImGui::CollapsingHeader("CHEST", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "<< CHEST >>");

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 76 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => DEFAULT_CHEST");
        

        ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Event Index [ 77 ]");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " => TRAP_CHEST");
    }


    ImGui::End();

}

#pragma region 맵툴 큐브 / 타일 Window
// 그냥 단순 메인 UI창
void CImGui::MapToolWindow()
{
    Default_Setting_Render("Map Tool Made By Min GH");

    if (m_ShowTextureWindow)
        TextureWindow();

    FixInfoWindow(m_pFixTarget);
    EventExplainWindow();
}
// 텍스처 설정 윈도우
void CImGui::TextureWindow()
{
    ImGui::Begin("Texture UI");

    RenderLine();

    ImGui::TextColored(ImVec4({255, 0 , 0, 255})," Texture UI...Select Texture");

    RenderLine();

    TileTex_Render();

    ImGui::End();
}
void CImGui::FixInfoWindow(CGameObject* pObj)
{
    // Info 값에서 바꿀만한게 ROtation 값 정도? 방 번호 타입...등등 나중에 Handle 값 추가하면 그때 추가하면될듯?
    
    ImGui::Begin("FIX INFO UI");

    if (pObj == nullptr)
    {
        ImGui::TextColored(ImVec4({ 1.f , 0.f ,0.f,1.f }), "None....");
        m_ShowFixInfoWindow = false;
        ImGui::End();
        return;
    }

    if (pObj->Get_OBJTYPE() == OBJTYPE::OBJ_TILE)
    {
        if (static_cast<CMapToolTile*>(pObj)->Get_TileType() == TILETYPE::TILE_NONE)
        {
            ImGui::TextColored(ImVec4({ 1.f , 0.f ,0.f,1.f }), "None....");
            m_ShowFixInfoWindow = false;
            ImGui::End();
            return;
        }
    }

    _vec3 vPos = *pObj->Get_Transform()->GetInfoPos();
    float posX = vPos.x;
    float posY = vPos.y;
    float posZ = vPos.z;

    ImGui::TextColored(ImVec4({ 255 / 255.f  , 255 / 255.f , 0 ,1.f }), "<<   POS  >>");

    this->Position_Render(vPos);


    // X 입력
    if (ImGui::InputFloat("X", &posX, 0.1f, 1.0f, "%.3f"))
    {
        _vec3 newPos = { posX, vPos.y, vPos.z };
        pObj->Get_Transform()->Set_Pos(newPos.x, newPos.y , newPos.z);
        pObj->SemiUpdate();
    }

    // Y 입력
    if (ImGui::InputFloat("Y", &posY, 0.1f, 1.0f, "%.3f"))
    {
        _vec3 newPos = { vPos.x, posY, vPos.z };
        pObj->Get_Transform()->Set_Pos(newPos.x, newPos.y, newPos.z);
        pObj->SemiUpdate();
    }

    // Z 입력
    if (ImGui::InputFloat("Z", &posZ, 0.1f, 1.0f, "%.3f"))
    {
        _vec3 newPos = { vPos.x, vPos.y, posZ };
        pObj->Get_Transform()->Set_Pos(newPos.x, newPos.y, newPos.z);
        pObj->SemiUpdate();
    }


    
    /////////////////////////////



    ImGui::TextColored(ImVec4({ 255/ 255.f  , 255 / 255.f , 0 ,1.f }),"<<   ROTATION  >>");
    if (ImGui::Button(" TURN ", ImVec2( {50.f , 40.f })))
    {

        pObj->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90));
        pObj->SemiUpdate();

    }

    if (pObj->Get_OBJTYPE() == OBJ_CUBE)
    {
         ImGui::TextColored(ImVec4({ 255 / 255.f  , 255 / 255.f , 0 ,1.f }), "<<   CUBETYPE  >>");

         CMapToolCube* pCube = static_cast<CMapToolCube*>(pObj);
         CUBE_TYPE eType = pCube->GetCubeType();
         const char* TypeStr = Get_CubeTypeStr(eType);

         ImGui::Text(" Current Cube Type => [ %s ] ", TypeStr);
         
         ImGui::Text(" Change Cube Type =>");
         ImGui::SameLine();
         if (ImGui::Button(" EVENT"))
         {
             pCube->SetCubeType(CUBE_EVENT);
         }
         ImGui::SameLine();
         if (ImGui::Button(" MAPTOOL "))
         {
             pCube->SetCubeType(CUBE_MAPTOOL);
         }
         ImGui::SameLine();
         if (ImGui::Button(" CreatureCantGo "))
         {
             pCube->SetCubeType(CUBE_CREATURECANTGO);
         }

    }


    ImGui::TextColored(ImVec4({ 255 / 255.f  , 255 / 255.f , 0 ,1.f }), "<< RoomNum >>");
    ImGui::Text("Current Obj Room Num => [ %d ]", pObj->GetRoomNum());

    if (ImGui::InputInt(" Input Fix Room Num ", &m_iFixRoomNum));

    if (ImGui::Button(" APPLY ROOM NUM "))
    {
        pObj->SetRoomNum(m_iFixRoomNum);
        m_iFixRoomNum = 0;
    }

    ObjectHandle vHandle = pObj->Get_Handle();

    OBJTYPE Type = pObj->Get_OBJTYPE();

    ImGui::TextColored(ImVec4({ 255 / 255.f  , 255 / 255.f , 0 ,1.f }), "<< Handle Index Num >>");
    ImGui::Text(" Next Create Obj Index Num => [ %d ]", [&](){
        if (Type == OBJ_TILE)
             return CTileManager::GetInstance()->GetCurIndex();
        else if (Type == OBJ_CUBE)
            return CCubeManager::GetInstance()->GetCurIndex();
        else if (Type == OBJ_MAPOBSTACLE || Type == OBJ_MAPEVENT || Type == OBJ_MAPDECOR)
        {
            return (CMapObjectManager::GetInstance()->GetCurIndex(static_cast<CMapToolMapObject*>(pObj)->Get_Category()));
        }
        else
            return -1;
        }());

    ImGui::Text(" Obj Handle Event Index Num => [ %d ]", vHandle.m_sEventIndex );

    if (ImGui::InputInt(" Input Fix Handle Index Num ", &m_iFix_EventIndexNum));

    if (ImGui::Button(" APPLY   INDEX "))
    {
        pObj->Set_HandleEventIndex(m_iFix_EventIndexNum);
        m_iFix_EventIndexNum = 0;
    }


    if (pObj->Get_OBJTYPE() == OBJTYPE::OBJ_DOOR)
    {
        CMapToolDoor* pDoor =  static_cast<CMapToolDoor*>(pObj);

        DOORTYPE eType = pDoor->Get_DoorType();

        if (eType == DOORTYPE::SWITCH_IRON_DOOR || eType == DOORTYPE::SWITCH_WOOD_DOOR)
        {

            ImGui::Text(" Switch Door Count Num => [ %d ]", pDoor->Get_DoorSwitchCount());

            if (ImGui::InputInt(" Input Fix Switch Door Count Num ", &m_iFix_SwitchDoorCount));

            if (ImGui::Button(" APPLY Count  Num "))
            {
                pDoor->Set_DoorSwitchCount(m_iFix_SwitchDoorCount);
                m_iFix_SwitchDoorCount = 1;
            }

        }
    }

    if (pObj->Get_OBJTYPE() == OBJTYPE::OBJ_MAPEVENT)
    {
        CMapToolEventObject* pEvent = static_cast<CMapToolEventObject*>(pObj);


        ImGui::Text(" Current Event Buffer Num => [ %d ]", pEvent->GetBufferInt());

        if (ImGui::InputInt(" Input Fix Switch Door Count Num ", &m_iFix_EventBufferNum));

        if (ImGui::Button(" APPLY Count  Num "))
        {
            pEvent->SetBufferInt(m_iFix_EventBufferNum);
            m_iFix_EventBufferNum = 0;
        }
    }


    ImGui::End();
}
#pragma endregion


void CImGui::ObjectToolWindow()
{
    File_Menu_Render();
    Active_Mode();

    auto ptr =  CMapObjectManager::GetInstance()->GetBufferMapObject();

    Settting_Floor_Render();


    if (ptr == nullptr)
        ImGui::Text("Null Ptr");
    else if (ptr != nullptr)
        ImGui::Text("Ptr On");

    CurrentSprite_Render();
    CurrentCategori_Render();
    CurrentPlacement_Render();
    
    if (CMapObjectManager::GetInstance()->GetCurCategory() == MAPOBJECT_CATEGORY::EVENT)
    {
        ImGui::Text("Current Event Path => %s ", CEnumStringFunction::EVENT_PATH_EnumToString(
            CMapObjectManager::GetInstance()->GetCurEventPath()).c_str());
    }

    if (CMapObjectManager::GetInstance()->GetCurCategory() == MAPOBJECT_CATEGORY::EVENT)
    {
        ImGui::Text("Current Event Path => %s ", CEnumStringFunction::EVENT_PATH_EnumToString(
            CMapObjectManager::GetInstance()->GetCurEventPath()).c_str());
    }

    if (CMapObjectManager::GetInstance()->GetCurCategory() == MAPOBJECT_CATEGORY::MONSTER)
    {
        ImGui::Text("Current Monster Type => %s ", CEnumStringFunction::MONSTER_TYPE_Enum_To_String(
            CMapObjectManager::GetInstance()->GetCurMonsterType()).c_str());
    }    

    RenderAllSprite();
}

void CImGui::RenderAllSprite()
{
    // 데코 
    if (ImGui::CollapsingHeader("Decor", 0))
    {
        if (ImGui::TreeNode("Ground_Decor"))
        {
            if (ImGui::BeginChild("Ground_DecorChild", ImVec2(0, 200), true))
            {
                //단순 데코
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Decor") , CPathFunction::GroundDecor_Json_Path_InMapTool);
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }


        if (ImGui::TreeNode("Wall_Decor"))
        {
            if (ImGui::BeginChild("Wall_DecorChild", ImVec2(0, 200), true))
            {
                //방해물 관련
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Decor"), CPathFunction::WallDecor_Json_Path_InMapTool);
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }
    }


    // 방해물
    if (ImGui::CollapsingHeader("Obstacle", 0))
    {
        if (ImGui::TreeNode("Obstacle_"))
        {

            if (ImGui::BeginChild("Obstacle_Child", ImVec2(0, 200), true))
            {
                //단순 데코
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Obstacle"), CPathFunction::Obstacle_Json_Path_InMapTool);
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }
    }



    // 이벤트
    if (ImGui::CollapsingHeader("Event", 0))
    {    //C:\Users\mgh11\Desktop\SR_ - framework_1.0ver\Client\Bin\Resource\Texture\Object\Ground


        if (ImGui::TreeNode("Ground_Event"))
        {
            if (ImGui::BeginChild("Ground_EventChild"), ImVec2(0, 200), true)
            {
                //이벤트 관련
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Event"), CPathFunction::GroundEvent_Json_Path_InMapTool);
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }


        if (ImGui::TreeNode("Wall_Event"))
        {
            if (ImGui::BeginChild("Wall_EventChild"), ImVec2(0, 200), true)
            {
                //이벤트 관련
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Event"), CPathFunction::WallEvent_Json_Path_InMapTool);
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }

    }

    //아이템

    if (ImGui::CollapsingHeader("ITEM", 0))
    {
        if (ImGui::TreeNode("RECOVERY"))
        {
            if (ImGui::BeginChild("RECOVERYChild", ImVec2(0, 200), true))
            {
                //회복류
                Render_Atlas_Item(CResourceManager::GetInstance()->Get<CTexture>(L"Item"), CPathFunction::MapToolItem_Json_Path_InMapTool, "RECOVERY");
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("COINS"))
        {
            if (ImGui::BeginChild("COINSChild", ImVec2(0, 200), true))
            {
                //코인
                Render_Atlas_Item(CResourceManager::GetInstance()->Get<CTexture>(L"Item"), CPathFunction::MapToolItem_Json_Path_InMapTool, "COINS");
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("WEAPONS"))
        {
            if (ImGui::BeginChild("WEAPONSChild", ImVec2(0, 200), true))
            {
                //무기
                Render_Atlas_Item(CResourceManager::GetInstance()->Get<CTexture>(L"Item"), CPathFunction::MapToolItem_Json_Path_InMapTool, "WEAPONS");
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("BULLETS"))
        {
            if (ImGui::BeginChild("BULLETSChild", ImVec2(0, 200), true))
            {
                //총알
                Render_Atlas_Item(CResourceManager::GetInstance()->Get<CTexture>(L"Item"), CPathFunction::MapToolItem_Json_Path_InMapTool, "BULLETS");
            }
            
            ImGui::EndChild();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("KEYS"))
        {
            if (ImGui::BeginChild("KEYSChild", ImVec2(0, 200), true))
            {
                //키
                Render_Atlas_Item(CResourceManager::GetInstance()->Get<CTexture>(L"Item"), CPathFunction::MapToolItem_Json_Path_InMapTool, "KEYS");
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }
    }


    //몬스터
    if (ImGui::CollapsingHeader("MONSTER", 0))
    {
        if (ImGui::TreeNode("MONSTER_1"))
        {
            if (ImGui::BeginChild("MONSTER_1Child", ImVec2(0, 200), true))
            {
                //회복류
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Monster"), CPathFunction::MapToolMonster_Json_Path_InMapTool);
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("MONSTER_2"))
        {
            if (ImGui::BeginChild("MONSTER_2Child", ImVec2(0, 200), true))
            {
                //회복류
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Monster2"), CPathFunction::MapToolMonster2_Json_Path_InMapTool);
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }
    }

    //문
    if (ImGui::CollapsingHeader("DOOR", 0))
    {
        if (ImGui::TreeNode("DOOR_1"))
        {
            if (ImGui::BeginChild("DOOR_1Child", ImVec2(0, 200), true))
            {
                
                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Door"), CPathFunction::MapToolDoor_Json_Path_InMapTool);
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }
    }

    //Light 오브젝트
    if (ImGui::CollapsingHeader("Light", 0))
    {
        if (ImGui::TreeNode("Light_1"))
        {
            if (ImGui::BeginChild("Light_1Child", ImVec2(0, 200), true))
            {

                Render_Atlas(CResourceManager::GetInstance()->Get<CTexture>(L"Light"), CPathFunction::MapToolLight_Json_Path_InMapTool);
            }

            ImGui::EndChild();
            ImGui::TreePop();
        }
    }

}

void CImGui::CurrentSprite_Render()
{
    SpriteRect vSprite = CMapObjectManager::GetInstance()->GetCurSpriteRect();

    
    if (CMapObjectManager::GetInstance()->GetCurCategory() == MAPOBJECT_CATEGORY::_END || CMapObjectManager::GetInstance()->GetCurPlacement() == MAPOBJECT_PLACEMENT::_END)
        return;

    CurrentSprite_ImageRender(vSprite);


    ImGui::Text("Start X : %f", vSprite.x );
    ImGui::SameLine();
    ImGui::Text("  Start Y : %f", vSprite.y);

    ImGui::Text("Width X : %f", vSprite.w);
    ImGui::SameLine();
    ImGui::Text("  Height Y : %f", vSprite.h);

    ImGui::Text("Max Width: %f", vSprite.Mw);
    ImGui::SameLine();
    ImGui::Text("  Max Height : %f", vSprite.Mw);
}

void CImGui::CurrentSprite_ImageRender(const SpriteRect& vSptriteRect)
{
    auto Categori = CMapObjectManager::GetInstance()->GetCurCategory();
    auto PlaceMent = CMapObjectManager::GetInstance()->GetCurPlacement();


    ImTextureID TexID = 0;

    
    if (Categori == MAPOBJECT_CATEGORY::DECOR)
    {
        if (PlaceMent == MAPOBJECT_PLACEMENT::GROUND)
        {
            TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Decor")->GetIDirect3DBaseTexture9();
        }
        else if (PlaceMent == MAPOBJECT_PLACEMENT::WALL)
        {
            TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Decor")->GetIDirect3DBaseTexture9();
        }
    }
     
    if (Categori == MAPOBJECT_CATEGORY::OBSTACLE)
    {
        TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Obstacle")->GetIDirect3DBaseTexture9();
    }

    if (Categori == MAPOBJECT_CATEGORY::EVENT)
    {
        if (PlaceMent == MAPOBJECT_PLACEMENT::GROUND)
        {
            TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Event")->GetIDirect3DBaseTexture9();

        }
        else if (PlaceMent == MAPOBJECT_PLACEMENT::WALL)
        {
            TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Event")->GetIDirect3DBaseTexture9();

        }
    }

    if (Categori == MAPOBJECT_CATEGORY::ITEM)
    {
        TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Item")->GetIDirect3DBaseTexture9();
    }

    if (Categori == MAPOBJECT_CATEGORY::MONSTER)
    {
        TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Monster")->GetIDirect3DBaseTexture9();
    }

    if (Categori == MAPOBJECT_CATEGORY::DOOR)
    {
        TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Door")->GetIDirect3DBaseTexture9();
    }


    _vec2 lt = LT(vSptriteRect.x, vSptriteRect.y, vSptriteRect.w, vSptriteRect.h, vSptriteRect.Mw, vSptriteRect.Mh);
    _vec2 rb = RB(vSptriteRect.x, vSptriteRect.y, vSptriteRect.w, vSptriteRect.h, vSptriteRect.Mw, vSptriteRect.Mh);



    ImGui::ImageButton("NONE", TexID, ImVec2(16, 16), ImVec2(lt.x, lt.y), ImVec2(rb.x, rb.y));

}

void CImGui::CurrentCategori_Render()
{
    Render_Type("Current Categori",CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(CMapObjectManager::GetInstance()->GetCurCategory()).c_str());
}

void CImGui::CurrentPlacement_Render()
{
    Render_Type("Current Placement", Get_PlacementStr(CMapObjectManager::GetInstance()->GetCurPlacement()));
}


//라인그려주는 함수
void CImGui::RenderLine()
{
    // 윈도우 내부 너비 가져오기
    float fullWidth = ImGui::GetContentRegionAvail().x;

    // 현재 커서 위치 (y만 저장)
    float yPos = ImGui::GetCursorScreenPos().y + ImGui::GetTextLineHeight() * 0.5f;

    // 줄 긋기 (가로 선)
    ImVec2 p1 = ImGui::GetCursorScreenPos();
    ImVec2 p2 = ImVec2(p1.x + fullWidth, p1.y);

    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(p1.x, yPos),
        ImVec2(p2.x, yPos),
        IM_COL32(180, 180, 180, 255),
        1.0f
    );
    ImGui::NewLine();
}
void CImGui::MousePos_Render()
{
    ImGui::TextColored(ImVec4(255, 255, 255, 255), " << Mouse Position >>");

    ImGui::Text(" Current Mouse Pos ");
    _vec3 CurTilePos = CTileManager::GetInstance()->GetBuffeRectPos();

    ImGui::Text("{  X : %f | Y : %f  | Z : %f  } ", CurTilePos.x, CurTilePos.y, CurTilePos.z);
}
//메뉴 창마다 가지고있을 디폴트 세팅 
void CImGui::Default_Setting_Render(const char* Text)
{
    File_Menu_Render();

    Active_Mode();
    Render_Mode();

    RenderLine();

    ImGui::TextColored(ImVec4(255, 0, 0, 255),Text);

    RenderLine();
    Settting_Floor_Render();
    
    RenderLine();
    ObjectInfo_Render();

    RenderLine();    
    MousePos_Render();

    RenderLine();
    Setting_Texture_Render();

    RenderLine();
    Setting_RoomNum_Render();

    RenderLine();
    Setting_BuildMode_Render();

    if (m_IsShapeTypeRender)
    {
        RenderLine();
        Setting_ShapeType_Render();
    }

    if ( m_IsTileTypeRender)
    {
        RenderLine();
        Setting_TileType_Render();
    }
}

void CImGui::File_Menu_Render()
{
    if (ImGui::Button("FILE", ImVec2(40, 20)))
    {
        ImGui::OpenPopup("FileMenu");
    }

    if (ImGui::BeginPopup("FileMenu"))
    {
        if (ImGui::BeginMenu("Save"))
        {
            ImGui::InputText("File ", &m_sFileName);
            string filename = m_sFileName;

            string FilePath = FILEPATH;

            string FullFilePath = FilePath + filename;

            if (ImGui::Button("Apply Save"))
            {
                Save_MapData(FullFilePath);

                ImGui::CloseCurrentPopup();
            }
            ImGui::EndMenu(); // BeginMenu가 true일 때만 호출
        }

        if (ImGui::BeginMenu("Load"))
        {
            // 디폴트 맵데이터 경로를 넣어주고 폴더안에있는 파일명을 벡터에 담아둔다
            vector<string> vFileName = Lead_Folder(FILEPATH);

            for (int i = 0; i < vFileName.size(); ++i)
            {
                if (ImGui::MenuItem(vFileName[i].c_str()))
                {
                    m_pFixTarget = nullptr;
                    CCollisionManager::GetInstance()->Delete_ObjectList();
                    CMapObjectManager::GetInstance()->SetInfoMapObject(nullptr);
                    CTileManager::GetInstance()->SetTargetObject(nullptr);
                    CCubeManager::GetInstance()->SetTargetObject(nullptr);

                    string FullPath = FILEPATH + vFileName[i];
                    CTileManager::GetInstance()->LoadTile(FullPath);
                    CCubeManager::GetInstance()->LoadCube(FullPath);
                    CMapObjectManager::GetInstance()->LoadMapObjectFromJson(FullPath);

                }
            }

            ImGui::EndMenu(); // BeginMenu가 true일 때만 호출
        }

        ImGui::EndPopup(); // BeginPopup이 true일 때만 호출
    }
}

void CImGui::Active_Mode()
{
    auto Mgr = CToolManager::GetInstance();

    MapToolActive   list = CToolManager::GetInstance()->GetActiveList();

    if (ImGui::CollapsingHeader("Active False Button", 0))
    {
        if (ImGui::TreeNode("Tile"))
        {
            if (ImGui::Checkbox(" Floor ", &list.isActiveFloorTile))
            {
                Mgr->SetActive(ACTIVE_OBJECT::TILE_FLOOR , list.isActiveFloorTile);
            }

            ImGui::SameLine();

            if (ImGui::Checkbox(" Ceiling ", &list.isActiveTileCeiling))
            {
                Mgr->SetActive(ACTIVE_OBJECT::TILE_CELILING, list.isActiveTileCeiling);
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Cube"))
        {

            if (ImGui::Checkbox(" Cube ", &list.isActiveCube))
            {
                Mgr->SetActive(ACTIVE_OBJECT::CUBE, list.isActiveCube);
            }

            ImGui::SameLine();

            if (ImGui::Checkbox(" Event Cube ", &list.isActiveEventCube))
            {
                Mgr->SetActive(ACTIVE_OBJECT::EVENTCUBE, list.isActiveEventCube);
            }

            if (ImGui::Checkbox(" Player Can't Go Cube ", &list.isActiveCreatureCantGoCube))
            {
                Mgr->SetActive(ACTIVE_OBJECT::CREATURECANTGOCUBE, list.isActiveCreatureCantGoCube);
            }



            ImGui::TreePop();
        }


        if (ImGui::TreeNode("Map Object"))
        {

            if (ImGui::Checkbox(" Decor ", &list.isActiveDecor))
            {
                Mgr->SetActive(ACTIVE_OBJECT::DECOR,list.isActiveDecor);
            }

            ImGui::SameLine();


            if (ImGui::Checkbox(" Obstacle ", &list.isActiveObstacle))
            {
                Mgr->SetActive(ACTIVE_OBJECT::OBSTACLE, list.isActiveObstacle);
            }

            ImGui::SameLine();


            if (ImGui::Checkbox(" Event Object ", &list.isActiveEventObject))
            {
                Mgr->SetActive(ACTIVE_OBJECT::EVENTOBJECT, list.isActiveEventObject);
            }

            ImGui::SameLine();


            if (ImGui::Checkbox(" Item ", &list.isActiveItem))
            {
                Mgr->SetActive(ACTIVE_OBJECT::ITEM, list.isActiveItem);
            }

            ImGui::SameLine();


            if (ImGui::Checkbox(" Monster ", &list.isActiveMonster))
            {
                Mgr->SetActive(ACTIVE_OBJECT::MONSTER, list.isActiveMonster);
            }


            ImGui::TreePop();
        }

    }

}

void CImGui::Render_Mode()
{
    if (ImGui::CollapsingHeader("Setting Render Type",0) )
    {
        if (ImGui::Button(" All Render ", ImVec2(100, 30)))
        {
            CToolManager::GetInstance()->SetAllRender();
        }

        ImGui::SameLine();

        if (ImGui::Button(" Floor Render ", ImVec2(100, 30)))
        { 
            CToolManager::GetInstance()->SetFloorRender();
        }
    }
}

//층관련 출력함수
void CImGui::Settting_Floor_Render()
{
    ImGui::TextColored(ImVec4(255, 255, 255, 255), " << Select Floor >>");


    ImGui::Text(" Current Floor => ");

    ImGui::SameLine();

    switch (m_eCurFloor)
    {
    case Engine::FLOOR_B3:
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.6f, 1.0f), " B3 ");
        break;
    case Engine::FLOOR_B2:
        ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.8f, 1.0f), " B2 ");
        break;
    case Engine::FLOOR_B1:
        ImGui::TextColored(ImVec4(0.5f, 0.7f, 0.9f, 1.0f), " B1 ");
        break;
    case Engine::FLOOR_GROUND:
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), " Ground (0Floor) ");
        break;
    case Engine::FLOOR_1F:
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), " 1F ");
        break;
    case Engine::FLOOR_2F:
        ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.3f, 1.0f), " 2F ");
        break;
    case Engine::FLOOR_3F:
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.2f, 1.0f), " 3F ");
        break;
    case Engine::FLOOR_END:
        break;
    default:
        break;
    }


    if (ImGui::Button("Down Button"))
    {
        CToolManager::GetInstance()->DownFloor();
    }

    ImGui::SameLine();

    if (ImGui::Button("Up Floor"))
    {
        CToolManager::GetInstance()->UpFloor();
    }

}

void CImGui::ObjectInfo_Render()
{
    ImGui::TextColored(ImVec4({255,0,255,255})," << INFO >> ");

    CGameObject* TargetObject = nullptr;
    m_pFixTarget = nullptr;

    //수정할 타겟 Obj가Tile
    if (CTileManager::GetInstance()->GetTargetObject() != nullptr)
    {
        TargetObject = CTileManager::GetInstance()->GetTargetObject();

        m_pFixTarget = TargetObject;

        TileInfo_Render((CMapToolTile*)TargetObject);

        if( ImGui::Button("FIX INFO"))
        {
            if (static_cast<CMapToolTile*>(m_pFixTarget)->Get_TileType() == TILETYPE::TILE_NONE)
                return;

            m_ShowFixInfoWindow = true;
        }
    }

    //수정할 타켓 Obj가 Cube
    else if (CCubeManager::GetInstance()->GetTargetObject() != nullptr)
    {
        TargetObject = CCubeManager::GetInstance()->GetTargetObject();
        m_pFixTarget = TargetObject;

        CubeInfo_Render((CMapToolCube*)TargetObject);
        if (ImGui::Button("FIX INFO"))
        {
            if (m_ShowFixInfoWindow == true)
            {
                m_ShowFixInfoWindow = false;
            }
            else
            {
                m_ShowFixInfoWindow = true;
            }
        }
    }
    else if (CMapObjectManager::GetInstance()->GetInfoMapObject() != nullptr)
    {
        TargetObject = CMapObjectManager::GetInstance()->GetInfoMapObject();
        m_pFixTarget = TargetObject;

        MapObjectInfo_Render((CMapToolMapObject*)m_pFixTarget);

        if (ImGui::Button("FIX INFO"))
        {
            if (m_ShowFixInfoWindow == true)
            {
                m_ShowFixInfoWindow = false;
            }
            else
            {
                m_ShowFixInfoWindow = true;
            }
        }
    }
    else 
    {

        ImGui::Text("....None");
        m_pFixTarget = nullptr;
        return;

    }
}

void CImGui::TileInfo_Render(CMapToolTile* pTile)
{
    DefaultInfo_Render(pTile);

    //타일 타입 렌더용
    const char* TileTypeStr = Get_TileTypeStr(pTile->Get_TileType());
    ImGui::SameLine(); ImGui::Text("  TILE TYPE => "); ImGui::SameLine(); ImGui::TextColored(ImVec4({ 255,0,0,255 }), "[ %s ]", TileTypeStr);

    //위에 천장이이 있냐 없냐
    _bool   IsHaveCeiling = pTile->IsHaveCeiling();
    const char* ceilingStr = [&]() {
        return IsHaveCeiling ? "TRUE" : "FALSE";
        }();
    ImGui::Text("Is Have Ceiling => "); ImGui::SameLine();
    ImGui::TextColored(ImVec4({ 1.f,0.f,0.f,1.f }), " [ %s ] " , ceilingStr);
    
    //위에 큐브가 있냐 없냐
    _bool   IsCubeOnTop = pTile->IsCubeOnTop();
    const char* OnCubeStr = [&]() {
        return IsCubeOnTop ? "TRUE" : "FALSE";
        }();
    ImGui::Text("Is Cube On Top => "); ImGui::SameLine();
    ImGui::TextColored(ImVec4({ 1.f,0.f,0.f,1.f }), " [ %s ] ", OnCubeStr);


    _vec2 CenterUV = pTile->Get_CenterUV();
  

    ImTextureID TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Tile")->GetIDirect3DBaseTexture9();

    ImGui::Text( "Center UV POS => ");
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 255 / 255.f ,165.f / 255.f , 0 / 255.f, 255 / 255.f }), "U : %f ", CenterUV.x);
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 250.f / 255.f, 237.f / 255.f , 39.f / 255.f , 1.f }), "V : %f ", CenterUV.y);
    
    ImGui::SameLine();
    ImGui::Image(TexID, ImVec2({ 32.f , 32.f }) , ImVec2({ CenterUV.x - TILE_UV_CENTER , CenterUV.y - TILE_UV_CENTER }),
        ImVec2({ CenterUV.x+ TILE_UV_CENTER , CenterUV.y + TILE_UV_CENTER }));

    ImGui::SameLine();

    if(ImGui::Button("Get Tex", ImVec2(100, 30)))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(CenterUV);
    }

}

void CImGui::CubeInfo_Render(CMapToolCube* pCube)
{
    if (pCube == nullptr)
        return;

    DefaultInfo_Render(pCube);


    ImTextureID TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Tile")->GetIDirect3DBaseTexture9();

    _vec2* CenterUV = pCube->GetCenterUV();

    ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " - Z => "); ImGui::SameLine();
    if (ImGui::ImageButton("-Z", TexID, ImVec2(32.f, 32.f),
        ImVec2(CenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER, CenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER),
        ImVec2(CenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER, CenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER)))
    {
        _vec2 vCenterUV = CCubeManager::GetInstance()->GetCurCenterUV();
        pCube->Change_CubeTextureFaceUV(CUBE_FACE_NEGATIVE_Z, vCenterUV);
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Copy###NAVZ", ImVec2(50, 30)))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(CenterUV[CUBE_FACE_NEGATIVE_Z]);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Hor###Hor_CUBE_FACE_NEGATIVE_Z"), ImVec2(40, 30)))
    {
        pCube->Filp_Horizontal(CUBE_FACE_NEGATIVE_Z);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Ver###Ver_CUBE_FACE_NEGATIVE_Z"), ImVec2(40, 30)))
    {
        pCube->Filp_Vertically(CUBE_FACE_NEGATIVE_Z);
    }


    // +Z
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), " + Z => "); ImGui::SameLine();
    if (ImGui::ImageButton("+Z", TexID, ImVec2(32.f, 32.f),
        ImVec2(CenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER, CenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER),
        ImVec2(CenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER, CenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER)))
    {
        _vec2 vCenterUV = CCubeManager::GetInstance()->GetCurCenterUV();
        pCube->Change_CubeTextureFaceUV(CUBE_FACE_POSITIVE_Z, vCenterUV);
    }


    ImGui::SameLine();
    if (ImGui::Button("Copy###POSZ", ImVec2(50, 30)))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(CenterUV[CUBE_FACE_POSITIVE_Z]);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Hor###Hor_CUBE_FACE_POSITIVE_Z"), ImVec2(40, 30)))
    {
        pCube->Filp_Horizontal(CUBE_FACE_POSITIVE_Z);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Ver###Ver_CUBE_FACE_POSITIVE_Z"), ImVec2(40, 30)))
    {
        pCube->Filp_Vertically(CUBE_FACE_POSITIVE_Z);
    }



    // -X
    ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), " - X => "); ImGui::SameLine();
    if (ImGui::ImageButton("-X", TexID, ImVec2(32.f, 32.f),
        ImVec2(CenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER, CenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER),
        ImVec2(CenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER, CenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER)))
    {
        _vec2 vCenterUV = CCubeManager::GetInstance()->GetCurCenterUV();
        pCube->Change_CubeTextureFaceUV(CUBE_FACE_NEGATIVE_X, vCenterUV);
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy###NAVX", ImVec2(50, 30)))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(CenterUV[CUBE_FACE_NEGATIVE_X]);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Hor###Hor_CUBE_FACE_NEGATIVE_X"), ImVec2(40, 30)))
    {
        pCube->Filp_Horizontal(CUBE_FACE_NEGATIVE_X);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Ver###Ver_CUBE_FACE_NEGATIVE_X"), ImVec2(40, 30)))
    {
        pCube->Filp_Vertically(CUBE_FACE_NEGATIVE_X);
    }


    // +X
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), " + X => "); ImGui::SameLine();
    if (ImGui::ImageButton("+X", TexID, ImVec2(32.f, 32.f),
        ImVec2(CenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER, CenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER),
        ImVec2(CenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER, CenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER)))
    {
        _vec2 vCenterUV = CCubeManager::GetInstance()->GetCurCenterUV();
        pCube->Change_CubeTextureFaceUV(CUBE_FACE_POSITIVE_X, vCenterUV);
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy###POSX", ImVec2(50, 30)))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(CenterUV[CUBE_FACE_POSITIVE_X]);
    }
    ImGui::SameLine();

    if (ImGui::Button(("Hor###Hor_CUBE_FACE_POSITIVE_X"), ImVec2(40, 30)))
    {
        pCube->Filp_Horizontal(CUBE_FACE_POSITIVE_X);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Ver###Ver_CUBE_FACE_POSITIVE_X"), ImVec2(40, 30)))
    {
        pCube->Filp_Vertically(CUBE_FACE_POSITIVE_X);
    }



    // +Y
    ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), " + Y => "); ImGui::SameLine();
    if (ImGui::ImageButton("+Y", TexID, ImVec2(32.f, 32.f),
        ImVec2(CenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER, CenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER),
        ImVec2(CenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER, CenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER)))
    {
        _vec2 vCenterUV = CCubeManager::GetInstance()->GetCurCenterUV();
        pCube->Change_CubeTextureFaceUV(CUBE_FACE_POSITIVE_Y, vCenterUV);
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy###POSY", ImVec2(50, 30)))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(CenterUV[CUBE_FACE_POSITIVE_Y]);
    }
    ImGui::SameLine();

    if (ImGui::Button(("Hor###Hor_CUBE_FACE_POSITIVE_Y"), ImVec2(40, 30)))
    {
        pCube->Filp_Horizontal(CUBE_FACE_POSITIVE_Y);
    }

    ImGui::SameLine();

    if (ImGui::Button(("Ver###Ver_CUBE_FACE_POSITIVE_Y"), ImVec2(40, 30)))
    {
        pCube->Filp_Vertically(CUBE_FACE_POSITIVE_Y);
    }


    if (ImGui::Button("Change Transparency", ImVec2(250, 30)))
    {
        pCube->SetCubeTransparent();
    }

    ImGui::SameLine();

    if (ImGui::Button("Change Default", ImVec2(250, 30)))
    {
        pCube->SetCubeDefault();
    }


}

void CImGui::MapObjectInfo_Render(CMapToolMapObject* pMapObject)
{
    DefaultInfo_Render(pMapObject);

    if( pMapObject->Get_OBJTYPE() == OBJ_MAPEVENT)
        ImGui::Text("Event Path => %s ",CEnumStringFunction::EVENT_PATH_EnumToString(pMapObject->Get_EventPath()).c_str());
    if (pMapObject->Get_OBJTYPE() == OBJ_ITEM)
        ImGui::Text("Item Type => %s", CEnumStringFunction::ITEMTYPE_Enum_To_String(static_cast<CMapToolItem*>(pMapObject)->Get_ItemType()).c_str());
    if (pMapObject->Get_OBJTYPE() == OBJ_MONSTER)
        ImGui::Text("Monster Type => %s", CEnumStringFunction::MONSTER_TYPE_Enum_To_String(static_cast<CMapToolMonster*>(pMapObject)->Get_MonsterType()).c_str());
    if (pMapObject->Get_OBJTYPE() == OBJ_DOOR)
        ImGui::Text("Door Type => %s", CEnumStringFunction::DOORTYPE_Enum_To_String(static_cast<CMapToolDoor*>(pMapObject)->Get_DoorType()).c_str());
    if (pMapObject->Get_OBJTYPE() == OBJ_LIGHT)
        ImGui::Text("Door Type => %s", CEnumStringFunction::LIGHT_OBJECT_TYPE_Enum_To_String(static_cast<CMapToolLight*>(pMapObject)->Get_LightType()).c_str());
}

void CImGui::DefaultInfo_Render(CGameObject* pObj)
{
    //타입 렌더
    OBJTYPE Type = pObj->Get_OBJTYPE();
    ImGui::Text(" OBJ TYPE => "); ImGui::SameLine(); ImGui::TextColored(ImVec4({ 255,0,0,255 }), "[  %s  ]",
        CEnumStringFunction::OBJTYPE_EnumToString(Type).c_str());


    ObjectHandle tHandle = pObj->Get_Handle();


    //층타입 렌더
    ImGui::SameLine();
    const char* FloorStr = Get_FloorStr(pObj->GetFloor());
    ImGui::SameLine(); ImGui::Text("  FLOOR TYPE => "); ImGui::SameLine(); ImGui::TextColored(ImVec4({ 255,0,0,255 }), "[ %s ]", FloorStr);

    //인덱스 번호 렌더
    Int_Render("Handle Index => ", tHandle.m_iIndex);
    Int_Render("Event  Index => ", tHandle.m_sEventIndex);
    Int_Render("Room   Number=> ", pObj->GetRoomNum());
    if (pObj->Get_OBJTYPE() == OBJTYPE::OBJ_TILE)
        this->Int_Render("Tile Make Index => ", static_cast<CMapToolTile*>(pObj)->Get_MakeIndex());

    //위치 좌표 렌더
    _vec3 vPos = *pObj->Get_Transform()->GetInfoPos();
    Position_Render(vPos);

    //회전 각도 렌더
    ImGui::SameLine();
    _vec3 vRot = pObj->Get_Transform()->Get_Angle();
    Rotation_Render(vRot);

}


#pragma region 기능 렌더 관련
// 빌드 모드 관련 출력함수
void CImGui::Setting_BuildMode_Render()
{
    ImGui::TextColored(ImVec4(255, 255, 255, 255), " << Build Type >>");

    int BuildType = CToolManager::GetInstance()->Get_BuildType();

    const char* BuildTypeStr = "Unknown";

    switch (BuildType)
    {
    case Engine::BUILD_NONE:
        BuildTypeStr = "None";
        break;
    case Engine::BUILD_CREATE:
        BuildTypeStr =  "Create";
        break;
    case Engine::BUILD_TEXTURE:
        BuildTypeStr = "Texture";
        break;
    case Engine::BUILD_DELETE:
        BuildTypeStr = "Delete";
        break;
    case Engine::BUILD_INFO:
        BuildTypeStr = "Info";
        break;
    case Engine::BUILD_END:
        BuildTypeStr = "Unknown";
        break;
    default:
        break;
    }

    ImGui::TextColored(ImVec4({ 1.f,0.f,0.f,1.f }), "Current Build Type: %s", BuildTypeStr);
  

    ImGui::PushID("BuildTypeRadio");
    ImGui::RadioButton(" CREATE##BuildTypeRadio ", &BuildType, 1);
    ImGui::SameLine();
    ImGui::RadioButton(" TEXTURE##BuildTypeRadio ", &BuildType, 2);
    ImGui::SameLine();
    ImGui::RadioButton(" DELETE##BuildTypeRadio ", &BuildType, 3);
    ImGui::SameLine();
    ImGui::RadioButton(" INFO##BuildTypeRadio", &BuildType, 4);
    ImGui::PopID();


    CToolManager::GetInstance()->Set_BuildType((MAPTOOL_BUILDTYPE)BuildType);

    if (BuildType == BUILD_CREATE || BuildType == BUILD_TEXTURE)
        m_IsShapeTypeRender = true;
    else
        m_IsShapeTypeRender = false;


}

// 맵 도형 선택 관련 출력함수
void CImGui::Setting_ShapeType_Render()
{
    ImGui::TextColored(ImVec4(255, 255, 255, 255), " << Shape Type >>");

    _int ShapeType = CToolManager::GetInstance()->Get_ShapeType();

    const char* ShapeTypeStr = Get_ShapeTypeStr((MAPTOOL_SHAPETYPE)ShapeType);

    ImGui::TextColored(ImVec4({ 1.f,0.f,0.f,1.f }), "Current Shape Type: %s", ShapeTypeStr);

    ImGui::PushID("ShapeTypeRadio");
    ImGui::RadioButton(" TILE ", &ShapeType, 1);
    ImGui::SameLine();
    ImGui::RadioButton(" CUBE ", &ShapeType, 2);
    ImGui::PopID();


    CToolManager::GetInstance()->Set_ShapeType((MAPTOOL_SHAPETYPE)ShapeType);

    if (ShapeType == MAPTOOL_SHAPETYPE::SHAPE_TILE)
        m_IsTileTypeRender = true;
    else
        m_IsTileTypeRender = false;

}

//타일 모드일 경우 타일의 종류 선택 출력함수
void CImGui::Setting_TileType_Render()
{
    ImGui::TextColored(ImVec4(255, 255, 255, 255), " << Tile Type >>");

    int TileType = CToolManager::GetInstance()->Get_TileType();

    const char* tileTypeStr = "Unknown";

    switch (TileType)
    {
    case TILETYPE::TILE_NONE:
        tileTypeStr = "None";
        break;
    case TILETYPE::TILE_FLOOR:
        tileTypeStr = "Floor";
        break;
    case TILETYPE::TILE_CEILING:
        tileTypeStr = "Ceiling";
        break;
    default:
        tileTypeStr = "Unknown";
        break;
    }

    ImGui::TextColored(ImVec4({ 1.f,0.f,0.f,1.f }), "Current Tile Type: %s", tileTypeStr);
    ImGui::SameLine();

    ImGui::PushID("TileTypeRadio");
    ImGui::RadioButton("  FLOOR##TileTypeRadio ", &TileType, 1);
    ImGui::SameLine();
    ImGui::RadioButton(" CELLING##TileTypeRadio ", &TileType, 2);
    ImGui::PopID();

    CToolManager::GetInstance()->Set_TileType((TILETYPE)TileType);

}

void CImGui::Setting_RoomNum_Render()
{
    _int RoomNum = CToolManager::GetInstance()->GetSettingRoomNum();

    ImGui::Text("Current Setting RoomNum ", RoomNum);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4({255,0,255,255}),"[ %d ]", RoomNum);

    if (ImGui::InputInt(" Input Room Num ",& RoomNum))
    {
        CToolManager::GetInstance()->Set_SettingRoomNum(RoomNum);
    }

}

// 텍스처 관련 출력 함수
void CImGui::Setting_Texture_Render()
{
    ImGui::TextColored(ImVec4(255, 255, 255, 255), " << Select Texture >>");

    ImGui::Text(" Open Texture  ===> ");
    ImGui::SameLine();
    if (ImGui::Button(" TEXTURE "))
    {
        if (m_ShowTextureWindow == true)
            m_ShowTextureWindow = false;
        else
            m_ShowTextureWindow = true;
    }

    _vec2 CenterUV = CToolManager::GetInstance()->GetTextureMode_CenterUV();

    ImGui::SameLine();

    ImGui::Text("Current UV Center => { X : %f  Y : %f } ", CenterUV.x, CenterUV.y);


    ImGui::Text(" Current Texture -> ");

    ImGui::SameLine();

    ImTextureID TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Tile")->GetIDirect3DBaseTexture9();


    ImVec2 LT = { CenterUV.x - TILE_UV_CENTER , CenterUV.y - TILE_UV_CENTER };
    ImVec2 RB = { CenterUV.x + TILE_UV_CENTER , CenterUV.y + TILE_UV_CENTER };

    ImGui::Image(TexID, ImVec2({ 64 , 64 }),
        LT,
        RB,// 밑에 이거 알파값 벡터 
        ImVec4({ 1.f,1.f,1.f,1.f }), ImVec4(1.f, 1.f, 1.f, 1.f));


    if (ImGui::Button("Set Texture Transparency"))
    {
        CToolManager::GetInstance()->SetTextureMode_CenterUV(_vec2(TRANSPARENT_TILE_UV_X,TRANSPARENT_TILE_UV_Y));
    }

    if (ImGui::Button("Set Default TILE UV"))
    {
        CToolManager::GetInstance()->Set_DefaultTileUV(CTileManager::GetInstance()->GetCenterUV());
    }

    if (ImGui::Button("Set Default CUBE UV"))
    {
        CToolManager::GetInstance()->Set_DefaultCubeUV(CTileManager::GetInstance()->GetCenterUV());
    }

}
//타일 Texture 모두 렌더해주는 함수
void CImGui::TileTex_Render()
{

    ImTextureID TexID = (ImTextureID)CResourceManager::GetInstance()->Get<CTexture>(L"Tile")->GetIDirect3DBaseTexture9();

    int count = 0;

    for (int i = 0; i < TILE_CNTY; ++i)
    {
        for (int j = 0; j < TILE_CNTX; ++j)
        {
            ++count;

            char FileName[128] = "";
            
            sprintf_s(FileName,"Tile_%d_%d",i,j);

            // 이미지에 text / 출력할 사진의 다렉 객체 넘겨주기 / 출력할 사이즈 / LT / RB /
            if (ImGui::ImageButton(FileName, TexID, ImVec2({ 64 , 64 }), ImVec2({ j * TILE_UV_SIZE , i * TILE_UV_SIZE }),
                ImVec2((j + 1) * TILE_UV_SIZE, (i + 1) * TILE_UV_SIZE)))
            {
                _vec2 uvLT = _vec2(j * TILE_UV_SIZE, i * TILE_UV_SIZE);
                _vec2 uvRB = _vec2((j + 1) * TILE_UV_SIZE, (i + 1) * TILE_UV_SIZE);
                _vec2 uvCenter = { (uvLT.x + uvRB.x) * 0.5f , (uvLT.y + uvRB.y) * 0.5f };

                CToolManager::GetInstance()->SetTextureMode_CenterUV(uvCenter);
            }

            if (count == 5)
            {
                ImGui::NewLine();
                count = 0;
            }
            else
                ImGui::SameLine();

        }
    }
}
void CImGui::Render_Type(const char* Title, const char* Value)
{
    string str = Title;
    string end = " => ";

    string Result = str + end;

    ImGui::Text(Result.c_str());
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 1.f , 0.f , 0.f , 1.f }), "[ %s ]", Value);

}
void CImGui::Position_Render(const _vec3& vPos)
{
    ImGui::Text(" POSITION => ");
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 255 / 255.f ,165.f / 255.f , 0 / 255.f, 255 / 255.f }), "X : %f ", vPos.x);
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 250.f / 255.f, 237.f / 255.f , 39.f / 255.f , 1.f }), "Y : %f ", vPos.y);
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 9.f / 255.f, 255.f / 255.f ,162.f / 255.f, 1.f }), "Z : %f ", vPos.z);
}

void CImGui::Rotation_Render(const _vec3& vRot)
{
    ImGui::Text(" ROTATION => ");
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 255 / 255.f ,165.f / 255.f , 0 / 255.f, 255 / 255.f }), "X : %.2f ", vRot.x);
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 250.f / 255.f, 237.f / 255.f , 39.f / 255.f , 1.f }), "Y : %.2f ", vRot.y);
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ 9.f / 255.f, 255.f / 255.f ,162.f / 255.f, 1.f }), "Z : %.2f ", vRot.z);
}


void CImGui::Int_Render(string str, const _int& Num, _vec4 vColor)
{
    ImGui::Text(str.c_str());
    ImGui::SameLine(); ImGui::TextColored(ImVec4({ vColor.x, vColor.y , vColor.z , vColor.w }), "[ %d ]", Num);
}

#pragma endregion

#pragma region Str 바꿔주는 함수 모음
const char* CImGui::Get_FloorStr(const FLOOR& eFloor)
{
    switch (eFloor)
    {
    case Engine::FLOOR_B3:
        return "FLOOR B3";
    case Engine::FLOOR_B2:
        return "FLOOR B2";
    case Engine::FLOOR_B1:
        return "FLOOR B1";
    case Engine::FLOOR_GROUND:
        return "FLOOR GROUND";
    case Engine::FLOOR_1F:
        return "FLOOR 1F";
    case Engine::FLOOR_2F:
        return "FLOOR 2F";
    case Engine::FLOOR_3F:
        return "FLOOR 3F";
    default:
        return "NONE";
        break;
    }
}

const char* CImGui::Get_TileTypeStr(const TILETYPE& eType)
{
    switch (eType)
    {
    case Engine::TILE_FLOOR:
        return "FLOOR";
        break;
    case Engine::TILE_CEILING:
        return "CEILING";
        break;
    default:
        return "NONE";
        break;
    }
}

const char* CImGui::Get_ShapeTypeStr(const MAPTOOL_SHAPETYPE& eType)
{
    switch (eType)
    {
    case Engine::SHAPE_TILE:
     return "TILE";
        break;
    case Engine::SHAPE_CUBE:
        return "CUBE";
        break;
    default:
        return "NONE....";
        break;
    }
}

const char* CImGui::Get_CubeTypeStr(const CUBE_TYPE& eType)
{
    switch (eType)
    {
    case Engine::CUBE_NONE:
        return "NONE";
        break;
    case Engine::CUBE_EVENT:
        return "EVENT_CUBE";
        break;
    case Engine::CUBE_MAPTOOL:
        return "MAPTOOL_CUBE";
        break;
    case Engine::CUBE_CREATURECANTGO:
        return "CUBE_CREATURECANTGO";
    default:
        return "NONE";
        break;
    }
}

const char* CImGui::Get_CategoriStr(const MAPOBJECT_CATEGORY& eType)
{
    switch (eType)
    {
    case Engine::MAPOBJECT_CATEGORY::DECOR:
        return "Decor";
        break;
    case Engine::MAPOBJECT_CATEGORY::OBSTACLE:
        return "Obstacle";
        break;
    case Engine::MAPOBJECT_CATEGORY::EVENT:
        return "Event";
        break;
    default:
        return "NONE";
        break;
    }
}

const char* CImGui::Get_PlacementStr(const MAPOBJECT_PLACEMENT& eType)
{
    switch (eType)
    {
    case Engine::MAPOBJECT_PLACEMENT::GROUND:
        return "Ground";
        break;
    case Engine::MAPOBJECT_PLACEMENT::WALL:
        return "Wall";
        break;
    default:
        return "NONE";
        break;
    }
}

void CImGui::Render_Atlas(CTexture* pTexture, const string& pPathJson)
{
    ifstream is(pPathJson);
    nlohmann::ordered_json j(pPathJson);

    if (is.is_open() == false)
        return;

    ImTextureID Image = (ImTextureID)(pTexture->GetIDirect3DBaseTexture9());

    is >> j;

    int Width = j["meta"]["size"]["w"];
    int Height = j["meta"]["size"]["h"];

    nlohmann::ordered_json Objects = j.value("frames", nlohmann::json::array());

    int count = 0;

    for(auto& Object : Objects)
    {
        count++;

        float x = Object["frame"]["x"];
        float y = Object["frame"]["y"];
        float w = Object["frame"]["w"];
        float h = Object["frame"]["h"];

        ImVec2 UV_LT_ = { x / (float)Width , y / (float)Height};
        ImVec2 UV_RB_ = { (x + w) / (float)Width  , (y + h) / (float)Height };

        string Name = Object["filename"];

        if( ImGui::ImageButton(Name.c_str(), Image, ImVec2(32, 32), UV_LT_, UV_RB_) )
        {

            string Categori = j["meta"]["MAPOBJECT_CATEGORY"];
            string Placement = j["meta"]["MAPOBJECT_PLACEMENT"];


            auto Mgr = CMapObjectManager::GetInstance();

            auto Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Categori);


            if (Cat == MAPOBJECT_CATEGORY::EVENT)
            {
                auto Enum = CEnumStringFunction::EVENT_PATH_StringToEnum(Object["filename"].get<string>());
                CMapObjectManager::GetInstance()->SetCurEventPath(Enum);
            }

            if (Cat == MAPOBJECT_CATEGORY::MONSTER)
            {
                auto Enum = CEnumStringFunction::MONSTER_TYPE_String_To_Enum(Object["filename"].get<string>());
                CMapObjectManager::GetInstance()->SetCurMonsterType(Enum);
            }


            if (Cat == MAPOBJECT_CATEGORY::DOOR)
            {
                auto Enum = CEnumStringFunction::DOORTTYPE_Stirng_To_Enum(Object["filename"].get<string>());
                CMapObjectManager::GetInstance()->SetDoorType(Enum);
            }


            if (Cat == MAPOBJECT_CATEGORY::LIGHT)
            {
                auto Enum = CEnumStringFunction::LIGHT_OBJECT_TYPE_String_To_Enum(Object["filename"].get<string>());
                CMapObjectManager::GetInstance()->SetLightTYpe(Enum);
            }


            if (Object.contains("PLACEMENT"))
                Mgr->SetCurPlacement(CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Object["PLACEMENT"].get<string>()));
            else
                Mgr->SetCurPlacement(CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Placement));

            Mgr->SetCurCategory(CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Categori));

            SpriteRect newSprite;

            newSprite.x = x;
            newSprite.y = y;
            newSprite.w = w;
            newSprite.h = h;
            newSprite.Mw = Width;
            newSprite.Mh = Height;

            Mgr->SetCurSpriteRect(newSprite);

        }

        if (count == 5)
        {
            ImGui::NewLine();
            count = 0;
        }
        else
            ImGui::SameLine();


    }
}

void CImGui::Render_Atlas_Item(CTexture* pTexture, const string& pPathJson, string strType)
{
    ifstream is(pPathJson);
    nlohmann::ordered_json j;

    if (is.is_open() == false)
        return;

    ImTextureID Image = (ImTextureID)(pTexture->GetIDirect3DBaseTexture9());

    is >> j;


    if (!j.contains("frames"))
    {
        int a = 0;
        return;
    }

    if (!j["frames"].contains(strType))
    {
        int a = 0;
        return;
    }
    
    MAPOBJECT_CATEGORY Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(j["meta"]["MAPOBJECT_CATEGORY"].get<string>());
    MAPOBJECT_PLACEMENT PlaceMent = CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(j["meta"]["MAPOBJECT_PLACEMENT"].get<string>());

    nlohmann::ordered_json Items = j["frames"].value(strType, nlohmann::json::array());

    int count = 0;

    int Width = j["meta"]["size"]["w"];
    int Height = j["meta"]["size"]["h"];


    for (auto& Item : Items)
    {
        count++;

        float x = Item["frame"]["x"];
        float y = Item["frame"]["y"];
        float w = Item["frame"]["w"];
        float h = Item["frame"]["h"];

        ImVec2 UV_LT_ = { x / (float)Width , y / (float)Height };
        ImVec2 UV_RB_ = { (x + w) / (float)Width  , (y + h) / (float)Height };

        string Name = Item["filename"];
        ITEMTYPE ItemType = CEnumStringFunction::ITEMTYPE_String_To_Enum(Name);

        if (ImGui::ImageButton(Name.c_str(), Image, ImVec2(32, 32), UV_LT_, UV_RB_))
        {

            string Categori = j["meta"]["MAPOBJECT_CATEGORY"];
            string Placement = j["meta"]["MAPOBJECT_PLACEMENT"];


            auto Mgr = CMapObjectManager::GetInstance();
            auto Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Categori);

            Mgr->SetCurCategory(CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Categori));
            Mgr->SetCurPlacement(CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Placement));
            Mgr->SetCurItemType(ItemType);

            SpriteRect newSprite;

            newSprite.x = x;
            newSprite.y = y;
            newSprite.w = w;
            newSprite.h = h;
            newSprite.Mw = Width;
            newSprite.Mh = Height;

            Mgr->SetCurSpriteRect(newSprite);

        }

        if (count == 5)
        {
            ImGui::NewLine();
            count = 0;
        }
        else
            ImGui::SameLine();
    }
}
#pragma endregion

#pragma region 데이터 세이브 로드
void CImGui::Save_MapData(const string& sFileName)
{
    nlohmann::ordered_json j;

    CTileManager::GetInstance()->SaveTileToJSON(j, sFileName);

    CCubeManager::GetInstance()->SaveCubeToJSON(j, sFileName);

    CMapObjectManager::GetInstance()->SaveMapObjectToJson(j, sFileName);

    std::ofstream outputFile(sFileName); // 출력 파일 스트림 객체 생성

    if (!outputFile.is_open())
        return;

    outputFile << j.dump(4);
    outputFile.close();
}
void CImGui::Load_MapData()
{
}
vector<string> CImGui::Lead_Folder(const string& FloderPath)
{
    namespace fs = std::filesystem;  // 파일시스템 쓰려고 임시
    vector<string> vFile;           // 모든 파일명을 담을 vector 
    vFile.clear();                  // 그냥 초기화

    fs::path Dir = FloderPath;     // 파일시스템 패스 변수에 폴더 경로 저장
    if (!fs::exists(Dir) || !fs::is_directory(Dir))  // 경로가 존재하지 않거나 디렉터리가 아니라면 빈깡통반환
        return vFile;                           

    //directory_iterator -> 해당 디렉토리의 "직접 자식"만 순회..
    for (const auto& entry : fs::directory_iterator(Dir))
    {
        //일반 파일만 찾는다 폴더더 나 다른 특별한 파일은 일단 무시
        if (!entry.is_regular_file())
            continue;

        // path() 함수가 경로를 반환해주는 함수

        //std::filesystem::path p1 = L"C:\\data\\images\\tile_0_0.png";
        //p1.filename()    // "tile_0_0.png"
        //    p1.stem()        // "tile_0_0"
        //    p1.extension()   // ".png"
        //    p1.parent_path() // "C:\\data\\images"

        //    std::filesystem::path p2 = L"/usr/local/bin/exec";
        //p2.filename()    // "exec"
        //    p2.parent_path() // "/usr/local/bin"


        const fs::path p = entry.path().filename(); 
        //경로를 string 으로 치환해주는 함수조차 지원
        vFile.push_back(p.string());
    }

    return vFile;
}
#pragma endregion

void CImGui::Add_ImGuiImageUV(TexTileImageUV* pTexImageUV)
{
    m_vImageTileUV.push_back(pTexImageUV);
}

void CImGui::Free()
{
    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
