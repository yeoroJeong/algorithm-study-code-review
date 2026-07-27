#include <iostream>
#include<vector>
using namespace std;
int main()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N;
		cin >> N;
		
		vector<vector<int>> arr(N, vector<int>(N, 0));
		int dx[4] = { 1, 0, -1, 0 };
		int dy[4] = { 0, 1, 0, -1 };

		int idx = 0;
		int curx = 0;
		int cury = 0;
		for (int i = 1; i <= N * N; i++) {
			arr[cury][curx] = i;
			int nx = curx + dx[idx];
			int ny = cury + dy[idx];
			if (nx < 0 || nx >= N || ny < 0 || ny >= N || arr[ny][nx] != 0) {
				idx++;
				idx %= 4;
				nx = curx + dx[idx];
				ny = cury + dy[idx];
			}
			curx = nx;
			cury = ny;
		}

		cout << "#" << t << "\n";
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << arr[i][j] << " ";
			}
			cout << "\n";
		}
	}
	return 0;
}