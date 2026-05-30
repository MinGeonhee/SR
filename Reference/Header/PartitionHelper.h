#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL PartitionHelper
{
public:
	// 현재 내가 속해있는 인덱스, X파티션 갯수, Z 파티션 갯수, 지름 몇짜리 파티션 Circle인지.
	static void CircleCalculate(int PlayerIndex, int XPartition, int ZPartition, int diameter, queue<int>& indexList);
};

END