#pragma once
#include "Engine_Define.h"
#include "Geometry.h"

BEGIN(Engine)

class ENGINE_DLL GeometryHelper
{
public:
	static void CreateQuad(Geometry<VTXCOL>* geometry, D3DXCOLOR color);
	static void CreateQuad(Geometry<VTXTEX>* geometry);

	// GPT 말로는 전개도 큐브 이름이 CubeNet이래 ㅇㅇ 
	static void CreateCubeNet(Geometry<VTXTEX>* geometry);
	static void CreateCube(Geometry<VTXTEX>* geometry);
	static void CreateSkyCube(Geometry<VTXCUBE>* geometry);

	static void CreateSphere(Geometry<VTXTEX>* geometry);
	static void CreateGrid(Geometry<VTXTEX>* geometry, int32 sizeX, int32 sizeZ);
	static void CreateGridLine(Geometry<VTXCOL>* geometry, int32 CntX, int32 CntZ, int Floor, int Size);

	static void CreateQuad(Geometry<VTXNORMALTEX>* geometry);
	static void CreateCube(Geometry<VTXNORMALTEX>* geometry);

	static void CreateGrid(Geometry< VTXNORMALTEX>* geometry, int32 sizeX, int32 sizeZ);
	static void CreateSphere(Geometry< VTXNORMALTEX>* geometry);
};

END