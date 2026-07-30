/*
문제: SWEA 2001 파리 퇴치
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PzOCKAigDFAUq
작성자: 최수빈
알고리즘: 누적합
핵심 아이디어: 
포함-배제의 원리를 이용해 2차원 배열의 누적합을 미리 계산한다. 
-> sum[b][b] - map[a-1][b] - map[b][a-1] + map[a-1][a-1] 공식을 사용해 O(1)의 연산으로 m*m 구간의 합을 도출해 최댓값을 갱신한다.
시간 복잡도: O(n^2)
공간 복잡도: O(N^2)
다시 볼 포인트: 처음 배열을 [0][0]에서 시작하는 것이 아닌 [1][1]로 시작하면 i-1이나 j-1을 할 때 인덱스가 음수가 되지 않아 복잡한 if-else 분기문을 없앨 수 있다.
*/

#include<iostream>
#include<cmath>
#define FLY_MAX 987654321;
using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T;
	ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	// freopen("2001_input.txt", "r", stdin);
	cin >> T;
	
	for (test_case = 1; test_case <= T; ++test_case)
	{
		int n = 0, m = 0, mx = 0;
		int tmp, cmp = 0;
		cin >> n >> m;
		int sum[20][20] = { {0, }, };	// 누적 합
		
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cin >> tmp;
				if (i > 0 && j > 0) {
					sum[i][j] = tmp + sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
				}
				else if (i > 0 && j == 0) {
					sum[i][j] = tmp + sum[i - 1][j];
				}
				else if (i == 0 && j > 0){	// i == 0 && j > 0
					sum[i][j] = tmp + sum[i][j - 1];
				}
				else {
					sum[0][0] = tmp;
				}
			}
		}	// 누적합
		mx = sum[m - 1][m - 1];
		for (int i = m - 1; i < n; i++) {
			for (int j = m - 1; j < n; j++) {
				if (i-m >= 0 && j-m >= 0) {
					cmp = sum[i][j] - sum[i][j - m] - sum[i - m][j] + sum[i - m][j - m];
				}
				else if ((i - m) < 0 && (j - m) >= 0) {
					cmp = sum[i][j] - sum[i][j - m];
				}
				else if ((j - m) < 0 && (i - m) >= 0){
					cmp = sum[i][j] - sum[i - m][j];
				}
				if (mx < cmp) mx = cmp;
			}
		}
		cout << '#' << test_case << ' ' << mx << '\n';
	}
	return 0;
}


// map[a][a] ~ map[b][b] 의 값은
// sum[b][b] - map[a-1][b] - map[b][a-1] + map[a-1][a-1]
// m*m 파리채일 때 for문을 돌 때 [m-1][m-1]부터 시작
// sum[i][j] - sum[i][j-m] - sum[i-m][j] + sum[i-m][j-m]	단 i-m < 0이면 예외처리