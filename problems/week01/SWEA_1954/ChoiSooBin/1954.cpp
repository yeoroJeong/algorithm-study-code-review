/*
문제: 1954. 달팽이 숫자
링크: https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5PobmqAPoDFAUq
작성자: 최수빈
알고리즘: 구현, 시뮬레이션
핵심 아이디어: 우-> 하->좌 ->상 순으로 이동하도록 dx, dy 배열을 세팅. 직진하다가 배열 범위를 벗어나거나 이미 숫자가 채워진 칸을 만나면 방향을 전환(flag 변경)하며 숫자를 채워나간다.
시간 복잡도: O(N^2)
공간 복잡도: O(N^2)
다시 볼 포인트:
- 방향 전환 로직 최적화: 현재 4방향을 if-else로 일일이 하드코딩했는데, `(flag + 1) % 4`를 활용하면 코드를 획기적으로 줄일 수 있다. 
  (예: 다음 갈 곳(ny, nx)을 미리 계산해보고, 막혔다면 `flag = (flag + 1) % 4;` 로 방향만 바꿔주면 됨)
- 루프 조건 단순화: while(1)과 복잡한 break 조건 대신, `for (int sn = 1; sn <= n * n; sn++)` 처럼 N*N번만 반복하도록 짜면 무한 루프나 종료 조건을 신경 쓸 필요가 없다.
최적화 코드:
for (sn = 1; sn <= n * n; sn++) {
    snail[i][j] = sn;
    
    // 다음 이동할 좌표 미리 계산
    int next_i = i + dy[flag];
    int next_j = j + dx[flag];
    
    // 범위를 벗어나거나 이미 숫자가 있다면 방향 전환
    if (next_i < 0 || next_i >= n || next_j < 0 || next_j >= n || snail[next_i][next_j] != 0) {
        flag = (flag + 1) % 4; // 0->1->2->3->0 반복
    }
    
    // 실제 이동
    i += dy[flag];
    j += dx[flag];
}
*/

#include<iostream>

using namespace std;

int main(int argc, char** argv)
{
	int test_case;
	int T;

	freopen("1954_input.txt", "r", stdin);
	cin >> T;
	
	for (test_case = 1; test_case <= T; ++test_case)
	{
		int n;
		// 우 하 좌 상
		int flag = 0;
		int dx[4] = { 1,0,-1,0 };
		int dy[4] = { 0, 1, 0, -1 };
		int snail[101][101] = { {0, }, };
		int i = 0, j = 0;
		cin >> n;
		int sn = 0;
		while (1) {
			if (i < 0 || j < 0 || i >= n || j >= n) break;	// 범위를 벗어나거나
			sn++;
			snail[i][j] = sn;
			if (flag == 0) {	
				if (j + 1 < n && !snail[i][j + 1]) {	
					// 현재 방향(우) + 1 이 범위를 벗어나지도, 막혀있지도 않다
				}
				else if (j + 1 >= n || snail[i][j + 1]) {
					// 범위를 벗어나거나 막혀있음
					flag = 1;
				}
			}else if (flag == 1) {
				if (i + 1 < n && !snail[i + 1][j]) {
					// 현재 방향(우) + 1 이 범위를 벗어나지도, 막혀있지도 않다
				}
				else if (i + 1 >= n || snail[i + 1][j]) {
					// 범위를 벗어나거나 막혀있음
					flag = 2;
				}
			}else if (flag == 2) {
				if (j - 1 >= 0 && !snail[i][j - 1]) {
					// 현재 방향(우) + 1 이 범위를 벗어나지도, 막혀있지도 않다
				}
				else if (j - 1 < 0 || snail[i][j - 1]) {
					// 범위를 벗어나거나 막혀있음
					flag = 3;
				}
			}else if (flag == 3) {
				if (i - 1 >= 0 && !snail[i - 1][j]) {
					// 현재 방향(우) + 1 이 범위를 벗어나지도, 막혀있지도 않다
				}
				else if (i - 1 < 0 || snail[i - 1][j]) {
					// 범위를 벗어나거나 막혀있음
					flag = 0;
				}
			}
			i += dy[flag];
			j += dx[flag];
			if (snail[i][j]) break;	
			// 방향을 바꿨는데 이미 차있다(스네일은 다음 방향이 정해져 있기 때문에 이런 로직이 가능
		}
		cout << '#' << test_case << '\n';
		for (int a = 0; a < n; a++) {
			for (int b = 0; b < n; b++) {
				cout << snail[a][b] << ' ';
			}
			cout << '\n';
		}
			
	}
	return 0;
}