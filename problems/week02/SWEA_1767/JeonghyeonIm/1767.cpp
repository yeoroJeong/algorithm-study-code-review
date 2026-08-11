#include<iostream>
#include<vector>

using namespace std;

int N;
vector<vector<int>> board;
vector<pair<int, int>> c; // cell 위치
int mxcell; // 최대 연결 cell
int mnwire; // 최대 연결 cell 일때 최소 연결 wire

int dx[4] = { 0, 0, 1, -1 };
int dy[4] = { 1, -1, 0, 0 };

void back(int curw, int curc, int s) {
	if (s == c.size()) {
		if (mxcell < curc) { // 최대 연결 cell 업데이트 일 경우
			mxcell = curc;
			mnwire = curw;
		}
		else if (mxcell == curc) { // 최대 연결 cell 같은데
			if (mnwire > curw) { // 최소 wire 수를 업데이트 해야할 경우
				mnwire = curw;
			}
		}
		return;
	}

	if (curc + (c.size() - s) < mxcell) { // 지금부터 아무리해도 mxcell못넘을 경우 => 빼버려
		return;
	}

	int cury = c[s].first; // 현재 cell y좌표
	int curx = c[s].second; // 현재 cell x좌표

	// 1. 가장자리일경우 전선 연결 안해도됨
	if (cury == 0 || cury == N - 1 || curx == 0 || curx == N - 1) {
		back(curw, curc + 1, s + 1);
		return;
	}

	back(curw, curc, s + 1); // 2. 이 코어 연결 안함

	for (int d = 0; d < 4; d++) { // 3. 4방향 연결가능 시 연결
		bool check = true; // 전선 연결 가능여부
		int ty = cury + dy[d];
		int tx = curx + dx[d];
		int tc = curc;
		int tw = curw;

		vector<vector<int>> temp = board;
		while (ty >= 0 && ty < N && tx >= 0 && tx < N) {
			if (board[ty][tx] >= 1) { // 전선 연결 불가
				check = false;
				break;
			}
			board[ty][tx] = 2;
			ty += dy[d];
			tx += dx[d];
			tw++;
		}

		if (!check) { // 전선 연결 불가일 경우
			board = temp;
			continue;
		}
		else { // 전선 연결 가능일 경우
			back(tw, tc + 1, s + 1);
			board = temp;
		}
	}
}

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N;
		board.assign(N, vector<int>(N, 0));
		c.resize(0);
		mxcell = 0;
		mnwire = 1e9;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
				if (board[i][j] == 1) {
					c.push_back({ i, j });
				}
			}
		}
		back(0, 0, 0);

		cout << "#" << t << " " << mnwire << "\n";
	}
	return 0;
}