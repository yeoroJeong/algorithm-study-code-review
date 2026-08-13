#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int N, W, H;
vector<vector<int>> board;
int mx;
int sum;

void relocate() { // board 재배치
	for (int col = 0; col < W; col++) {
		vector<int> temp;
		for (int row = 0; row < H; row++) {
			if (board[row][col] > 0) {
				temp.push_back(board[row][col]);
			}
		}
		for (int row = 0; row < H; row++) {
			board[row][col] = 0;
		}
		int idx = H - 1;
		for (int k = temp.size() - 1; k >= 0; k--) {
			board[idx--][col] = temp[k];
		}
	}
}

void breaks(int h, int w) { // 블록 깨는 함수(재귀)
	int c = board[h][w];
	board[h][w] = 0;
	sum++;
	if (c > 1) { // 지금 깬 블록의 num
		for (int col = w - (c - 1); col <= w + (c - 1); col++) {
			if (col < 0 || col >= W) {
				continue;
			}
			if (board[h][col] >= 1) {
				breaks(h, col);
			}
		}
		for (int row = h - (c - 1); row <= h + (c - 1); row++) {
			if (row < 0 || row >= H) {
				continue;
			}
			if (board[row][w] >= 1) {
				breaks(row, w);
			}
		}
	}
}


void back(int cnt) { // backtraking 메인함수
	mx = max(sum, mx);
	if (cnt == N) {
		return;
	}

	for (int i = 0; i < W; i++) { // 구슬은 가로방향으로만 이동 가능
		int row = -1;
		for (int r = 0; r < H; r++) { // 부술 칸 찾음
			if (board[r][i] > 0) {
				row = r;
				break;
			}
		}
		if (row == -1) {
			continue;
		} // 빈 열이면 스킵

		vector<vector<int>> temp1 = board;
		int temp2 = sum;
		breaks(row, i); // 블록깨고
		relocate(); // 보드 재배치
		back(cnt + 1);
		board = temp1;
		sum = temp2;
	}
}


int main() {
	int T;
	cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N >> W >> H;
		board.assign(H, vector<int>(W, 0));
		int cnt = 0; // 초기 벽돌 개수
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				cin >> board[i][j];
				if (board[i][j] > 0) {
					cnt++;
				}
			}
		}
		// 입력 끝
		sum = 0;
		mx = 0;
		back(0);

		cout << "#" << t << " " << cnt - mx << "\n";
	}
}