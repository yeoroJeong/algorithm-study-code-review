#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

using namespace std;

int main() {
	int T;
	cin >> T;

	for (int t = 1; t <= T; t++) {
		int day, month1, month3, year;
		cin >> day >> month1 >> month3 >> year;

		vector<int> input(13, 0);
		for (int i = 1; i <= 12; i++) {
			cin >> input[i];
		}
		vector<int> dp(13, 0); // dp[n]: n달까지 봤을때 최소비용

		for (int i = 1; i <= 12; i++) { // 1일 이용권 및 1달 이용권, 3달 이용권 적용
			dp[i] = min(dp[i - 1] + input[i] * day, dp[i - 1] + month1);
			if (i >= 3) {
				dp[i] = min(dp[i - 3] + month3, dp[i]);
			}
		}
		
		if (dp[12] < year) { // 1년 이용권이랑 비교
			cout << "#" << t << " " << dp[12] << "\n";
		}
		else {
			cout << "#" << t << " " << year << "\n";
		}
	}
	return 0;
}