#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int N, M, K;
int totalT = 0; // 경과한 시간

// 상 하 좌 우
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

// 방향, 거리
queue<pair<char, int>> orders;

// 뱀 관련 정보
vector<pair<int, int>> snakePos; // 뱀의 전체 좌표 (머리 포함)

bool isInRange(const int& x, const int& y)
{
	return (x >= 0 && x < N && y >= 0 && y < N);
}

// 특히 새로 이동한 머리 자리가 이번 이동으로 사라지는 꼬리 자리와 같더라도
// 꼬리가 먼저 빠지는 것으로 간주하여 몸이 겹치지 않은 것으로 처리
bool move(vector<vector<int>>& board, int dir, int len)
{
	// 움직일 거리 만큼
	for (int curLen = 1; curLen <= len; ++curLen)
	{
		// 충돌한 이동도 1초가 경과한 것으로 처리
		totalT++;

		int nextX = snakePos[0].first + dx[dir];
		int nextY = snakePos[0].second + dy[dir];

		if (!isInRange(nextX, nextY))
		{
			return false;
		}

		// 한 칸 이동에서는 최대 한 개의 사과만 먹을 수 있음
		bool ateApple = (board[nextX][nextY] == 1);

		// 사과를 먹지 않으면 기존 꼬리가 빠지므로 충돌 검사에서 제외
		// 사과를 먹으면 기존 꼬리가 남으므로 전체 몸을 검사
		int checkLen = (int)snakePos.size();

		if (!ateApple)
		{
			checkLen--;
		}

		// 머리는 이동하므로 기존 머리 다음 마디부터 검사
		for (int i = 1; i < checkLen; ++i)
		{
			if (snakePos[i].first == nextX &&
				snakePos[i].second == nextY)
			{
				return false;
			}
		}

		// 기존 머리 위치를 다음 마디에 넘기기 위해 저장
		int prevX = snakePos[0].first;
		int prevY = snakePos[0].second;

		// 머리 이동
		snakePos[0] = { nextX, nextY };

		// 뱀의 전체 길이 만큼.
		// 머리는 이동했으므로 몸부터 앞 마디의 이전 위치로 이동
		for (int i = 1; i < (int)snakePos.size(); ++i)
		{
			// 현재 마디의 이동 전 위치 저장
			int curX = snakePos[i].first;
			int curY = snakePos[i].second;

			snakePos[i] = { prevX, prevY };

			// 다음 마디에 넘겨줄 위치 갱신
			prevX = curX;
			prevY = curY;
		}

		// 꼬리를 어느 방향으로 추가하는지?
		// 반복문이 끝난 뒤 prevX, prevY가 이동 전 꼬리 위치
		// 사과를 먹었다면 해당 위치를 추가하고, 먹은 사과 자리는 0으로
		if (ateApple)
		{
			snakePos.push_back({ prevX, prevY });
			board[nextX][nextY] = 0;
		}
	}

	return true;
}

int main()
{
	cin >> N >> M >> K;

	vector<vector<int>> board(N, vector<int>(N, 0));

	for (int i = 0; i < M; i++)
	{
		int tmpX, tmpY;
		cin >> tmpX >> tmpY;
		board[tmpX - 1][tmpY - 1] = 1;
	}

	for (int i = 0; i < K; i++)
	{
		char dir;
		int moveLen;
		cin >> dir >> moveLen;
		orders.push({ dir, moveLen });
	}

	// 초기에는 전체 중 대가리만 있음.
	snakePos.push_back({ 0, 0 });

	while (!orders.empty())
	{
		char tmpDir = orders.front().first;
		int len = orders.front().second;
		orders.pop();

		int dir; // char에서 변환한 방향

		if (tmpDir == 'U')
			dir = 0;
		else if (tmpDir == 'D')
			dir = 1;
		else if (tmpDir == 'L')
			dir = 2;
		else
			dir = 3;

		// 이동
		// 벽이나 몸에 충돌하면 남은 명령도 실행하지 않음
		if (!move(board, dir, len))
		{
			break;
		}
	}

	cout << totalT << '\n';

	return 0;
}