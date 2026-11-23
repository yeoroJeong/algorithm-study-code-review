#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int n, m;

int dx[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
int dy[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

struct Number {
	pair<int, int> pos;
};

// 수는 정확히 한번씩 나온다
map<int, Number> nums;

bool isInRange(const int& x, const int& y)
{
	return (x >= 0 && x < n && y >= 0 && y < n);
}
void switchNumber(vector<vector<int>>& board)
{
	for (auto& num : nums)
	{
		int tmpMax = -1;	// 인접 위치의 최댓값
		int tmpDir = -1;

		// 8방향 탐색
		for (int dir = 0; dir < 8; ++dir)
		{
			int nextX = num.second.pos.first + dx[dir];
			int nextY = num.second.pos.second + dy[dir];

			bool possible = isInRange(nextX, nextY);
			// 범위 밖이라면 건뛰
			if (!possible) continue;

			// 최댓값과 그 방향 갱신 
			if (tmpMax < board[nextX][nextY])
			{
				// 값 및 방향 갱신
				tmpMax = board[nextX][nextY];
				tmpDir = dir;
			}
		}

		// swap 관련 로직
		int curX = num.second.pos.first;
		int curY = num.second.pos.second;

		int nextX = curX + dx[tmpDir];
		int nextY = curY + dy[tmpDir];

		int tmpX = curX;
		int tmpY = curY;

		// board의 값을 갱신
		board[curX][curY] = tmpMax;
		board[nextX][nextY] = num.first;

		// 현재 검사하는 번호의 위치를 갱신
		nums[num.first].pos.first = nextX;
		nums[num.first].pos.second = nextY;

		// max값의 번호의 위치를 갱신
		nums[tmpMax].pos.first = tmpX;
		nums[tmpMax].pos.second = tmpY;


	}
}

int main() {
	cin >> n >> m;

	vector<vector<int>> board(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
			nums[board[i][j]].pos.first = i;    // x
			nums[board[i][j]].pos.second = j;   // y
		}
	}

	for (int turn = 0; turn < m; ++turn)
	{
		switchNumber(board);
	}

	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			cout << board[row][col] << " ";
		}
		cout << "\n";
	}

	return 0;
}
