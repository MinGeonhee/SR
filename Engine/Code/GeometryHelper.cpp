#include "GeometryHelper.h"


void GeometryHelper::CreateQuad(Geometry<VTXCOL>* geometry, D3DXCOLOR color)
{
	vector<VTXCOL> vtx;
	vtx.resize(4);

	vtx[0].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	vtx[0].dwColor = color;
	vtx[1].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	vtx[1].dwColor = color;
	vtx[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	vtx[2].dwColor = color;
	vtx[3].vPosition = _vec3(0.5f, 0.5f, 0.f);
	vtx[3].dwColor = color;
	geometry->SetVertices(vtx);

	vector<_uint32> idx = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateQuad(Geometry<VTXTEX>* geometry)
{
	vector<VTXTEX> vtx;
	vtx.resize(4);

	vtx[0].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	vtx[0].vTexUV = _vec2(0.f, 1.f);
	vtx[1].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	vtx[1].vTexUV = _vec2(0.f, 0.f);
	vtx[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	vtx[2].vTexUV = _vec2(1.f, 1.f);
	vtx[3].vPosition = _vec3(0.5f, 0.5f, 0.f);
	vtx[3].vTexUV = _vec2(1.f, 0.f);
	geometry->SetVertices(vtx);

	vector<_uint32> idx = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(Geometry<VTXTEX>* geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<VTXTEX> vtx(24);

	// 앞면
	vtx[0] = VTXTEX{ _vec3(-w2, -h2, -d2), _vec2(0.0f, 1.0f) };
	vtx[1] = VTXTEX{ _vec3(-w2, +h2, -d2), _vec2(0.0f, 0.0f) };
	vtx[2] = VTXTEX{ _vec3(+w2, +h2, -d2), _vec2(1.0f, 0.0f) };
	vtx[3] = VTXTEX{ _vec3(+w2, -h2, -d2), _vec2(1.0f, 1.0f) };
	// 뒷면
	vtx[4] = VTXTEX{ _vec3(-w2, -h2, +d2), _vec2(1.0f, 1.0f) };
	vtx[5] = VTXTEX{ _vec3(+w2, -h2, +d2), _vec2(0.0f, 1.0f) };
	vtx[6] = VTXTEX{ _vec3(+w2, +h2, +d2), _vec2(0.0f, 0.0f) };
	vtx[7] = VTXTEX{ _vec3(-w2, +h2, +d2), _vec2(1.0f, 0.0f) };
	// 윗면
	vtx[8] = VTXTEX{ _vec3(-w2, +h2, -d2), _vec2(0.0f, 1.0f) };
	vtx[9] = VTXTEX{ _vec3(-w2, +h2, +d2), _vec2(0.0f, 0.0f) };
	vtx[10] = VTXTEX{ _vec3(+w2, +h2, +d2), _vec2(1.0f, 0.0f) };
	vtx[11] = VTXTEX{ _vec3(+w2, +h2, -d2), _vec2(1.0f, 1.0f) };
	// 아랫면
	vtx[12] = VTXTEX{ _vec3(-w2, -h2, -d2), _vec2(1.0f, 1.0f) };
	vtx[13] = VTXTEX{ _vec3(+w2, -h2, -d2), _vec2(0.0f, 1.0f) };
	vtx[14] = VTXTEX{ _vec3(+w2, -h2, +d2), _vec2(0.0f, 0.0f) };
	vtx[15] = VTXTEX{ _vec3(-w2, -h2, +d2), _vec2(1.0f, 0.0f) };
	// 왼쪽면
	vtx[16] = VTXTEX{ _vec3(-w2, -h2, +d2), _vec2(0.0f, 1.0f) };
	vtx[17] = VTXTEX{ _vec3(-w2, +h2, +d2), _vec2(0.0f, 0.0f) };
	vtx[18] = VTXTEX{ _vec3(-w2, +h2, -d2), _vec2(1.0f, 0.0f) };
	vtx[19] = VTXTEX{ _vec3(-w2, -h2, -d2), _vec2(1.0f, 1.0f) };
	// 오른쪽면
	vtx[20] = VTXTEX{ _vec3(+w2, -h2, -d2), _vec2(0.0f, 1.0f) };
	vtx[21] = VTXTEX{ _vec3(+w2, +h2, -d2), _vec2(0.0f, 0.0f) };
	vtx[22] = VTXTEX{ _vec3(+w2, +h2, +d2), _vec2(1.0f, 0.0f) };
	vtx[23] = VTXTEX{ _vec3(+w2, -h2, +d2), _vec2(1.0f, 1.0f) };


	geometry->SetVertices(vtx);

	vector<_uint32> idx(36);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}


// 이야 GPT 일 잘한다
void GeometryHelper::CreateCubeNet(Geometry<VTXTEX>*geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	const float u = 0.25f;
	const float v = 1.0f / 3.0f;

	vector<VTXTEX> vtx(24);

	// === Front (U: 0.25~0.5, V: 1/3~2/3)
	vtx[0] = { _vec3(-w2, -h2, -d2), _vec2(u, 2 * v) };         // left bottom
	vtx[1] = { _vec3(-w2, +h2, -d2), _vec2(u, v) };           // left top
	vtx[2] = { _vec3(+w2, +h2, -d2), _vec2(2 * u, v) };         // right top
	vtx[3] = { _vec3(+w2, -h2, -d2), _vec2(2 * u, 2 * v) };       // right bottom

	// === Back (U: 0.75~1.0)
	vtx[4] = { _vec3(-w2, -h2, +d2), _vec2(3 * u, 2 * v) };
	vtx[5] = { _vec3(+w2, -h2, +d2), _vec2(4 * u, 2 * v) };
	vtx[6] = { _vec3(+w2, +h2, +d2), _vec2(4 * u, v) };
	vtx[7] = { _vec3(-w2, +h2, +d2), _vec2(3 * u, v) };

	// === Up (U: 0.25~0.5, V: 0~1/3)
	vtx[8] = { _vec3(-w2, +h2, -d2), _vec2(u, 0.f) };
	vtx[9] = { _vec3(-w2, +h2, +d2), _vec2(u, v) };
	vtx[10] = { _vec3(+w2, +h2, +d2), _vec2(2 * u, v) };
	vtx[11] = { _vec3(+w2, +h2, -d2), _vec2(2 * u, 0.f) };

	// === Down (U: 0.25~0.5, V: 2/3~1.0)
	vtx[12] = { _vec3(-w2, -h2, -d2), _vec2(u, 2 * v) };
	vtx[13] = { _vec3(+w2, -h2, -d2), _vec2(2 * u, 2 * v) };
	vtx[14] = { _vec3(+w2, -h2, +d2), _vec2(2 * u, 3 * v) };
	vtx[15] = { _vec3(-w2, -h2, +d2), _vec2(u, 3 * v) };

	// === Left (U: 0.0~0.25)
	vtx[16] = { _vec3(-w2, -h2, +d2), _vec2(0.f, 2 * v) };
	vtx[17] = { _vec3(-w2, +h2, +d2), _vec2(0.f, v) };
	vtx[18] = { _vec3(-w2, +h2, -d2), _vec2(u, v) };
	vtx[19] = { _vec3(-w2, -h2, -d2), _vec2(u, 2 * v) };

	// === Right (U: 0.5~0.75)
	vtx[20] = { _vec3(+w2, -h2, -d2), _vec2(2 * u, 2 * v) };
	vtx[21] = { _vec3(+w2, +h2, -d2), _vec2(2 * u, v) };
	vtx[22] = { _vec3(+w2, +h2, +d2), _vec2(3 * u, v) };
	vtx[23] = { _vec3(+w2, -h2, +d2), _vec2(3 * u, 2 * v) };

	geometry->SetVertices(vtx);

	vector<_uint32> idx(36);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}



void GeometryHelper::CreateSphere(Geometry<VTXTEX>* geometry)
{
	const float radius = 0.5f;
	const _uint32 stacks = 20;  // 위도 분할 (북->남, 세로)
	const _uint32 slices = 32;  // 경도 분할 (좌->우, 가로)

	vector<VTXTEX> vtx;
	vtx.reserve(1 + (stacks - 1) * (slices + 1) + 1);

	VTXTEX v;

	// 북극
	v.vPosition = _vec3(0.0f, radius, 0.0f);
	v.vTexUV = _vec2(0.5f, 0.0f);   // u는 임의여도 됨
	vtx.push_back(v);

	const float dPhi = D3DX_PI / stacks;        // 0..PI
	const float dTheta = 2.0f * D3DX_PI / slices; // 0..2PI

	// 중간 링(북극/남극 제외): 각 링마다 slices+1개(시임 복제)
	for (_uint32 y = 1; y <= stacks - 1; ++y) {
		float phi = y * dPhi;                     // 위도(0=북극, PI=남극)
		float sinPhi = sinf(phi), cosPhi = cosf(phi);

		for (_uint32 x = 0; x <= slices; ++x) {   // <=  (중요: 시임 복제)
			float theta = x * dTheta;             // 경도
			float cosTheta = cosf(theta), sinTheta = sinf(theta);

			v.vPosition.x = radius * sinPhi * cosTheta;
			v.vPosition.y = radius * cosPhi;
			v.vPosition.z = radius * sinPhi * sinTheta;

			// 이큐렉탱귤러 UV
			float u = (float)x / (float)slices;   // 0..1 (x=slices에서 1)
			float w = (float)y / (float)stacks;   // 0..1
			v.vTexUV = _vec2(u, w);

			vtx.push_back(v);
		}
	}

	// 남극
	v.vPosition = _vec3(0.0f, -radius, 0.0f);
	v.vTexUV = _vec2(0.5f, 1.0f);
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	// ----- 인덱스 -----
	vector<_uint32> idx;
	// 삼각형 수 대략: 2 * slices * (stacks - 1)
	idx.reserve((size_t)(2 * slices * (stacks - 1)) * 3);

	const _uint32 ring = slices + 1;          // 링당 정점 수
	const _uint32 north = 0;
	const _uint32 south = (_uint32)vtx.size() - 1;

	// 북극 캡
	// top ring 시작: 1
	for (_uint32 x = 0; x < slices; ++x) {
		_uint32 i0 = north;
		_uint32 i1 = 1 + x;         // 링의 x
		_uint32 i2 = 1 + x + 1;     // 링의 x+1

		// 바깥면 CCW 기준(일반 스피어). 스카이돔은 CULL_CW로 안쪽을 보자.
		idx.push_back(i0); idx.push_back(i1); idx.push_back(i2);
	}

	// 몸통
	// 링 개수: (stacks - 1) - 1 = stacks - 2
	for (_uint32 y = 0; y < stacks - 2; ++y) {
		_uint32 row0 = 1 + y * ring;         // 윗링 시작
		_uint32 row1 = row0 + ring;          // 아랫링 시작

		for (_uint32 x = 0; x < slices; ++x) {
			_uint32 i0 = row0 + x;
			_uint32 i1 = row0 + x + 1;
			_uint32 i2 = row1 + x;
			_uint32 i3 = row1 + x + 1;

			// 두 개의 삼각형
			idx.push_back(i0); idx.push_back(i2); idx.push_back(i1);
			idx.push_back(i1); idx.push_back(i2); idx.push_back(i3);
		}
	}

	// 남극 캡
	// bottom ring 시작: south - ring
	_uint32 rowLast = south - ring;
	for (_uint32 x = 0; x < slices; ++x) {
		_uint32 i0 = south;
		_uint32 i1 = rowLast + x + 1;
		_uint32 i2 = rowLast + x;

		idx.push_back(i0); idx.push_back(i1); idx.push_back(i2);
	}

	geometry->SetIndices(idx);
}


void GeometryHelper::CreateGrid(Geometry<VTXTEX>* geometry, int32 sizeX, int32 sizeZ)
{
	vector<VTXTEX> vtx;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VTXTEX v;
			v.vPosition = _vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.vTexUV = _vec2(static_cast<float>(x), static_cast<float>(z));

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<_uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}


void GeometryHelper::CreateGridLine(Geometry<VTXCOL>* geometry, int32 CntX, int32 CntZ, int Floor, int Size)
{
	//내가 지정한 버텍스의 정점 개수 가로 * 세로
	// 홀수면 비대칭 짝수면 비대칭

	vector<VTXCOL> vVtxCol;

	
	VTXCOL vtx;

	for (int z = 0 ; z <= CntZ; ++z)
	{
		vtx.vPosition = { (float)0 , (float)Floor , (float)Size * z  };
		vtx.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
		vVtxCol.push_back(vtx);

		vtx.vPosition = { (float)Size * CntX , (float)Floor , (float)Size * z };
		vtx.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
		vVtxCol.push_back(vtx);
	}

	for (int x = 0; x <= CntX; ++x)
	{
		vtx.vPosition = { (float)Size * x , (float)Floor , 0};
		vtx.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
		vVtxCol.push_back(vtx);

		vtx.vPosition = { (float)Size * x , (float)Floor ,  (float)CntZ * Size };
		vtx.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
		vVtxCol.push_back(vtx);
	}

	geometry->SetVertices(vVtxCol);

	vector<_uint32> idx;

	for (int i = 0; i < geometry->GetVertexCount(); i += 2)
	{
		idx.push_back(i);
		idx.push_back(i + 1);
	}

	geometry->SetIndices(idx);

}


void GeometryHelper::CreateQuad(Geometry<VTXNORMALTEX>* geometry)
{
	vector<VTXNORMALTEX> vtx;
	vtx.resize(4);

	vtx[0].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	vtx[0].vTexUV = _vec2(0.f, 1.f);
	vtx[0].vNormal = _vec3(0.f, 0.f, -1.f);
	vtx[1].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	vtx[1].vTexUV = _vec2(0.f, 0.f);
	vtx[1].vNormal = _vec3(0.f, 0.f, -1.f);
	vtx[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	vtx[2].vTexUV = _vec2(1.f, 1.f);
	vtx[2].vNormal = _vec3(0.f, 0.f, -1.f);
	vtx[3].vPosition = _vec3(0.5f, 0.5f, 0.f);
	vtx[3].vTexUV = _vec2(1.f, 0.f);
	vtx[2].vNormal = _vec3(0.f, 0.f, -1.f);
	geometry->SetVertices(vtx);

	vector<_uint32> idx = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(idx);
}
void GeometryHelper::CreateSkyCube(Geometry<VTXCUBE>* geometry)
{
	_vec3 scale = { 100, 100, 100 };

	float w2 = 0.5f * scale.x;	// x
	float h2 = 0.5f * scale.y;	// y
	float d2 = 0.5f * scale.z;	// z

	vector<VTXCUBE>	vtx(24);

	// 위 버텍스
	vtx[0] = VTXCUBE{ _vec3(-w2, +h2, +d2), _vec3(-w2, +h2, +d2) };
	vtx[1] = VTXCUBE{ _vec3(+w2, +h2, +d2), _vec3(+w2, +h2, +d2) };
	vtx[2] = VTXCUBE{ _vec3(+w2, +h2, -d2), _vec3(+w2, +h2, -d2) };
	vtx[3] = VTXCUBE{ _vec3(-w2, +h2, -d2), _vec3(-w2, +h2, -d2) };

	// 아래 버텍스
	vtx[4] = VTXCUBE{ _vec3(-w2, -h2, +d2), _vec3(-w2, -h2, +d2) };
	vtx[5] = VTXCUBE{ _vec3(+w2, -h2, +d2), _vec3(+w2, -h2, +d2) };
	vtx[6] = VTXCUBE{ _vec3(+w2, -h2, -d2), _vec3(+w2, -h2, -d2) };
	vtx[7] = VTXCUBE{ _vec3(-w2, -h2, -d2), _vec3(-w2, -h2, -d2) };


	// 방향벡터 전부 노말라이즈
	for (auto& Vertex : vtx)
		D3DXVec3Normalize(&Vertex.vTexUV, &Vertex.vTexUV);

	geometry->SetVertices(vtx);

	vector<_uint32> idx(36);

	// 윗면
	idx[0] = 0; idx[1] = 2; idx[2] = 3;
	idx[3] = 0; idx[4] = 1; idx[5] = 2;

	// 아랫면 
	idx[6] = 4; idx[7] = 6; idx[8] = 7;
	idx[9] = 4; idx[10] = 5; idx[11] = 6;

	// 왼쪽면
	idx[12] = 0; idx[13] = 7; idx[14] = 4;
	idx[15] = 0; idx[16] = 3; idx[17] = 7;

	// 오른쪽면
	idx[18] = 2; idx[19] = 5; idx[20] = 6;
	idx[21] = 2; idx[22] = 1; idx[23] = 5;

	// 앞면
	idx[24] = 3; idx[25] = 6; idx[26] = 7;
	idx[27] = 3; idx[28] = 2; idx[29] = 6;

	// 뒷면
	idx[30] = 0; idx[31] = 5; idx[32] = 4;
	idx[33] = 0; idx[34] = 1; idx[35] = 5;

	geometry->SetIndices(idx);

}

void GeometryHelper::CreateCube(Geometry<VTXNORMALTEX>* geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	std::vector<VTXNORMALTEX> vtx(24);

	auto make = [](const _vec3& p, const _vec3& n, const _vec2& uv) {
		VTXNORMALTEX v;
		v.vPosition = p;
		v.vNormal = n;
		v.vTexUV = uv;
		return v;
		};

	// 앞면 (0,0,-1)
	vtx[0] = make(_vec3(-w2, -h2, -d2), _vec3(0, 0, -1), _vec2(0.0f, 1.0f));
	vtx[1] = make(_vec3(-w2, +h2, -d2), _vec3(0, 0, -1), _vec2(0.0f, 0.0f));
	vtx[2] = make(_vec3(+w2, +h2, -d2), _vec3(0, 0, -1), _vec2(1.0f, 0.0f));
	vtx[3] = make(_vec3(+w2, -h2, -d2), _vec3(0, 0, -1), _vec2(1.0f, 1.0f));

	// 뒷면 (0,0,+1)
	vtx[4] = make(_vec3(-w2, -h2, +d2), _vec3(0, 0, 1), _vec2(1.0f, 1.0f));
	vtx[5] = make(_vec3(+w2, -h2, +d2), _vec3(0, 0, 1), _vec2(0.0f, 1.0f));
	vtx[6] = make(_vec3(+w2, +h2, +d2), _vec3(0, 0, 1), _vec2(0.0f, 0.0f));
	vtx[7] = make(_vec3(-w2, +h2, +d2), _vec3(0, 0, 1), _vec2(1.0f, 0.0f));

	// 윗면 (0,+1,0)
	vtx[8] = make(_vec3(-w2, +h2, -d2), _vec3(0, 1, 0), _vec2(0.0f, 1.0f));
	vtx[9] = make(_vec3(-w2, +h2, +d2), _vec3(0, 1, 0), _vec2(0.0f, 0.0f));
	vtx[10] = make(_vec3(+w2, +h2, +d2), _vec3(0, 1, 0), _vec2(1.0f, 0.0f));
	vtx[11] = make(_vec3(+w2, +h2, -d2), _vec3(0, 1, 0), _vec2(1.0f, 1.0f));

	// 아랫면 (0,-1,0)
	vtx[12] = make(_vec3(-w2, -h2, -d2), _vec3(0, -1, 0), _vec2(1.0f, 1.0f));
	vtx[13] = make(_vec3(+w2, -h2, -d2), _vec3(0, -1, 0), _vec2(0.0f, 1.0f));
	vtx[14] = make(_vec3(+w2, -h2, +d2), _vec3(0, -1, 0), _vec2(0.0f, 0.0f));
	vtx[15] = make(_vec3(-w2, -h2, +d2), _vec3(0, -1, 0), _vec2(1.0f, 0.0f));

	// 왼쪽면 (-1,0,0)
	vtx[16] = make(_vec3(-w2, -h2, +d2), _vec3(-1, 0, 0), _vec2(0.0f, 1.0f));
	vtx[17] = make(_vec3(-w2, +h2, +d2), _vec3(-1, 0, 0), _vec2(0.0f, 0.0f));
	vtx[18] = make(_vec3(-w2, +h2, -d2), _vec3(-1, 0, 0), _vec2(1.0f, 0.0f));
	vtx[19] = make(_vec3(-w2, -h2, -d2), _vec3(-1, 0, 0), _vec2(1.0f, 1.0f));

	// 오른쪽면 (+1,0,0)
	vtx[20] = make(_vec3(+w2, -h2, -d2), _vec3(1, 0, 0), _vec2(0.0f, 1.0f));
	vtx[21] = make(_vec3(+w2, +h2, -d2), _vec3(1, 0, 0), _vec2(0.0f, 0.0f));
	vtx[22] = make(_vec3(+w2, +h2, +d2), _vec3(1, 0, 0), _vec2(1.0f, 0.0f));
	vtx[23] = make(_vec3(+w2, -h2, +d2), _vec3(1, 0, 0), _vec2(1.0f, 1.0f));

	geometry->SetVertices(vtx);

	std::vector<_uint32> idx(36);
	// 앞
	idx[0] = 0;  idx[1] = 1;  idx[2] = 2;
	idx[3] = 0;  idx[4] = 2;  idx[5] = 3;
	// 뒤
	idx[6] = 4;  idx[7] = 5;  idx[8] = 6;
	idx[9] = 4;  idx[10] = 6;  idx[11] = 7;
	// 위
	idx[12] = 8;  idx[13] = 9;  idx[14] = 10;
	idx[15] = 8;  idx[16] = 10; idx[17] = 11;
	// 아래
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}


void GeometryHelper::CreateGrid(Geometry<VTXNORMALTEX>* geometry, int32 sizeX, int32 sizeZ)
{
	vector<VTXNORMALTEX> vtx;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VTXNORMALTEX v;
			v.vPosition = _vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.vTexUV = _vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.vNormal = _vec3(0.f, 1.f, 0.f);

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<_uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(Geometry< VTXNORMALTEX>* geometry)
{
	float radius = 0.5f; // 구의 반지름
	_uint32 stackCount = 20; // 가로 분할
	_uint32 sliceCount = 20; // 세로 분할

	vector<VTXNORMALTEX> vtx;

	VTXNORMALTEX v;

	// 북극
	v.vPosition = _vec3(0.0f, radius, 0.0f);
	v.vTexUV = _vec2(0.5f, 0.0f);
	v.vNormal = v.vPosition;
	D3DXVec3Normalize(&(v.vNormal), &(v.vNormal));
	vtx.push_back(v);

	float stackAngle = D3DX_PI / stackCount;
	float sliceAngle = (D3DX_PI * 2) / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (_uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (_uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.vPosition.x = radius * sinf(phi) * cosf(theta);
			v.vPosition.y = radius * cosf(phi);
			v.vPosition.z = radius * sinf(phi) * sinf(theta);

			v.vTexUV = _vec2(deltaU * x, deltaV * y);

			v.vNormal = v.vPosition;
			D3DXVec3Normalize(&(v.vNormal), &(v.vNormal));

			vtx.push_back(v);
		}
	}

	// 남극
	v.vPosition = _vec3(0.0f, -radius, 0.0f);
	v.vTexUV = _vec2(0.5f, 1.0f);
	v.vNormal = v.vPosition;
	D3DXVec3Normalize(&(v.vNormal), &(v.vNormal));
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	vector<_uint32> idx(36);

	// 북극 인덱스
	for (_uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// 몸통 인덱스
	_uint32 ringVertexCount = sliceCount + 1;
	for (_uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (_uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	_uint32 bottomIndex = static_cast<_uint32>(vtx.size()) - 1;
	_uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (_uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}