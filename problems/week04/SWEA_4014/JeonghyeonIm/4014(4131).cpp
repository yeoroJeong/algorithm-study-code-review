#include<iostream>
#include<vector>

using namespace std;

//사실상 23~60, 62~99줄은 같은로직입니다. 가로방향체크하냐 세로방향체크하냐 차이

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, X;
		cin >> N >> X;
		vector<vector<int>> board(N, vector<int>(N, 0));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
			}
		}

		int cnt = 0;

		for (int i = 0; i < N; i++) { // 가로방향 체크
			bool ck = true; // 활주로 되냐
			vector<bool> visited(N, false); // 경사로 놓은 곳 체크

			for (int j = 0; j < N - 1 && ck; j++) {
				if (board[i][j] != board[i][j + 1]) { // 높이가 다르다면
					if (board[i][j] + 1 == board[i][j + 1]) {
						// 지나왔던 방향보다 1만큼 높으면
						for (int c = j; c >= j - (X - 1); c--) {
							if (c < 0 || visited[c] == true) {
								// 만약 이미 경사로 놓은곳이거나 못놓는곳이면
								ck = false;
								break; // 탈락
							}
							visited[c] = true;
						}
					}
					else if (board[i][j] == board[i][j + 1] + 1) {
						// 지나왔던 방향의 다음방향이 1만큼 낮으면
						for (int c = j + 1; c <= j + X; c++) {
							if (c >= N || visited[c] == true) {
								// 만약 이미 경사로 놓은곳이거나 못놓는곳이면
								ck = false;
								break; // 탈락
							}
							visited[c] = true;
						}
					}
					else {
						ck = false;
					}
				}
			}
			if (ck) {
				cnt++;
			}
			visited.assign(N, false);
		}

		for (int i = 0; i < N; i++) { // 세로방향 체크
			bool ck = true; // 활주로 되냐
			vector<bool> visited(N, false); // 경사로 놓은 곳 체크
				for (int j = 0; j < N - 1 && ck; j++) { // 세로방향 체크
				if (board[j][i] != board[j + 1][i]) { // 높이가 다르다면
					if (board[j][i] + 1 == board[j + 1][i]) {
						// 지나왔던 방향보다 1만큼 높으면
						for (int c = j; c >= j - (X - 1); c--) {
							if (c < 0 || visited[c] == true) {
								// 만약 이미 경사로 놓은곳이거나 못놓는곳이면
								ck = false;
								break; // 탈락
							}
							visited[c] = true;
						}
					}
					else if (board[j][i] == board[j + 1][i] + 1) {
						// 지나왔던 방향의 다음방향이 1만큼 낮으면
						for (int c = j + 1; c <= j + X; c++) {
							if (c >= N || visited[c] == true) {
								// 만약 이미 경사로 놓은곳이거나 못놓는곳이면
								ck = false;
								break; // 탈락
							}
							visited[c] = true;
						}
					}
					else {
						ck = false;
						break;
					}
				}
			}
			if (ck) {
				cnt++;
			}
			visited.assign(N, false);
		}

		cout << "#" << t << " " << cnt << "\n";
	}
	return 0;
}