#include <string>
#include <vector>
#include <algorithm>


using namespace std;

int solution(vector<vector<int>> points, vector<vector<int>> routes) {
	int robotCount = (int)routes.size();

	// robotPaths[robot][time] = time초에 robot이 있는 좌표 {row, col}
	vector<vector<pair<int, int>>> robotPaths(robotCount);

	for (int robot = 0; robot < robotCount; ++robot) {
		// ---------- 출발 지점 ----------
		int startPointId = routes[robot][0];		// 출발 지점 번호 (1부터 시작)
		int startPointIndex = startPointId - 1;     // points 벡터 인덱스 (0부터 시작)

		// 시작 지점에 대한 좌표
		int curRow = points[startPointIndex][0];
		int curCol = points[startPointIndex][1];
		robotPaths[robot].push_back({ curRow, curCol });   // time = 0

		// ---------- 다음 목표 지점들을 차례로 이동 ----------
		int routeLength = (int)routes[robot].size();
		for (int routeOrder = 1; routeOrder < routeLength; ++routeOrder) {
			int targetPointId = routes[robot][routeOrder];
			int targetPointIndex = targetPointId - 1;

			int targetRow = points[targetPointIndex][0];
			int targetCol = points[targetPointIndex][1];

			// row 이동을 먼저
			while (curRow != targetRow) {
				if (targetRow > curRow) {
					curRow++;
				}
				else {
					curRow--;
				}
				robotPaths[robot].push_back({ curRow, curCol });
			}

			// 그다음 col 이동
			while (curCol != targetCol) {
				if (targetCol > curCol) {
					curCol++;
				}
				else {
					curCol--;
				}
				robotPaths[robot].push_back({ curRow, curCol });
			}
		}
	}


	// 가장 긴 거리를 가진 로봇의 이동 시간 만큼 설정.
	int maxTime = 0;
	for (auto& path : robotPaths) {
		maxTime = max(maxTime, (int)path.size());
	}

	// robotsAtCell[row][col] = 현재 시간에 (row, col)에 있는 로봇 수
	vector<vector<int>> robotsAtCell(101, vector<int>(101, 0));
	int dangerCount = 0;

	for (int time = 0; time < maxTime; ++time) {
		// 1) 현재 시간의 로봇 위치를 기록하고 위험 상황을 센다
		for (int robot = 0; robot < robotCount; ++robot) {

			// 이미 도착했다면
			if (time >= robotPaths[robot].size()) 
				continue;   
		

			int row = robotPaths[robot][time].first;
			int col = robotPaths[robot][time].second;

			robotsAtCell[row][col]++;
			if (robotsAtCell[row][col] == 2) {
				dangerCount++;
			}
		}

		// 2) 기록한 칸만 0으로 되돌린다
		for (int robot = 0; robot < robotCount; ++robot) {
			// 이미 도착한 로봇은 제외
			if (time >= (int)robotPaths[robot].size()) {
				continue;
			}
			int row = robotPaths[robot][time].first;
			int col = robotPaths[robot][time].second;
			robotsAtCell[row][col] = 0;
		}
	}

	return dangerCount;
}