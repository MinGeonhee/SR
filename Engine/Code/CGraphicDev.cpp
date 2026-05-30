#include "CGraphicDev.h"

// 해당 그래픽 디바이스는 Direct3D 장치를 생성하고, 그 장치에서 그리기를 담당하는 클래스입니다.
// " 그래픽 카드와 직접 소통하는 장치" 라고 보면 된다.


// 주요 멤버 변수
IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev()
	: m_pSDK(nullptr), m_pGraphicDev(nullptr)
{

}

CGraphicDev::~CGraphicDev()
{
	Free();
}

HRESULT CGraphicDev::Ready_GraphicDev(HWND hWnd, WINMODE eMode,
	const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
		return E_FAIL;

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// D3DADAPTER_DEFAULT : 현재 컴퓨터에 설치된 기본 그래픽 카드
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	_ulong	dwFlag(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = iSizeX;
	d3dpp.BackBufferHeight = iSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD = 1,	// 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP = 2,		// 버퍼 하나를 뒤집으면서 사용
	// D3DSWAPEFFECT_COPY = 3,		// 더블 버퍼링과 흡사

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;

	d3dpp.Windowed = eMode;		// TRUE(창 모드)

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		dwFlag,
		&d3dpp,
		&m_pGraphicDev)))
	{
		return E_FAIL;
	}

	*ppGraphicDev = this;

	return S_OK;
}

// 후면 버퍼의 동작 원리

// 1. 화면을 지운다.
// 2. 다시 그릴 화면 준비
void CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		Color,
		1.f,		// z버퍼의 초기화값
		0);

	m_pGraphicDev->BeginScene();

}


// 3. 여기까지 한 프레임을 그릴 것이라고 장치에게 통보
// 4. 전면 버퍼와 서피스 교환
void CGraphicDev::Render_End()
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);

	// 1, 2 인자 : 렉트의 주소
	// 1, 2, 4 인자 : D3DSWAPEFFECT_COPY 옵션일 때만 사용 가능
	// 3인자 : 창 핸들
}

void CGraphicDev::Make_Triangle()
{

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 8개의 버텍스를 저장할 버텍스 버퍼를 만든다.
	IDirect3DVertexBuffer9* vb; // Vertexbuffer 선언
	m_pGraphicDev->CreateVertexBuffer(
		3 * sizeof(VTXCOL),
		0,
		D3DFVF_XYZ | D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED,
		&vb,
		0);

	// 36개의 16 - 비트 인덱스를 보관할 만큼의 동적 인덱스 버퍼를 만든다.
	IDirect3DIndexBuffer9* ib; // Indexbuffer 선언
	m_pGraphicDev->CreateIndexBuffer(
		9 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&ib,
		0);

	// GPU메모리는 CPU가 직접 [], ->, * 연산으로 건드릴 수가 없다.
	// 잠깐 GPU의 buffer를 건드릴 수 있게 해주는게 Lock이다.
	VTXCOL* vertexs;
	vb->Lock(0, 0, (void**)&vertexs, 0);

	vertexs[0] = VTXCOL{ {-1.f, -1.f,0.f}, D3DCOLOR_ARGB(255, 255, 0, 255) };
	vertexs[1] = VTXCOL{ {0.f, 1.f, 0.f}, D3DCOLOR_ARGB(255, 0, 255, 255) };
	vertexs[2] = VTXCOL{ {1.f, -1.f, 0.f}, D3DCOLOR_ARGB(255, 0, 0, 255) };
	vb->Unlock(); // 버퍼 이용이 끝나면 잠금을 해제한다.

	WORD* indices;
	ib->Lock(0, 0, (void**)&indices, 0);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	ib->Unlock();

	m_pGraphicDev->SetStreamSource(0, vb, 0, sizeof(VTXCOL));
	m_pGraphicDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pGraphicDev->SetIndices(ib);

	// 6. 삼각형 그리기
	m_pGraphicDev->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, // 삼각형 리스트로 그림
		0,                  // BaseVertexIndex
		0,                  // MinIndex
		3,                  // NumVertices
		0,                  // StartIndex
		1);                 // PrimitiveCount (삼각형 1개)

	
}


void CGraphicDev::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pSDK);
}
