﻿// MapTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "MapTool.h"
#include "Engine_Define.h"
#include "CFrameManager.h"
#include "CTimeManager.h"
#include "CInputManager.h"
#include "CMapTool.h"
#include "imgui_impl_win32.h"
#include "imgui.h"

#define MAX_LOADSTRING 100

static int      iCurFloor = 0;

// 전역 변수:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HWND                g_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAPTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPTOOL));




    if (FAILED(CTimeManager::GetInstance()->Ready_Timer(L"Timer_Immediate")))
        return E_FAIL;

    if (FAILED(CTimeManager::GetInstance()->Ready_Timer(L"Timer_60")))
        return E_FAIL;

    if (FAILED(CFrameManager::GetInstance()->Ready_Frame(L"Frame60", 60.f)))
        return E_FAIL;

    CInputManager::GetInstance()->Init(hInst, g_hWnd);

    CMapTool* Maptool = CMapTool::Create();

    if (Maptool == nullptr)
        return FALSE;


    MSG msg = {};


    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        //주석주석주석
        else
        {
            CTimeManager::GetInstance()->Set_TimeDelta(L"Timer_Immediate");
            _float fTimer_Immediate = CTimeManager::GetInstance()->Get_TimeDelta(L"Timer_Immediate");

            /*if (CFrameManager::GetInstance()->IsPermit_Call(L"Frame60", fTimer_Immediate))
            {*/
            CTimeManager::GetInstance()->Set_TimeDelta(L"Timer_60");
            _float fTimer_60 = CTimeManager::GetInstance()->Get_TimeDelta(L"Timer_60");

            Maptool->Update_Maptool(fTimer_60);
            Maptool->LateUpdate_Maptool(fTimer_60);
            Maptool->Render_Maptool();
            //}
        }
    }

    _ulong  dwRefCnt(0);

    if (dwRefCnt = Engine::Safe_Release(Maptool))
    {
        MSG_BOX("MainApp Release Failed");
        return FALSE;
    }


    return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPTOOL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
    RECT rc{ 0, 0, WINCX, WINCY };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        rc.right - rc.left,
        rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    g_hWnd = hWnd;

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(g_hWnd);
            break;
        }

        break;


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}