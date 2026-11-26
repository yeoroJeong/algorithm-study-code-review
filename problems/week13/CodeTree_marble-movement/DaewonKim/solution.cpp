#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int n, m, t, k;

int dx[4] = {-1, 1, 0 ,0};
int dy[4] = {0, 0, -1, 1};

struct Ball
{
	int id = -1;
	int x = -1;
	int y = -1;
	int dir = -1;
	int speed = -1;		// -1이면 없는 공.
};

vector<Ball> balls;

bool isInRange(const int& x, const int& y)
{
	return (x >= 0 && x < n && y >= 0 && y < n);
}

// 속도가 빠른, id값이 큰.
bool compare(const int &a, const int &b)
{
	if (balls[a].speed == balls[b].speed)
		return a > b ? true : false;	
	return balls[a].speed > balls[b].speed;
}

void moveBall(vector<vector<vector<int>>>& board)
{
	// 공 만큼 반복.
	for (int i = 1; i < (int)balls.size(); ++i)
	{
		// 부서진 공은 건뛰
		if (balls[i].speed == -1) continue;

		int curX = balls[i].x;
		int curY = balls[i].y;
		int curSpeed = balls[i].speed;
		int curDir = balls[i].dir;
		
		for (int speed = 1; speed <= curSpeed; ++speed)
		{
			int nextX = curX + dx[curDir];
			int nextY = curY + dy[curDir];

			// 다음 칸이 범위를 벗어난다면.
			if (!isInRange(nextX, nextY))
			{
				// 방향을 바꿔주고 이동시킨다.
				if (curDir == 0)
					curDir = 1;
				else if (curDir == 1)
					curDir = 0;
				else if (curDir == 2)
					curDir = 3;
				else
					curDir = 2;

				// 방향을 바꾸고 위치값까지 갱신.
				nextX = curX + dx[curDir];
				nextY = curY + dy[curDir];
				
				// 방향을 갱신
				balls[i].dir = curDir;

			}
			// 이동 시킨 값을 갱신.
			curX = nextX; 
			curY = nextY;
		}
		
		// 이동한 좌표 정보를 update
		balls[i].x = curX; balls[i].y = curY;
		
		// board에 구슬의 id 값을 넣어준다.
		board[curX][curY].push_back(i);
	}

}

// 충돌검사
void checkCrush(vector<vector<vector<int>>>& board)
{
	for (int x = 0; x < n; ++x)
	{
		for (int y = 0; y < n; ++y)
		{
			// 들어 있는 구슬의 갯수가 k보다 크다면? -> 부서지는 구슬이 발생.
			if ((int)board[x][y].size() > k)
			{
				// 우선순위 대로 정렬을 통해 앞에서부터 k개만 살리고 나머지는 버림.
				sort(board[x][y].begin(), board[x][y].end(), compare);
				for (int idx = k; idx < (int)board[x][y].size(); ++idx)
				{
					int id = board[x][y][idx];
					balls[id].speed = -1; // 죽음 표시
				}

			}
			// 전부 파악 했으면 clear
			board[x][y].clear();
		}
	}
}

int main() {
	cin >> n >> m >> t >> k;

	// board : 3차원 배열로 만듬.
	// 초기화를 잘못해줌, 마지막 vector에 n을 습관적으로 넣어버림. 
	vector<vector<vector<int>>> board(n, vector<vector<int>> (n, vector<int>()));
	
	// dummy data
	Ball b;
	balls.push_back(b); 
	
	for (int i = 1; i <= m; i++) {
		int x, y, dir, speed;
		char tmpDir;
		
		cin >> x >> y >> tmpDir >> speed;

		if (tmpDir == 'U')
			dir = 0;
		else if (tmpDir == 'D')
			dir = 1;
		else if (tmpDir == 'L')
			dir = 2;
		else
			dir = 3;

		balls.push_back({ i, x - 1, y - 1, dir, speed });

	}

	int curT = 0;
	
	while (curT < t)
	{
		// 공 이동
		moveBall(board);
		// 충돌 체크
		checkCrush(board);
		curT++;
	}

	int answer = 0;
	for (auto&it : balls)
	{
		if (it.speed != -1)
			answer++;
	}
	cout << answer;

	return 0;
}
