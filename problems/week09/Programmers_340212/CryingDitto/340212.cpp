#include <string>
#include <vector>
#include <map>
using namespace std;

// points 운송 포인트 n개의 좌표를 담은 2차원 정수 배열 
	// 2 ≤ points의 길이 = n ≤ 100
	// 각 좌표 r, c도 1~100
// routes 로봇 x대의 운송 경로를 담은 2차원 정수 배열
	// 2 ≤ x ≤ 10
	// routes의 길이 = 로봇의 수 = x
// 2 ≤ routes[i]의 길이 = m ≤ 100
// routes[i][j]
	// i + 1번째 로봇이 j + 1번째로 방문하는 포인트 번호
	// 1 ≤ routes[i][j] ≤ n

// 이동은 거리 같을 경우 r 우선, c는 다음

// 로봇이 현재 가야 하는 목적지 idx (routes 접근 시 사용)
vector<int> dIdx;
// 각 로봇 작업 다 끝났는지 확인용
vector<bool> isDone;
// 각 robot 현재 위치 (while에서 사용)
vector<pair<int, int>> curPos;

int solution(vector<vector<int>> points, vector<vector<int>> routes) {
	int answer = 0;

	dIdx.clear();
	isDone.clear();
	curPos.clear();
	int robotCnt = routes.size();
	int routeLength = routes[0].size();
	isDone.resize(robotCnt, false);

	map<pair<int, int>, int> robotPositions;

	for (int i = 0; i < robotCnt; i++)
	{
		dIdx.push_back(1);
		int x = points[routes[i][0] - 1][0];
		int y = points[routes[i][0] - 1][1];
		curPos.push_back({ x,y });
		robotPositions[curPos[i]]++;
	}
	for (auto pos : robotPositions)
	{
		if (pos.second > 1)
		{
			answer++;
		}
	}

	int completedCnt = 0;
	while (completedCnt < robotCnt)
	{
		map<pair<int, int>, int> robotPositions;
		for (int i = 0; i < robotCnt; i++)
		{
			// 작업 다 한 로봇은 스킵
			if (isDone[i]) continue;

			auto route = routes[i];
			pair<int, int> curXY = curPos[i];

			int destionationIdx = dIdx[i];
			int toIdx = route[destionationIdx] - 1;

			// 목적지와의 차이 확인 후 이동
			int xDiff = points[toIdx][0] - curXY.first;
			int yDiff = points[toIdx][1] - curXY.second;

			if (xDiff != 0)
			{
				int adder = xDiff > 0 ? 1 : -1;
				curPos[i] = { curXY.first + adder, curXY.second };
				robotPositions[curPos[i]]++;
			}
			else if (yDiff != 0)
			{
				int adder = yDiff > 0 ? 1 : -1;
				curPos[i] = { curXY.first, curXY.second + adder };
				robotPositions[curPos[i]]++;
			}

			if (curPos[i].first == points[toIdx][0] && curPos[i].second == points[toIdx][1])
			{
				// x, y 좌표 둘다 같은 것 -> 다음 목적지 접근할 수 있게 index 업데이트
				dIdx[i]++;
				if (dIdx[i] == routeLength)
				{
					isDone[i] = true;
					completedCnt++;
				}
			}
		}
		// robot 위치 업데이트 완료 -> 검사
		for (auto pos : robotPositions)
		{
			if (pos.second > 1)
			{
				answer++;
			}
		}
	}

	return answer;
}