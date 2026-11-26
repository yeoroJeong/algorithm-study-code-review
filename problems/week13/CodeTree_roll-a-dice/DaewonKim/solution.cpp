#include <iostream>
#include <vector>
#include <set>


// 주사위의 이동 방향

// 상 하 좌 우
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

using namespace std;

int n, m;
int r, c;
char directions[10000];

// 상 하 좌 우 순
vector<int> curDiceMap(4);

// 주사위가 있었던 자리 기록용 (위치가 중복될 수도 있기 때문에 unordered_set 사용)
set<pair<int, int>> sumBoardPos;

bool isInRange(const int& row, const int& col)
{
	return(row >= 0 && row < n && col >= 0 && col < n);
}

// 해당 위치에 숫자가 적혀 있었더라도, 새로 숫자가 적히게 됩니다.
void rollDice(vector<vector<int>>& board, int dir, int& curRow, int& curCol, int& diceEye)
{
	int nextRow = curRow + dx[dir];
	int nextCol = curCol + dy[dir];

	// 범위 밖을 벗어난다면 시행 X
	if (!isInRange(nextRow, nextCol)) return;

	// set이라서 또 넣어도 중복을 제거하기 때문에 상관 없음
	sumBoardPos.insert({ nextRow,nextCol });
	board[nextRow][nextCol] = curDiceMap[dir];

	// 위치 갱신
	curRow = nextRow;
	curCol = nextCol;

	// 현재 주사위 눈을 저장
	int prevDiceEye = diceEye;

	// 갱신
	diceEye = curDiceMap[dir];

	// 주사위 전개도 갱신
	// 상 하로 이동했으면 -> 좌 우의 전개도는 이전과 동일하다, 좌 우는 변경 X
	// 좌 우로 이동했으면 -> 상 하의 전개도는 이전과 동일하다, 상 하는 변경 X
	switch (dir)
	{
		case 0: // 상
		{
			curDiceMap[0] = 7-prevDiceEye;
			curDiceMap[1] = prevDiceEye;
			break;
		}
		case 1: // 하
		{
			curDiceMap[0] = prevDiceEye;
			curDiceMap[1] = 7- prevDiceEye;
			break;
		}
		case 2: // 좌
		{
			/*좌*/curDiceMap[2] = 7- prevDiceEye;
			/*우*/curDiceMap[3] = prevDiceEye;
			break;
		}
		case 3: // 우
		{
			curDiceMap[2] = prevDiceEye;
			curDiceMap[3] = 7- prevDiceEye;
			break;
		}
	}


}

int main() {
	cin >> n >> m >> r >> c;

	vector<vector<int>> board(n, vector<int>(n, 0));

	//  방향 입력
	for (int i = 0; i < m; i++) {
		char dir;
		cin >> dir;
		switch (dir)
		{
		case 'U':
		{
			directions[i] = 0;
			break;
		}
		case 'D':
		{
			directions[i] = 1;
			break;
		}
		case 'L':
		{
			directions[i] = 2;
			break;
		}
		case 'R':
		{
			directions[i] = 3;
			break;
		}
		}
	}

	r -= 1;
	c -= 1;
	int diceEye = 6; // 밑바닥의 주사위 눈, 초기에는 6으로 주어짐
	
	// 초기 주사위 전개도 설정
	curDiceMap[0] = 5;
	curDiceMap[1] = 2;
	curDiceMap[2] = 4;
	curDiceMap[3] = 3;

	// set이라서 또 넣어도 중복을 제거하기 때문에 상관 없음
	sumBoardPos.insert({ r,c });
	board[r][c] = diceEye; // 굴리기 이전 현재 좌표에 대입
	for (int i = 0; i < m; ++i)
	{
		rollDice(board, directions[i], r, c, diceEye);
	}

	int total = 0;
	for (auto &p : sumBoardPos)
	{
		total += board[p.first][p.second];
	}
	cout << total;
	return 0;
}
