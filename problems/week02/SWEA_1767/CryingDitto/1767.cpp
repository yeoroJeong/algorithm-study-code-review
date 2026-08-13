// 1767. 프로세서 연결하기
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV4suNtaXFEDFAUf&

#include <iostream>
#include <vector>
#include <climits>
using namespace std;
int mapSize;
int map[12][12];
bool visited[12][12];
vector<pair<int, int>> cores;
int connectedMaxCores = 0;
int totalCoreCnt;
int minLength = INT_MAX;

int dx[4] = { -1,1,0,0 };
int dy[4] = { 0,0,-1,1 };

// check if connection from core to the power(edge) is possible
int testConnection(int x, int y, int dir)
{
	int length = 0;
	int nx = x + dx[dir];
	int ny = y + dy[dir];
	while (nx >= 0 && ny >= 0 && nx < mapSize && ny < mapSize)
	{
		if (visited[nx][ny] || map[nx][ny] == 1)
		{
			// unable to connect
			length = -1;
			break;
		}
		// possible to step forward
		nx += dx[dir];
		ny += dy[dir];
		length += 1;
	}
	return length;
}

void fillVisited(int x, int y, int dir, bool val) {
	// keep core position as visited
	int nx = x + dx[dir];
	int ny = y + dy[dir];
	while (nx >= 0 && ny >= 0 && nx < mapSize && ny < mapSize)
	{
		visited[nx][ny] = val;
		nx = nx + dx[dir];
		ny = ny + dy[dir];
	}

}

void wireCores(int coreIdx, int connectedCoreCnt, int wireLength)
{
	if (coreIdx >= cores.size())
	{
		// if the number of connected cores is larger than previous max counts
		if (connectedCoreCnt > connectedMaxCores)
		{
			// update max core counts & min wire length
			connectedMaxCores = connectedCoreCnt;
			minLength = wireLength;
		}
		else if (connectedCoreCnt == connectedMaxCores) 
		{
			// update length only when core count is valid
			minLength = min(wireLength, minLength);
		}
		return;
	}

	int x = cores[coreIdx].first;
	int y = cores[coreIdx].second;
	// check if connection is possible towards four direction
	for (int dirIdx = 0; dirIdx < 4; dirIdx++)
	{
		// check if connection towards power is possible
		int length = testConnection(x, y, dirIdx);

		if (length > 0)
		{
			// try connecting current core in current direction 
			fillVisited(cores[coreIdx].first, cores[coreIdx].second, dirIdx, true);
			wireCores(coreIdx + 1, connectedCoreCnt + 1, wireLength + length);
			fillVisited(cores[coreIdx].first, cores[coreIdx].second, dirIdx, false);
		}
		
	}
	// try not connecting current core
	wireCores(coreIdx + 1, connectedCoreCnt, wireLength);

}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;
	for (int tc = 0; tc < T; tc++)
	{
		cin >> mapSize;
		// initialize global variables
		connectedMaxCores = 0;
		totalCoreCnt = 0;
		minLength = INT_MAX;
		cores.clear();
		for (int i = 0; i < mapSize; i++)
		{
			for (int j = 0; j < mapSize; j++)
			{
				// initialize map and visited array
				cin >> map[i][j];
				visited[i][j] = false;
				if (map[i][j] == 1)
				{
					totalCoreCnt++;
					visited[i][j] = true;
					if ((i == 0 || j == 0 || i == mapSize - 1 || j == mapSize - 1))
					{
						connectedMaxCores++;
					}
					else
					{
						cores.push_back({ i,j });
					}
				}
			}
		}

		wireCores(0, connectedMaxCores, 0);
		
		cout << "#" << tc + 1 << " " << minLength << "\n";
	}
	return 0;
}