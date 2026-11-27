#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int T;
int N, M;

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

struct Ball
{
	int x = -1; int y = -1;
	int dir = -1;
	bool alive = true;
};

vector<Ball> balls;

bool isInRange(const int& x, const int& y)
{
	return (x >= 0 && x < N && y >= 0 && y < N);
}

void move(vector<vector<vector<int>>>& board)
{
	for (int num = 1; num <= M; ++num)
	{
		Ball& b = balls[num];

		if (!b.alive) continue;

		int curX = b.x;
		int curY = b.y;

		int nextX = curX + dx[b.dir];
		int nextY = curY + dy[b.dir];

		// 범위 밖 -> 그대로 방향만 바꿈
		if (!isInRange(nextX, nextY))
		{
			switch (b.dir)
			{
			case 0:
			{
				b.dir = 1;
				board[curX][curY].push_back(num);
				break;
			}
			case 1:
			{
				b.dir = 0;
				board[curX][curY].push_back(num);
				break;
			}
			case 2:
			{
				b.dir = 3;
				board[curX][curY].push_back(num);
				break;
			}
			case 3:
			{
				b.dir = 2;
				board[curX][curY].push_back(num);
				break;
			}
			}
		}
		else
		{
			board[nextX][nextY].push_back(num);

			b.x = nextX;
			b.y = nextY;
		}
	}
}

void checkCrash(vector<vector<vector<int>>>& board)
{
	for (int row = 0; row < N; ++row)
	{
		for (int col = 0; col < N; ++col)
		{
			// 딱 1개
			if (board[row][col].size() == 1)
			{
				int num = board[row][col][0];
				balls[num].x = row;
				balls[num].y = col;
			}
			// 여러 개.
			else if (!board[row][col].empty())
			{
				for (auto& num : board[row][col])
				{
					balls[num].alive = false;
				}
			}
		}
	}
}

// 후보군 vector 초기화
void initializeSection(vector<vector<vector<int>>>& board)
{
	for (int row = 0; row < N; ++row)
	{
		for (int col = 0; col < N; ++col)
		{
			board[row][col].clear();
		}
	}
}

int main()
{
	cin >> T;
	for (int t = 0; t < T; t++)
	{
		cin >> N >> M;

		balls.assign(M + 1, Ball{});

		vector<vector<vector<int>>> board(
			N, vector<vector<int>>(N)
		);

		// 공 정보 입력 초기화.
		for (int i = 0; i < M; i++)
		{
			int x, y;
			char dir;
			cin >> x >> y >> dir;

			balls[i + 1].x = x - 1;
			balls[i + 1].y = y - 1;

			switch (dir)
			{
			case 'U':
			{
				balls[i + 1].dir = 0;
				break;
			}
			case 'D':
			{
				balls[i + 1].dir = 1;
				break;
			}
			case 'L':
			{
				balls[i + 1].dir = 2;
				break;
			}
			case 'R':
			{
				balls[i + 1].dir = 3;
				break;
			}
			}
		}

		for (int time = 0; time < 2 * N; ++time)
		{
			// 이동
			move(board);

			// 충돌 판정
			checkCrash(board);
			initializeSection(board);
		}

		int answer = 0;
		for (int num = 1; num <= M; ++num)
		{
			if (balls[num].alive)
				answer++;
		}

		cout << answer << "\n";
	}

	return 0;
}