#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// 0:상 / 1:하 / 2:좌  / 3:우
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

int n, m, t;

struct Section
{
	int curBallNum = 0;				// 현재 이 칸에 있는 공의 번호
	vector<int> nextBallNums;	// 다음 턴에 이 칸에 올 공의 번호들.
};
struct Bead {
	int dir; // 현재 공 번호의 방향
	int weight = 0; // 무게
	pair<int, int> pos; // 위치
	bool isAvail = true; // 합쳐졌는지 안 합쳐졌는지 확인용
};

map<int, Bead> beads;

// 방향 바뀌는 것 또한 1초
// 1초에 1칸씩 움직임. 
bool isInRange(const int& x, const int& y)
{
	return (x >= 0 && x < n && y >= 0 && y < n);
}

// 우선 순위: 무게가 더 무거운 것, 같다면 번호가 더 큰 것
bool compare(const int& ballNum1, const int& ballNum2)
{
	return ballNum1 > ballNum2 ? true : false;
}

void moveBalls(vector<vector<Section>>& board)
{
	for (auto& b : beads)
	{
		// 이미 합쳐진 구슬이라면?
		if (!b.second.isAvail) continue;

		// 현재 이동 방향의 다음 위치
		int nextX = b.second.pos.first + dx[b.second.dir];
		int nextY = b.second.pos.second + dy[b.second.dir];

		// 다음 위치가 범위 밖이라면 
		// -> 방향을 바꿈, 위치 그대로, 다음 칸에 올 공의 번호에 추가. why? 다음 턴에도 공이 여기에 있으니까.
		if (!isInRange(nextX, nextY))
		{
			switch (b.second.dir)
			{
				case 0:
				{
					b.second.dir = 1;
					board[b.second.pos.first][b.second.pos.second].nextBallNums.push_back(b.first);
					break;
				}
				case 1:
				{
					b.second.dir = 0;
					board[b.second.pos.first][b.second.pos.second].nextBallNums.push_back(b.first);
					break;
				}
				case 2:
				{
					b.second.dir = 3;
					board[b.second.pos.first][b.second.pos.second].nextBallNums.push_back(b.first);
					break;
				}
				case 3:
				{
					b.second.dir = 2;
					board[b.second.pos.first][b.second.pos.second].nextBallNums.push_back(b.first);
					break;
				}
			}
		}
		// 범위 안이라면
		else
		{
			// 일단 다음 칸으로 위치를 옮기고, 다음 칸에 갈 수 있는 후보에 id값을 추가
			board[nextX][nextY].nextBallNums.push_back(b.first);
		}
	}
}

// 일단 이동을 시켰으니 점검
void decideBalls(vector<vector<Section>>& board)
{
	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			// 다음 칸에 오는 공이 하나도 없다면 건뛰.
			if (board[row][col].nextBallNums.size() < 1) continue;
			// 다음에 오는 공이 정확히 1개라면
			else if (board[row][col].nextBallNums.size() == 1)
			{
				// board의 새로운 공 반영
				int ballNum = board[row][col].nextBallNums[0];
				board[row][col].curBallNum = ballNum;

				// 공 정보 변경
				beads[ballNum].pos.first = row;
				beads[ballNum].pos.second = col;
			}
			// 2개 이상이라면?
			else
			{
				// 다음에 올 공의 번호를 우선순위에 맞게 정렬
				sort(board[row][col].nextBallNums.begin(), board[row][col].nextBallNums.end(), compare);

				// 맨 앞 공이 살아남는 공임.
				// 방향은 합쳐진 구슬들 중 가장 큰 번호가 매겨져 있는 구슬의 방향을 따르게 되고
				int liveBallNum = board[row][col].nextBallNums[0];

				// 나머지 공은 합쳐짐. + 공 상태 변경
				for (int i = 1; i < board[row][col].nextBallNums.size(); ++i)
				{
					int tmpBallNum = board[row][col].nextBallNums[i];
					// 이용 불가 표시
					beads[tmpBallNum].isAvail = false;
					beads[tmpBallNum].pos.first = -1;  // 방어적 처리
					beads[tmpBallNum].pos.second = -1; // 방어적 처리

					// 무게 합쳐줌
					beads[liveBallNum].weight += beads[tmpBallNum].weight;
				}
				// board에 정보 반영 / beads에도 정보 반영
				board[row][col].curBallNum = liveBallNum;
				beads[liveBallNum].pos.first = row;
				beads[liveBallNum].pos.second = col;

			}

			// 현재 칸에 대한 정보 갱신 완료 -> 초기화.
			board[row][col].nextBallNums.clear();
		}
	}
}
int main() {
	cin >> n >> m >> t;

	// 구슬 정보 입력
	for (int i = 1; i <= m; i++) {
		int x, y, weight;
		char dir;
		cin >> x >> y >> dir >> weight;
		beads[i].pos = { x-1, y-1 };
		beads[i].weight = weight;
		switch (dir) {
		case 'U':
		{
			beads[i].dir = 0;
			break;
		}
		case 'D':
		{
			beads[i].dir = 1;
			break;
		}
		case 'L':
		{
			beads[i].dir = 2;
			break;
		}
		case 'R':
		{
			beads[i].dir = 3;
			break;
		}

		}
	}

	Section s;
	s.nextBallNums.clear();
	vector<vector<Section>> board(n, vector<Section>(n, s));

	for (int time = 1; time <= t; ++time)
	{
		moveBalls(board);
		decideBalls(board);
	}

	int leftBalls = 0;
	int tmpMax = 0;

	for (auto& b : beads)
	{
		// 아직 안 합쳐지고 살아있다면
		if (b.second.isAvail)
		{
			// 제일 무거운 거 판별
			if (tmpMax < b.second.weight)
				tmpMax = b.second.weight;
			++leftBalls;
		}
	}

	cout << leftBalls << " " << tmpMax;
	return 0;
}
