#include <iostream>
#include <unordered_map>
#include <queue>
using namespace std;

// 5650. [모의 SW 역량테스트] 핀볼 게임
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWXRF8s6ezEDFAUo

// 0 빈 공간: 방향 유지
// 6 ~ 10 웜홀: 진입했을 때의 방향 유지, 같은 번호의 웜홀로 이동시킴 (최대 5쌍)
// 1 ~ 5 블록: 진입 방향에 따라 방향 수정이 다름

// 종료 조건: 블랙홀 (-1)에 빠질 때 or 출발 위치로 돌아올 때
// 출력 : 벽이나 블록에 부딪힌 누적 횟수의 최댓값

// 출발 가능 위치
// 블록, 블랙홀, 웜홀 없는 위치 -> 맵상에서 0인 위치

const int BLACKHOLE = -1;
const int WARPOFFSET = 6;

// 0상 1하 2좌 3우
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

// 행 index: 블록 번호 - 1, 열 index: 현재 dir 진행 방향(상하좌우)
//int dirX[4][4] = { {1,0,-1,0}, {0,-1,1,0}, {0,-1,0,1}, {1,0,0,-1} };
//int dirY[4][4] = { {0,1,0,-1}, {1,0,0,-1}, {-1,0,1,0}, {0,-1,1,0} };

// dirIdx[gameMap[nx][ny] - 1][dir];
// row idx: block type - 1, column idx: original direction
int dirIdx[4][4] = { {1, 3, 0, 2}, // 하 우 상 좌 
					{3, 0, 1, 2}, // 우 상 하 좌
					{2, 0, 3, 1}, // 좌 상 우 하
					{1, 2, 3, 0} // 하 좌 우 상
};

struct warphole
{
	int id;
	int x; int y;
};

unordered_map<int, warphole> warpholeMap;

pair<int, int> startPos;
queue<pair<int, int>> startPosQ;

int sizeN;
int gameMap[100][100];

int maxScore = 0;
int first = true;


int getReverseDir(int dirIdx)
{
	if (dirIdx == 0 || dirIdx == 2)
	{
		return dirIdx + 1;
	}
	if (dirIdx == 1 || dirIdx == 3)
	{
		return dirIdx - 1;
	}
	// dirIdx 범위 벗어난 것
	return -1;
}

void search(int x, int y, int count, int dir)
{
	//int loop = 0;

	while (true)
	{
		//loop++;
		//if (loop > 100000)
		//{
		//	cout << "INFINITE: "
		//		<< startPos.first << " "
		//		<< startPos.second << " / "
		//		<< dir << "\n";
		//	return;
		//}
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		x = nx;
		y = ny;
		// 벽에 부딪힘
		if (nx < 0 || ny < 0 || nx >= sizeN || ny >= sizeN)
		{
			dir = getReverseDir(dir);
			count++;
			continue;
		}

		// 종료 조건
		if ((x == startPos.first && y == startPos.second) || gameMap[x][y] == BLACKHOLE)
		{
			if (maxScore < count)
			{
				maxScore = count;
			}
			break;
		}

		// block
		if (gameMap[x][y] >= 1 && gameMap[x][y] < 5)
		{
			int newDir = dirIdx[gameMap[x][y] - 1][dir];
			dir = newDir;
			count++;
		}
		else if (gameMap[x][y] == 5)
		{
			dir = getReverseDir(dir);
			count++;
		}
		else if (gameMap[x][y] >= WARPOFFSET)
		{
			int id = gameMap[x][y];
			warphole w1 = warpholeMap[id];
			warphole w2 = warpholeMap[id - WARPOFFSET];
			// 다음 위치를 상대 워프 위치로 설정
			if (w1.x == x && w1.y == y)
			{
				x = w2.x;
				y = w2.y;
			}
			else
			{
				x = w1.x;
				y = w1.y;
			}
		}

	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);

	//freopen("input.txt", "r", stdin);
	int T;
	cin >> T;
	for (int tc = 0; tc < T; tc++)
	{
		cin >> sizeN;
		warpholeMap.clear();
		maxScore = 0;

		for (int i = 0; i < sizeN; i++)
		{
			fill(gameMap[i], gameMap[i] + 100, 0);
			for (int j = 0; j < sizeN; j++)
			{
				cin >> gameMap[i][j];
				if (gameMap[i][j] == 0)
				{
					startPosQ.push({ i, j });
				}
				else if (gameMap[i][j] >= WARPOFFSET)
				{
					int id = gameMap[i][j];

					if (warpholeMap.find(id) == warpholeMap.end())
					{
						// 없으니까 추가
						warpholeMap[id] = { id, i, j };
					}
					else
					{
						// 이미 있으면 id - 6으로 추가
						warpholeMap[id - WARPOFFSET] = { id, i, j };
					}
				}
			}
		}

		while (!startPosQ.empty())
		{
			startPos = startPosQ.front();
			startPosQ.pop();

			// 서로 다른 방향으로 시작해보기
			first = true;
			search(startPos.first, startPos.second, 0, 0);
			first = true;
			search(startPos.first, startPos.second, 0, 1);
			first = true;
			search(startPos.first, startPos.second, 0, 2);
			first = true;
			search(startPos.first, startPos.second, 0, 3);
		}

		cout << "#" << tc + 1 << " " << maxScore << "\n";
	}

	return 0;
}
