#include "pch.h"
#include "CShape.h"

void CShape::Ready_DrawShape()
{
	// 3차원 공간에서는 default Vector가 z = 1;
	// 카메라가 바라보는 방향이 z의 양의 방향이라서.

	// 현재 내가 어떤 축 기준으로 Vertex를 찍었는지에 따라서 
	// 내가 Default Vector를 어디다 둬야하는지 정해진다.

	m_tInfo.m_vDefault = { 0, 1, 0};
	m_tInfo.m_vLook = { 0, 0, 0};
	m_tInfo.m_vPos = { 0.f, 0.f, 0 };
	m_tInfo.m_vDir = { 0, 0, 0};
	m_tInfo.m_vScale = { 1, 1, 1};
	m_vAngle = { 0,0,0};

	m_fSpeed = 3.f;
}

void CShape::Ready_DrawTriangle()
{

	/*
	typedef enum _D3DCULL {
    D3DCULL_NONE                = 1, // 후면 추려내기(컬링)을 하지 않겠다.
    D3DCULL_CW                  = 2,
    D3DCULL_CCW                 = 3,
    D3DCULL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum 
						} D3DCULL;
	*/

	m_pDeviceClass->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDeviceClass->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// SetRenderState

	// 8개의 버텍스를 저장할 버텍스 버퍼를 만든다.
 // Vertexbuffer 선언
	m_pDeviceClass->CreateVertexBuffer(
		3 * sizeof(VTXCOL),
		0,
		D3DFVF_XYZ | D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED,
		&m_vVertexBuffer,
		0);

	// 36개의 16 - 비트 인덱스를 보관할 만큼의 동적 인덱스 버퍼를 만든다.
// Indexbuffer 선언
	m_pDeviceClass->CreateIndexBuffer(
		9 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_vIndexBuffer,
		0);

	// GPU메모리는 CPU가 직접 [], ->, * 연산으로 건드릴 수가 없다.
	// 잠깐 GPU의 buffer를 건드릴 수 있게 해주는게 Lock이다.
	VTXCOL* vertexs;
	m_vVertexBuffer->Lock(0, 0, (void**)&vertexs, 0);

	vertexs[0] = VTXCOL{ {- 1.f, - 1.f, + 0.f}, D3DCOLOR_ARGB(255, 255, 0, 255) };
	vertexs[1] = VTXCOL{ {+ 0.f, + 1.f, + 0.f}, D3DCOLOR_ARGB(255, 0, 255, 255) };
	vertexs[2] = VTXCOL{ {+ 1.f, - 1.f, + 0.f}, D3DCOLOR_ARGB(255, 0, 0, 255) };
	m_vVertexBuffer->Unlock(); // 버퍼 이용이 끝나면 잠금을 해제한다.

	WORD* indices;
	m_vIndexBuffer->Lock(0, 0, (void**)&indices, 0);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	m_vIndexBuffer->Unlock();
}

void CShape::Draw_Triangle()
{
	m_pDeviceClass->SetStreamSource(0, m_vVertexBuffer, 0, sizeof(VTXCOL));
	m_pDeviceClass->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pDeviceClass->SetIndices(m_vIndexBuffer);

	// 6. 삼각형 그리기
	m_pDeviceClass->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, // 삼각형 리스트로 그림
		0,                  // BaseVertexIndex
		0,                  // MinIndex
		3,                  // NumVertices
		0,                  // StartIndex
		1);                 // PrimitiveCount (삼각형 1개)
}

void CShape::Offset(const float& fTimeDelta)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_tInfo.m_vPos.x += fTimeDelta * m_fSpeed * m_tInfo.m_vDir.x;
		m_tInfo.m_vPos.y += fTimeDelta * m_fSpeed * m_tInfo.m_vDir.y;
		m_tInfo.m_vPos.z += fTimeDelta * m_fSpeed * m_tInfo.m_vDir.z;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_tInfo.m_vPos.x -= fTimeDelta * m_fSpeed * m_tInfo.m_vDir.x;
		m_tInfo.m_vPos.y -= fTimeDelta * m_fSpeed * m_tInfo.m_vDir.y;
		m_tInfo.m_vPos.z -= fTimeDelta * m_fSpeed * m_tInfo.m_vDir.z;
	}

	
	if (GetAsyncKeyState('A') & 0x8000)
		m_vAngle.x += fTimeDelta* m_fSpeed * 10;

	if (GetAsyncKeyState('D') & 0x8000)
		m_vAngle.x -= fTimeDelta * m_fSpeed * 10;

	if (GetAsyncKeyState('Q') & 0x8000)
		m_vAngle.y -= fTimeDelta * m_fSpeed * 10;

	if (GetAsyncKeyState('E') & 0x8000)
		m_vAngle.y += fTimeDelta * m_fSpeed * 10;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_vAngle.z += fTimeDelta * m_fSpeed * 10;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_vAngle.z -= fTimeDelta * m_fSpeed * 10;
}

int CShape::Update_MainApp(const float& fTimeDelta)
{
	Dir_Calculate();
	return S_OK;
}

void CShape::LateUpdate_MainApp(const float& fTimeDelta)
{
	Offset(fTimeDelta);
	Update_Vertex();
}

void CShape::Update_Vertex()
{
	// 스 * 자 * 이
	D3DXMATRIX matWorld = Make_WorldMatrix(); // 내가 구한 내 World 행렬

	m_pDeviceClass->SetTransform(D3DTS_WORLD, &matWorld);
}

void CShape::Dir_Calculate()
{
	D3DXMATRIX _matRotate = Make_RotationMatrix();

	D3DXVec3TransformCoord(&m_tInfo.m_vDir, &m_tInfo.m_vDefault, &_matRotate);
	D3DXVec3Normalize(&m_tInfo.m_vDir, &m_tInfo.m_vDir);

	AllocConsole();

	FILE* pStream;
	freopen_s(&pStream, "CONOUT$", "w", stdout); // 안전한 방식
	cout << "Dir: " << m_tInfo.m_vDir.x << ", " << m_tInfo.m_vDir.y << ", " << m_tInfo.m_vDir.z << endl;
}

D3DXMATRIX CShape::Make_WorldMatrix()
{
	D3DXMATRIX _matTrans, _matRotate, _matScale;

	D3DXMatrixScaling(&_matScale, m_tInfo.m_vScale.x, m_tInfo.m_vScale.y, m_tInfo.m_vScale.z);

	D3DXMatrixTranslation(&_matTrans, m_tInfo.m_vPos.x, m_tInfo.m_vPos.y, m_tInfo.m_vPos.z);

	_matRotate = Make_RotationMatrix();

	D3DXMATRIX _matWorld;
	_matWorld = _matScale * _matRotate * _matTrans;


	return _matWorld;
}

D3DXMATRIX CShape::Make_RotationMatrix()
{
	D3DXMATRIX matRotateX, matRotateY, matRotateZ;

	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(m_vAngle.z));

	D3DXMATRIX _matRotate = matRotateX * matRotateY * matRotateZ;

	return _matRotate;
}