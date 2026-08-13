#include <iostream>
#include <deque>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <climits>
#include <array>
#include <cstring>

using namespace std;

int answer;
int N, M;

// 우, 하, 좌, 상 순서
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };

vector<vector<int>> board;

// dest[i] : i번째 보석의 위치
vector<pair<int, int>> dest;


/*
 * 현재 위치에서 현재 방향으로 이동하며
 * dest_idx번째 보석을 순서대로 수집하는 함수
 *
 * x, y       : 현재 로봇 위치
 * dest_idx   : 현재 찾아야 하는 보석 번호
 * direction  : 현재 진행 방향
 * rotate_cnt : 지금까지 회전한 횟수
 */
void solv(int x, int y, int dest_idx, int direction, int rotate_cnt) {

	// 모든 보석을 수집한 경우 최소 회전 횟수 갱신
	if (dest_idx > M) {
		answer = min(answer, rotate_cnt);
		return;
	}

	// 이미 구한 최소 회전 횟수보다 크거나 같으면 더 탐색할 필요 없음
	if (rotate_cnt >= answer)
		return;

	// 현재 찾아야 하는 보석의 위치
	int dest_x = dest[dest_idx].first;
	int dest_y = dest[dest_idx].second;

	// 현재 방향으로 한 칸 이동한 위치
	int temp_x = x + dx[direction];
	int temp_y = y + dy[direction];

	// 시작하자마자 맵 밖으로 나가는 경우 탐색 종료
	if (temp_x < 0 || temp_x >= N || temp_y < 0 || temp_y >= N)
		return;


	/*
	 * 현재 방향으로 계속 직진하면서
	 * 1. 목적지 보석을 만나는 경우
	 * 2. 회전이 가능한 위치를 만나는 경우
	 * 3. 맵 끝에 도달한 경우
	 * 를 처리
	 */
	while (true) {

		// 맵 밖으로 나간 경우
		if (temp_x < 0 || temp_x >= N || temp_y < 0 || temp_y >= N) {

			// 직전 위치에서 오른쪽으로 회전하여 탐색
			solv(
				temp_x - dx[direction],
				temp_y - dy[direction],
				dest_idx,
				(direction + 1) % 4,
				rotate_cnt + 1
			);

			break;
		}


		// 현재 찾아야 하는 보석에 도착한 경우
		if (temp_x == dest_x && temp_y == dest_y) {

			// 보석을 먹고 기존 방향 그대로 진행
			solv(
				temp_x,
				temp_y,
				dest_idx + 1,
				direction,
				rotate_cnt
			);

			// 보석을 먹은 위치에서 오른쪽으로 회전하여 진행
			solv(
				temp_x,
				temp_y,
				dest_idx + 1,
				(direction + 1) % 4,
				rotate_cnt + 1
			);

			break;
		}


		// 현재 방향에서 오른쪽으로 회전했을 때의 방향
		int new_direction = (direction + 1) % 4;


		/*
		 * 현재 위치가 다음 보석과 같은 행에 도달한 경우
		 * 오른쪽으로 회전했을 때 보석 방향으로 갈 수 있는지 확인
		 */
		if (x != dest_x && temp_x == dest_x) {

			if ((dest_y - temp_y) / dy[new_direction] > 0) {

				solv(
					temp_x,
					temp_y,
					dest_idx,
					(direction + 1) % 4,
					rotate_cnt + 1
				);
			}
		}


		/*
		 * 현재 위치가 다음 보석과 같은 열에 도달한 경우
		 * 오른쪽으로 회전했을 때 보석 방향으로 갈 수 있는지 확인
		 */
		if (y != dest_y && temp_y == dest_y) {

			if ((dest_x - temp_x) / dx[new_direction] > 0) {

				solv(
					temp_x,
					temp_y,
					dest_idx,
					(direction + 1) % 4,
					rotate_cnt + 1
				);
			}
		}


		// 현재 방향으로 한 칸 더 직진
		temp_x += dx[direction];
		temp_y += dy[direction];
	}
}


int main()
{
	int T;
	cin >> T;

	for (int test_case = 1; test_case <= T; test_case++)
	{
		cin >> N;

		// 최소 회전 횟수
		answer = INT_MAX;

		// 가장 큰 보석 번호
		M = INT_MIN;

		board.resize(N, vector<int>(N));
		dest.resize(11);


		/*
		 * 맵 입력
		 * 보석이 존재하는 경우 번호별 위치를 dest에 저장
		 */
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {

				cin >> board[i][j];

				if (board[i][j] != 0) {

					dest[board[i][j]] = make_pair(i, j);

					// 마지막으로 먹어야 할 보석 번호 갱신
					M = max(M, board[i][j]);
				}
			}
		}


		/*
		 * (0, 0)에서 시작
		 * 첫 번째 보석부터 탐색
		 * 초기 방향은 오른쪽(direction = 0)
		 * 초기 회전 횟수는 0
		 */
		solv(0, 0, 1, 0, 0);


		cout << "#" << test_case << " " << answer << "\n";


		// 다음 테스트 케이스를 위해 초기화
		board.clear();
		dest.clear();
	}

	return 0;
}