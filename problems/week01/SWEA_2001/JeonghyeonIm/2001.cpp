#include <iostream>
#include<vector>
using namespace std;
int main()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, M;
		cin >> N >> M;
		vector<vector<int>> board(N, vector<int>(N, 0));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
			}
		}
		int mx = 0;

		for (int i = 0; i <= N - M; i++) {
			for (int j = 0; j <= N - M; j++) {
				int sum = 0;
				for (int dy = i; dy < i + M; dy++) {
					for (int dx = j; dx < j + M; dx++) {
						if (dx < 0 || dx >= N || dy < 0 || dy >= N) {
							continue;
						}
						sum += board[dy][dx];
					}
				}
				if (mx < sum) {
					mx = sum;
				}
			}
		}

		cout << "#" << t << " " << mx <<  "\n";
	}
	return 0;
}