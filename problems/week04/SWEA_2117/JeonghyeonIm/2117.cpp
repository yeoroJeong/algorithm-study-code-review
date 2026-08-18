#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
using namespace std;

int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, M;
		cin >> N >> M;
		vector<vector<int>> house(N, vector<int>(N, 0));
		vector<pair<int, int>> homes; // 집 좌표 저장

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> house[i][j];
				if (house[i][j] == 1) {
					homes.push_back({ i, j });
				}
			}
		}
		// 입력 끝~

		int K = (N * 2) - 1;
		int r = K - 1; // 마름모 반경

		int mx = 0;

		for (int k = (N * 2) - 1; k >= 1; k--) {
			int cost = (k*k) + ((k - 1)*(k - 1)); // 운영 비용
			int r = k - 1; // 마름모 반경

			for (int cy = 0; cy < N; cy++) {
				for (int cx = 0; cx < N ; cx++) { // cx, cy는 마름모의 중심점
					int cnt = 0; // 집 수 카운트

					for (auto x : homes) {
						int row = x.first; // 집 좌표의 y값
						int col = x.second; // 집 좌표의 x값
						if (abs(cy - row) + abs(cx - col) <= r) { // 마름모 판정
							cnt++;
						}
					}

					if (cost <= cnt * M) { // 마름모 체크 후 cost체크
						mx = max(mx, cnt);
					}
				}
			}
		}

		cout << "#" << t << " " << mx << '\n';
	}
}