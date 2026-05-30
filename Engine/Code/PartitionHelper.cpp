#include "PartitionHelper.h"


void PartitionHelper::CircleCalculate
(int PlayerIndex, int XPartition, int ZPartition, int diameter, queue<int>& indexList)
{
	if (PlayerIndex < 0 || XPartition < 0 || ZPartition < 0 || diameter <= 0) return;

	queue<int>	IndexList; // 숫자를 담아둘 Index 리스트를 작성.

	const int PlayerXIndex = PlayerIndex % XPartition;	// x 값의 인덱스
	const int PlayerZIndex = PlayerIndex / XPartition;	// z 값의 인덱스

	const int r = diameter / 2;	// 원의 반지름
	const int r2 = r * r;	// 원의 넒이

	// 탐색 범위를 지정한다. + 형태로 Min Max를 구함.

	const int xMin = max(0, PlayerXIndex - r);	// 0과 원의 반지름을 뺀 내 인덱스값 비교
	const int xMax = min(XPartition - 1, PlayerXIndex + r);
	const int zMin = max(0, PlayerZIndex - r);
	const int zMax = min(XPartition - 1, PlayerZIndex + r);

	// 거리와 인덱스를 넣어준다.
	vector<pair<int, int>> cand;
	cand.reserve((xMax - xMin + 1) * (zMax - zMin + 1));

	for (int z = zMin; z < zMax; ++z)
	{
		const int dz = z - PlayerZIndex;	// 플레이어와 z 거리
		const int dz2 = dz * dz;			// 거리^2

		for (int x = xMin; x < xMax; ++x)
		{
			const int dx = x - PlayerXIndex;
			const int d2 = dx * dx + dz2;	// 피타고라스 정리 쓸 준비.  a^2 + b^2 = c^2

			if (d2 <= r2)	// 만약에 원 내부라고 한다면.
			{
				const int idx = z * XPartition + x;
				cand.emplace_back(d2, idx);	// 거리와 인덱스 넣기
			}
		}
	}

	sort(cand.begin(), cand.end(), [](const auto& a, const auto& b)
		{
			if (a.first != b.first)
				return a.first < b.first;

			return a.second < b.second;
		});

		for (auto& p : cand)
			indexList.push(p.second);

}