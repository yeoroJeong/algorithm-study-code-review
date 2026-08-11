// 2105. [모의 SW 역량테스트] 디저트 카페
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5VwAr6APYDFAWu
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int mapSize;
vector<vector<int>> cafeMap;
vector<vector<bool>> visited;
int startX, startY;

bool dessertSet[101]; // 1~100
int maxDessertCount = -1;
// right-down, left-down, left-up, right-up
int dx[4] = { 1, 1, -1, -1 };
int dy[4] = { 1, -1, -1, 1 };

void search(int x, int y, int dir, int dessertCount)
{
	if (dir == 3 && (x == startX && y == startY) && dessertCount >= 4)
	{
		// escape
		maxDessertCount = max(maxDessertCount, dessertCount);
		return;
	}
	if(dessertSet[cafeMap[x][y]])
	{
		// already visited
		return;
	}
	visited[x][y] = true;
	dessertSet[cafeMap[x][y]] = true;

	// fix direction to avoid duplicate paths
	for (int nextDir = dir; nextDir < 4; nextDir++)
	{
		// from current direction to direction 3 (right-up)
		int nx = x + dx[nextDir];
		int ny = y + dy[nextDir];
		if (nx < 0 || ny < 0 || nx >= mapSize || ny >= mapSize)
		{
			continue;
		}

		// start point needs to be visited again
		// other points cannot be visited again
		if (!visited[nx][ny] || (nx==startX && ny==startY))
		{
			search(nx, ny, nextDir, dessertCount + 1);
		}
	}
	visited[x][y] = false;
	dessertSet[cafeMap[x][y]] = false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;

	for (int tc = 0; tc < T; tc++)
	{
		cin >> mapSize;
		cafeMap.clear();
		cafeMap.resize(mapSize);
		visited.resize(mapSize);
		maxDessertCount = -1;
		fill(dessertSet, dessertSet + 101, 0);
		for (int i = 0; i < mapSize; i++)
		{
			cafeMap[i].resize(mapSize);
			visited[i].resize(mapSize, false);
			for (int j = 0; j < mapSize; j++)
			{
				cin >> cafeMap[i][j];
			}
		}

		for (int i = 0; i < mapSize; i++)
		{
			for (int j = 0; j < mapSize; j++)
			{
				// start in right-down direction -> left, right, bottom edges cannot be allowed
				// 방향 우하단 시작으로 고정, 0,0과 edge들은 사각형 완성 불가
				if ((i == 0 && j == 0) || j == 0 || i == mapSize - 1 || j == mapSize - 1)
				{
					continue;
				}

				startX = i;
				startY = j;
				search(i, j, 0, 0);
			}
		}
		cout << "#" << tc + 1 << " " << maxDessertCount << "\n";
	}
	return 0;
}