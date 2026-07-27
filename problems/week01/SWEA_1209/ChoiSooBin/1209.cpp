//
/*
문제: SWEA 1209.[S / W 문제해결 기본] 2일차 - Sum
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV13_BWKACUCFAYh
작성자: 최수빈
알고리즘: 구현, 누적합(?)
핵심 아이디어: 입력을 받으면서 바로 ROW, COLUMN, DIAGONAL1, 2를 계산해 100*100을 모두 저장하지 않고 SUM의 최댓값을 구한다.
시간 복잡도: O(n^2)
공간 복잡도: O(N)
다시 볼 포인트: 
- ROW의 경우 매 줄 TOTAL_ROW_SUM과 같은 변수만 사용하고 바로 바로 MAX를 업데이트 하면 공간을 더 최적화할 수 있다.
[1][1] 시작
- 우하향 대각선 : i == j
- 우상향 대각선: i + j = n + 1
[0][0] 시작
- 우상향 대각선: i+j = n - 1
*/

#include<iostream>
using namespace std;

int main(int argc, char** argv)
{
	ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	int test_case;
	freopen("1209_input.txt", "r", stdin);

	for (test_case = 1; test_case <= 10; ++test_case)
	{
		int test_num;
		int mx = 0, tmp = 0;
		int row[110] = { 0, };
		int col[110] = { 0, };
		int diag1 = 0, diag2 = 0;
		cin >> test_num;
		for (int i = 1; i <= 100; i++) {
			for (int j = 1; j <= 100; j++) {
				cin >> tmp;
				row[i] += tmp;
				col[j] += tmp;
				if (i == j) diag1 += tmp;
				if ((i + j) == 101) diag2 += tmp;
			}
		}
		for (int i = 1; i <= 100; i++) {
			if (mx < row[i]) mx = row[i];
			if (mx < col[i]) mx = col[i];
		}
		if (mx < diag1) mx = diag1;
		if (mx < diag2) mx = diag2;

		cout << '#' << test_num << ' ' << mx << '\n';
	}
	return 0;
}
