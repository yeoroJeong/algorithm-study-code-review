#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

/*
출발위치 0,0이고 출발 시 현재 바라보고 있는 방향으로 전진(초기방향 오른쪽)
후진 불가능, 방향 바꾸려면 오른쪽 90도 회전
=>
1. 오른쪽이면 아래로만 회전가능
2. 아래면 왼쪽으로만 회전가능
3. 왼쪽이면 위로만 회전 가능
4. 위면 오른쪽으로만 회전가능
보석 위치 도달시, 수집함과 동시에 회전 가능
제자리 여러번 연속 회전 불가능, 오직 한번만 회전가능
모든 보석을 번호 순서대로 수집 시, 필요한 최소 회전 횟수 출력
*/

struct node {
	int cury;
	int curx;
	int target; // 목표 보석 번호
	int d; // 현재 방향(0:오, 1:하, 2:왼, 3:상)
	int cnt; // 지금부터 목표 보석 번호까지 가는데 회전 횟수
};


int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		int N, M;
		cin >> N;
		M = 0;
		vector<vector<int>> board(N, vector<int>(N));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
				M = max(M, board[i][j]); // M값 찾기
			}
		}
		// 입력 끝

		int dx[4] = { 1, 0, -1, 0 }; // 0:오, 1:하, 2:왼, 3:상
		int dy[4] = { 0, 1, 0, -1 };
		
		queue<node> q;
		q.push({ 0, 0, 1, 0, 0 });

		int sum = 0;
		while (!q.empty()) {
			node cur = q.front();
			q.pop();
			int cury = cur.cury;
			int curx = cur.curx;
			int target = cur.target;
			int d = cur.d;
			int cnt = cur.cnt;


			if (target == board[cury][curx] && target == M) { // 목표 보석 찾았고, M까지 찾았으면
				sum += cnt;
				break; // 끝
			}
			else if (target == board[cury][curx]) { // 그냥 목표 보석 찾았으면
				q = queue<node>(); // queue초기화하고
				sum += cnt;
				q.push({ cury, curx, target + 1, d, 0 }); // target 다음꺼랑 count 초기화 하고
				continue; // 다시 실행
			}
			
			// 하나의 칸에서 할 수 있는 일 1(그냥 가기)
			int ny = cury + dy[d];
			int nx = curx + dx[d];
			if (nx >= 0 && nx < N && ny >= 0 && ny < N) { // 범위 체크
				q.push({ ny, nx, target, d, cnt });
			}
			// 하나의 칸에서 할 수 있는 일 2(방향 바꿔 가기)
			d += 1;
			d %= 4;
			ny = cury + dy[d];
			nx = curx + dx[d];
			if (nx >= 0 && nx < N && ny >= 0 && ny < N) { // 범위 체크
				q.push({ ny, nx, target, d, cnt + 1 });
			}
		}

		cout << "#" << t << " " << sum << "\n";
	}


	return 0;
}
