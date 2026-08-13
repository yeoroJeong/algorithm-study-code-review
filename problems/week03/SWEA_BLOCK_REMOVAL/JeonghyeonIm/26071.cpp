#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N;
vector<int> board;
vector<vector<int>> dp;
vector<vector<bool>> done; // 지금 블록 깻는지 체크하기 위함

int solve(int l, int r) {
	if (l > r) { // 빈 구간 깰거 없음 0점
		return 0;
	}
	if (done[l][r]) { // 이미 계산했던 구간이면 저장된 값 재사용
		return dp[l][r];
	}
	done[l][r] = true; // 체크

	bool left = (l - 1 >= 0); // 왼쪽 있냐
	bool right = (r + 1 < N); // 오른쪽 있냐
	int best = 0;

	// k => 이 구간에서 가장 마지막에 깨는 블록
	for (int k = l; k <= r; k++) {
		int score;
		if (left && right) { // 좌우 이웃블록 곱만큼 점수 얻음
			score = board[l - 1] * board[r + 1];
		}
		else if (left) { // 둘중 하나만 있는경우 그만큼 점수 얻음
			score = board[l - 1];
		}
		else if (right) {
			score = board[r + 1];
		}
		else { // 이웃 없으면, 깨진 블록만큼 점수 얻음
			score = board[k];
		}
		best = max(best, solve(l, k - 1) + solve(k + 1, r) + score);
		// k를 마지막에 깨기 전의 좌우 구간은 모두 다 깨진 상태임
	}
	dp[l][r] = best;
	return best;
}


int main()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N;
		board.assign(N, 0);
		for (int i = 0; i < N; i++) {
			cin >> board[i];
		}
		dp.assign(N, vector<int>(N, 0));
		done.assign(N, vector<bool>(N, false));


		cout << "#" << t << " " << solve(0, N - 1) << "\n";
	}
}