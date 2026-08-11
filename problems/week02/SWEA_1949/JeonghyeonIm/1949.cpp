#include<iostream>
#include<vector>
#include<queue>
using namespace std;

struct node {
	int cury;
	int curx;
	int hei; // 현재 높이
	int cnt; // 현재까지 등산로 길이
	bool check; // 등산로 깍았냐 안깍았냐
	vector<pair<int, int>> visited; // 지나온길 체크
};

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, K;
		cin >> N >> K;
		vector<vector<int>> board(N, vector<int>(N, 0));
		int mxnum = 0;
		queue<node> q;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
				if (board[i][j] > mxnum) {
					mxnum = board[i][j];
					queue<node>().swap(q);
					vector<pair<int, int>> visited;
					visited.push_back({ i, j });
					q.push({ i, j, mxnum, 1, false, visited });
				}
				else if (board[i][j] == mxnum) {
					vector<pair<int, int>> visited;
					visited.push_back({ i, j });
					q.push({ i, j, mxnum, 1, false, visited });
				}
			}
		}
		//초기셋팅 끝 => 입력 다 받고 제일 높은곳 queue에 투입//
		int mx = 0;

		int dx[4] = { 0, 0, 1, -1 };
		int dy[4] = { 1, -1, 0, 0 };

		while (!q.empty()) {
			node cur = q.front();
			q.pop();
			if (mx < cur.cnt) {
				mx = cur.cnt;
			}

			for (int i = 0; i < 4; i++) {
				int ny = cur.cury + dy[i];
				int nx = cur.curx + dx[i];
				if (nx < 0 || nx >= N || ny < 0 || ny >= N) {
					continue;
				}
				// 이미 지나온길이면 안가
				bool cont = false;
				for (pair<int, int> ck : cur.visited) {
					if (ck.first == ny && ck.second == nx) {
						cont = true;
						break;
					}
				}
				if (cont) {
					continue;
				}
				// 주요로직
				vector<pair<int, int>> v = cur.visited;
				if (board[ny][nx] >= cur.hei && board[ny][nx] - K < cur.hei) { // 그냥은 못가는데 공사하면 갈 수 있는경우 => 공사해
					if (cur.check == true) { // 근데 이미 공사한적 있으면 끝
						continue;
					}
					
					v.push_back({ ny, nx });
					q.push({ ny, nx, cur.hei - 1, cur.cnt + 1, true,  v}); // cur.hei - 1로 하는 이유는 이게 최적임
				}
				else if (board[ny][nx] >= cur.hei) { // 그냥도 못가는데 공사해도 못가는 경우 => 끝
					continue;
				}
				else { // 그냥 갈 수 있는 경우
					v.push_back({ ny, nx });
					q.push({ ny, nx, board[ny][nx], cur.cnt + 1, cur.check, v});
				}
				
			}
		}

		cout << "#" << t << " " << mx << "\n";
	}



	return 0;
}