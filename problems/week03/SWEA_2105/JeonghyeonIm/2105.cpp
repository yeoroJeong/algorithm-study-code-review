#include<iostream>
#include<vector>
using namespace std;

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N;
		cin >> N;
		vector<vector<int>> cafe(N, vector<int>(N, 0));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> cafe[i][j];
			}
		}
		int mx = -1; // 최댓값
		int dy[4] = { 1, 1, -1, -1 }; // 방향 우하 => 우상 => 좌상 => 좌하순
		int dx[4] = { 1, -1, -1, 1 };

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) { // i, j는 시작점

				for (int r = 1; r < N; r++) { // 우하, 좌상 길이
					for (int d = 1; d < N; d++) { // 우상, 좌하 길이
						vector<bool>visited(101, false); // 중복체크
						int cury = i, curx = j; // cury랑 curx는 이동
						int cnt = 0; // 디저트 개수
						bool check = true; // 중복, 범위 체크용

						int len[4] = { r,d,r,d };
						// 안쪽 for문용 순서대로 우하, 우상, 좌상, 좌하 길이
						for (int iter = 0; iter < 4 && check; iter++) {
							for (int step = 0; step < len[iter]; step++) {
								cury += dy[iter];
								curx += dx[iter];
								// 범위 체크
								if (cury < 0 || cury >= N || curx < 0 || curx >= N) {
									check = false;
									break;
								}
								// 중복 체크
								if (visited[cafe[cury][curx]] == true) {
									check = false;
									break;
								}
								visited[cafe[cury][curx]] = true;
								cnt++;
							}
						}
						// 중복체크 범위체크 모두 통과했고, 끝점이 시작점과 같으면
						if (check == true && cury == i && curx == j) {
							if (mx < cnt) {
								mx = cnt;
							}
						}
						// 업데이트
					}
				}
			}
		}

		cout << "#" << t << " " << mx << "\n";
	}
}