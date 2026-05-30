 
#include "pch.h"
#include "CImGuiDemo.h"
#include "CParticleMgr.h"


CImGuiDemo::CImGuiDemo()
    : m_pGraphicDevice(nullptr), 
    show_demo_window(true), 
    show_another_window(false) , 
    clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f))
{
}

CImGuiDemo::~CImGuiDemo()
{
}

void CImGuiDemo::Init(LPDIRECT3DDEVICE9 device)
{
    m_pGraphicDevice = device;
    m_pGraphicDevice->AddRef();

    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
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

_int CImGuiDemo::Update_ImGui(const float& fTimeDelta)
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //Test()
    this->Test();

    return 0;
}

void CImGuiDemo::LateUpdate_ImGui()
{
}

void CImGuiDemo::Render_ImGui()
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

void CImGuiDemo::Test()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //if (show_demo_window)
    //    ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float fBoundingBoxScale_X = 1.0f;
        static float fBoundingBoxScale_Y = 1.0f;
        static float fBoundingBoxScale_Z = 1.0f;

        static int iParticleType = 0;

        static int iParticleTexIndex_X = 0;
        static int iParticleTexIndex_Y = 0;

        static int iParticleNum = 1;
        static bool bRandomizeSize = false;

        static float fConstantPScale = 0.2f;

        static float fGoUpSpeed = 1.6f;

        static float fDropSpeed_XZ = 0.6f;
        static float fDropSpeed_Y = 7.f;
        static float fDropGravity = 9.8f;

       

        ImGui::Begin("Particle Settings         ");                          // Create a window called "Hello, world!" and append into it.

        // 1. 바운딩 박스 크기
        ImGui::Text("[Global::Bounding Box Scale]");         
        ImGui::SliderFloat("X", &fBoundingBoxScale_X, 0.5f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("Y", &fBoundingBoxScale_Y, 0.5f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("Z", &fBoundingBoxScale_Z, 0.5f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        _vec3 vBoundingBoxScale = { fBoundingBoxScale_X, fBoundingBoxScale_Y, fBoundingBoxScale_Z };
		CParticleMgr::GetInstance()->Set_BoundingBoxScale(vBoundingBoxScale);
        
        ImGui::Text("");

        // 3. 입자 텍스쳐 설정
        ImGui::Text("[Global::Particle Texture Index]");
        ImGui::Text("X = %d", iParticleTexIndex_X);
        ImGui::Text("Y = %d", iParticleTexIndex_Y);
        if (ImGui::Button("Next X Index"))
        { 
            iParticleTexIndex_X++;
            if (iParticleTexIndex_X == 16)
            {
                iParticleTexIndex_X = 0;
            }
            CParticleMgr::GetInstance()->Set_IsTextureIndexChanged(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Next Y Index"))
        {
            iParticleTexIndex_Y++;
            if (iParticleTexIndex_Y == 6)
            {
                iParticleTexIndex_Y = 0;
            }
            CParticleMgr::GetInstance()->Set_IsTextureIndexChanged(true);
        }
       
        CParticleMgr::GetInstance()->Set_ParticleTextureIndex(iParticleTexIndex_X, iParticleTexIndex_Y);

        ImGui::Text("");

        // 4. 입자 갯수
        ImGui::Text("[Global::Particle Num]");    
        ImGui::SliderInt("Num", &iParticleNum, 1, 50);    
        CParticleMgr::GetInstance()->Set_ParticleNum(iParticleNum);

        ImGui::Text("");

        // 6. 입자 크기 일정할때 사이즈
        ImGui::Text("[Global::P Size::Constant]");
        ImGui::SliderFloat("Size", &fConstantPScale, 0.2f, 0.4f);
        CParticleMgr::GetInstance()->Set_ParticleSize(fConstantPScale);

        ImGui::Text("");

		// 5. 입자 크기 일정 or 랜덤
        ImGui::Text("[Global::Randomize P Size]");
        ImGui::Checkbox(" ", &bRandomizeSize);
		CParticleMgr::GetInstance()->Set_IsParticleRandomSize(bRandomizeSize);

        ImGui::Text("");

        

        ImGui::Text("----------------------------------");

        // 2. 텍스쳐 타입 설정
        ImGui::Text("[Global::Particle Type]");
       
		if (iParticleType == CParticleMgr::PARTICLE_TYPE_TEST)
		{
			ImGui::Text("Curr Type: Test");
		}
		else if (iParticleType == CParticleMgr::PARTICLE_TYPE_SPARKLE)
		{
			ImGui::Text("Curr Type: Sparkle");
		}
		else if (iParticleType == CParticleMgr::PARTICLE_TYPE_GO_UP)
		{
			ImGui::Text("Curr Type: Go Up");
		}
		else if (iParticleType == CParticleMgr::PARTICLE_TYPE_DROP)
		{
			ImGui::Text("Curr Type: Drop");
		}
		else if (iParticleType == CParticleMgr::PARTICLE_TYPE_FLIES)
		{
			ImGui::Text("Curr Type: Flies");
		}

        ImGui::Text("");

        if (ImGui::Button("Change Type"))
        {
            iParticleType++;
            if (iParticleType == 4)
            {
                iParticleType = 0;
            }
            CParticleMgr::GetInstance()->Set_IsParticleTypeChanged(true);
            CParticleMgr::GetInstance()->Set_ParticleType(static_cast<CParticleMgr::PARTICLE_TYPE>(iParticleType));
        }

        if (iParticleType == CParticleMgr::PARTICLE_TYPE_TEST)
        {
        }
        else if (iParticleType == CParticleMgr::PARTICLE_TYPE_SPARKLE)
        {
        }
        else if (iParticleType == CParticleMgr::PARTICLE_TYPE_GO_UP)
        {
            ImGui::Text("----------------------------------");
            // 7. 고업 타입일 때 올라가는 속도
            ImGui::Text("[GoUp Type::Speed]");
            ImGui::SliderFloat("Speed", &fGoUpSpeed, 0.5f, 5.0f);
            CParticleMgr::GetInstance()->Set_Speed_GoUp(fGoUpSpeed);

            ImGui::Text("");
        }
        else if (iParticleType == CParticleMgr::PARTICLE_TYPE_DROP)
        {
            ImGui::Text("----------------------------------");
            // 8. 드롭 타입일 때 드롭 속도
            ImGui::Text("[Drop Type::Speed]");
            ImGui::SliderFloat("Speed XZ", &fDropSpeed_XZ, 0.3f, 4.0f);
            CParticleMgr::GetInstance()->Set_DropSpeed_XZ(fDropSpeed_XZ);

            ImGui::SliderFloat("Speed Y", &fDropSpeed_Y, 1.0f, 10.0f);
            CParticleMgr::GetInstance()->Set_DropSpeed_Y(fDropSpeed_Y);

            ImGui::Text("");

            ImGui::Text("[Drop Type::Gravity]");
            ImGui::SliderFloat("Gravity", &fDropGravity, 1.0f, 20.0f);
            CParticleMgr::GetInstance()->Set_DropGravity(fDropGravity);

            ImGui::Text("");

        }
        else if (iParticleType == CParticleMgr::PARTICLE_TYPE_FLIES)
        {
        }

      

        ImGui::Text("----------------------------------");
        ImGui::Text("[Apply Settings]");
        ImGui::Text("");

        if (ImGui::Button("Apply"))
        {
            CParticleMgr::GetInstance()->Set_ApplySettings(true);
        }
        ImGui::Text("----------------------------------");



        
        







        //ImGui::Checkbox("Demo Window", &show_demo_window);  

        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //if (ImGui::Button("Button"))
        //{ 
        //    counter++;
        //}
        //// Buttons return true when clicked (most widgets return true when edited/activated)
        //   
        //ImGui::SameLine();
        //ImGui::Text("counter = %d", counter);

        ImGui::End();
    }


     
}

void CImGuiDemo::Free()
{
    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
