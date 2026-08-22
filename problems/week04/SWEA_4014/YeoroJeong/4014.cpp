#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <climits>
#include <array>
#include <cstring>
#include <queue>
#include <deque>

using namespace std;

int answer;
int N, X;

// direction = 0 : 오른쪽 이동
// direction = 1 : 아래쪽 이동
int dx[4] = { 0, 1 };
int dy[4] = { 1, 0 };

vector<vector<int>> board;


/*
 * idx번째 행 또는 열에 활주로를 건설할 수 있는지 확인
 *
 * idx       : 확인할 행 또는 열의 번호
 * direction : 0이면 가로 방향, 1이면 세로 방향
 */
void is_ok(int idx, int direction) {

	int x = 0;
	int y = 0;

	// 가로 방향 탐색인 경우 idx번째 행에서 시작
	if (direction == 0) {
		x = idx;
	}
	// 세로 방향 탐색인 경우 idx번째 열에서 시작
	else {
		y = idx;
	}

	// 현재 기준 높이
	int cur_height = board[x][y];

	/*
	 * 낮아지는 경사로가 설치된 위치 저장
	 *
	 * 이후 오르막 경사로를 설치할 때
	 * 이미 사용된 칸과 겹치는지 확인하기 위해 사용
	 */
	set<int> down;


	// 현재 행 또는 열을 끝까지 탐색
	while (true) {

		// 진행 방향으로 한 칸 이동
		x += dx[direction];
		y += dy[direction];

		// 맵 끝까지 정상적으로 확인한 경우 종료
		if (x >= N || y >= N)
			break;


		/*
		 * 높이 차이가 2 이상이면
		 * 경사로를 설치할 수 없으므로 활주로 건설 불가능
		 */
		if (abs(cur_height - board[x][y]) > 1)
			return;


		/*
		 * 현재 높이보다 다음 칸이 1 낮은 경우
		 * → 내려가는 경사로 설치
		 */
		else if (cur_height - board[x][y] == 1) {

			int temp_height = board[x][y];

			// 경사로가 사용한 첫 번째 칸 기록
			down.insert(x + y);


			/*
			 * 현재 위치부터 X칸 동안
			 * 같은 높이가 유지되는지 확인
			 */
			for (int i = 1; i < X; i++) {

				x += dx[direction];
				y += dy[direction];

				// 경사로가 설치된 위치 기록
				down.insert(x + y);

				// 경사로 길이가 맵 밖으로 넘어가는 경우
				if (x == N || y == N)
					return;

				// 경사로가 놓일 칸의 높이가 다르면 설치 불가능
				if (temp_height != board[x][y])
					return;
			}

			// 경사로 설치 후 현재 높이 갱신
			cur_height = temp_height;
		}


		/*
		 * 현재 높이보다 다음 칸이 1 높은 경우
		 * → 올라가는 경사로 설치
		 */
		else if (cur_height - board[x][y] == -1) {

			int temp_height = board[x][y];

			// 실제 탐색 위치는 유지하고,
			// 경사로 설치 가능 여부 확인용 좌표 사용
			int temp_x = x;
			int temp_y = y;


			/*
			 * 현재 위치의 이전 X칸이
			 * 경사로 설치 조건을 만족하는지 확인
			 */
			for (int i = 0; i < X; i++) {

				temp_x -= dx[direction];
				temp_y -= dy[direction];

				// 경사로가 맵 밖으로 넘어가는 경우
				if (temp_x < 0 || temp_y < 0)
					return;


				/*
				 * 이전 칸의 높이가 현재 기준 높이와 다르거나
				 * 이미 내려가는 경사로가 설치된 칸이면 설치 불가능
				 */
				if (cur_height != board[temp_x][temp_y] ||
					down.find(temp_x + temp_y) != down.end())
					return;
			}

			// 경사로 설치 후 현재 높이 갱신
			cur_height = temp_height;
		}
	}


	// 끝까지 조건을 만족했다면 활주로 건설 가능
	answer++;
}


int main()
{
	int T;
	cin >> T;

	for (int test_case = 1; test_case <= T; test_case++)
	{
		cin >> N >> X;

		answer = 0;

		board.resize(N, vector<int>(N));


		// 지형 높이 입력
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
			}
		}


		/*
		 * 모든 행과 열을 각각 확인
		 *
		 * direction = 0 : i번째 행
		 * direction = 1 : i번째 열
		 */
		for (int i = 0; i < N; i++) {
			is_ok(i, 0);
			is_ok(i, 1);
		}


		cout << "#" << test_case << " " << answer << "\n";


		// 다음 테스트 케이스를 위해 초기화
		board.clear();
	}

	return 0;
}