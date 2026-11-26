#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 우선순위가 상 하 좌 우 순으로
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

int n, m, t;
struct Ball {
	int id = 0;
	pair<int, int> curPos; // 현재 위치
	pair<int, int> nextPos = { 0, 0 }; // 다음 위치
	bool isCanceled = false; // 충돌되어 파괴되었는지
};

struct Section {
	int value = 0;
	vector<int> ballIds;
};

unordered_map<int, Ball> balls;

bool compare(const pair<int, int> &a, const pair<int, int> &b) {
	if (a.first == b.first)
		return a.second < b.second;
	return a.first > b.first;
}

pair<int, int> maxValue(const pair<int, int> &up, const pair<int, int> &down, const pair<int, int>& left, const pair<int, int>& right) {
	vector<pair<int, int>> sorted;
	sorted.push_back(up);
	sorted.push_back(down);
	sorted.push_back(left);
	sorted.push_back(right);

	sort(sorted.begin(), sorted.end(), compare);
	return sorted[0];
}
void moveBalls(vector<vector<Section>>& board) {
	for (auto& b : balls) {
		// 충돌되어 사라진 공이면 건뛰
		if (b.second.isCanceled) {
			continue;
		}
		pair<int, int> upValue = { -1, 0 };
		pair<int, int> downValue = { -1, 1 };
		pair<int, int> leftValue = { -1, 2 };
		pair<int, int> rightValue = { -1, 3 };

		// 다음 위치가 범위 이내일 때 갱신.
		int nextUpx = b.second.curPos.first + dx[0];
		int nextUpy = b.second.curPos.second + dy[0];
		if (nextUpx >= 0 && nextUpx < n && nextUpy >= 0 && nextUpy < n)
			upValue.first = board[nextUpx][nextUpy].value;

		int downUpx = b.second.curPos.first + dx[1];
		int downUpy = b.second.curPos.second + dy[1];
		if (downUpx >= 0 && downUpx < n && downUpy >= 0 && downUpy < n)
			downValue.first = board[downUpx][downUpy].value;

		int leftUpx = b.second.curPos.first + dx[2];
		int leftUpy = b.second.curPos.second + dy[2];
		if (leftUpx >= 0 && leftUpx < n && leftUpy >= 0 && leftUpy < n)
			leftValue.first = board[leftUpx][leftUpy].value;

		int rightUpx = b.second.curPos.first + dx[3];
		int rightUpy = b.second.curPos.second + dy[3];
		if (rightUpx >= 0 && rightUpx < n && rightUpy >= 0 && rightUpy < n)
			rightValue.first = board[rightUpx][rightUpy].value;

		// 어떤 방향이 가장 큰 값을 가지는지
		pair<int, int> maxVal = maxValue(upValue, downValue, leftValue, rightValue);

		// 다음 위치 갱신
		b.second.nextPos.first = b.second.curPos.first + dx[maxVal.second];
		b.second.nextPos.second = b.second.curPos.second + dy[maxVal.second];

		// 다음 위치를 현재 위치로 갱신.
		b.second.curPos = b.second.nextPos;

		int tmpX = b.second.nextPos.first;
		int tmpY = b.second.nextPos.second;

		// 보드의 다음 위치에 들어갈 공의 id를 추가.
		board[tmpX][tmpY].ballIds.push_back(b.first);

	}

}

// 모든 공의 다음 위치를 정했다면, 충돌 여부 확인
void checkCanceledBall(vector<vector<Section>>& board) {
	for (int row = 0; row < n; ++row) {
		for (int col = 0; col < n; ++col) {
			// 2개 이상의 공이 몰림.
			if (board[row][col].ballIds.size() >= 2) {
				for (auto &id : board[row][col].ballIds) {
					// 충돌 true 표시
					balls[id].isCanceled = true;
				}
			}
		}
	}
}

// 다음 턴 공 초기화
void initializeBallIds(vector<vector<Section>>& board) {
	for (int row = 0; row < n; ++row) {
		for (int col = 0; col < n; ++col) {
			if (!board[row][col].ballIds.empty()) {
				board[row][col].ballIds.clear();
			}
		}
	}
}
int main() {
	cin >> n >> m >> t;
	Section s;
	vector<vector<Section>> board(n, vector<Section>(n, s));
	balls.clear();

	int result = 0;

	// 격자 정보 받기
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j].value;
			board[i][j].ballIds.clear(); // 초기화
		}
	}

	// 구슬의 초기 정보
	for (int i = 1; i <= m; i++) {
		balls[i].id = i;
		int row, col;
		cin >> row >> col;
		balls[i].curPos.first = row - 1;
		balls[i].curPos.second = col - 1;
	}
	// Please write your code here.
	for (int time = 1; time <= t; ++time) {
		moveBalls(board);
		checkCanceledBall(board);
		initializeBallIds(board);
	}

	for (auto &b : balls) {
		if (!b.second.isCanceled) result++;
	}
	cout << result;
	return 0;
}
