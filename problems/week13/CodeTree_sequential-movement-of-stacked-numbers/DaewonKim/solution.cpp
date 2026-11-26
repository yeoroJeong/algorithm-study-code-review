#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <deque>
#include <set>

using namespace std;

int n, m;
int move_nums[100];

int dx[8] = {1, -1, 0, 0, 1, 1, -1, -1};
int dy[8] = {0, 0, -1, 1, 1, -1, 1, -1};

struct Section
{
	int curMaxNum = -1;		// dq안의 원소들을 포함해서 가장 큰 값.
	deque<int> dq;			// 현재 칸 안에 있는 원소들
	set<int> s;
};

struct Number
{
	// 숫자의 위치
	int x = -1;
	int y = -1;
};

map<int, Number> nums;

bool isInRange(const int& x, const int& y)
{
	return (x >= 0 && x < n && y >= 0 && y < n);
}

void setMaxNumOnSections(vector<vector<Section>>& board)
{
	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			// 비어있으면 none
			if (board[row][col].s.empty())
			{
				board[row][col].curMaxNum = -1;
			}
			else
			{
				// 정렬된 마지막 값이 제일 큰 값.
				board[row][col].curMaxNum = *board[row][col].s.rbegin();
			}
		}
	}
}

// 이동 시킬 번호, 인접한 칸에 가장 큰 숫자, 판
void move(int&moveNum, int& maxNum, vector<vector<Section>>& board)
{
	// 이동할 숫자의 좌표
	int startX = nums[moveNum].x;
	int startY = nums[moveNum].y;

	// 이동 시킬 칸의 좌표
	int endX = nums[maxNum].x;
	int endY = nums[maxNum].y;

	// 별칭 지정.
	deque<int>& dq = board[startX][startY].dq;
	deque<int>& targetdq = board[endX][endY].dq;
	deque<int> tmpdq;
	while (!dq.empty())
	{
		int num = dq.front();
		dq.pop_front();
		board[startX][startY].s.erase(num);
		tmpdq.push_back(num);

		if (num == moveNum)
			break;
	}

	while(!tmpdq.empty())
	{
		int num = tmpdq.back();
		tmpdq.pop_back();
		// target에 넣기. nums값 변경
		targetdq.push_front(num);
		nums[num].x = endX;
		nums[num].y = endY;

		board[endX][endY].s.insert(num);
	}


}

// 숫자가 위로 이동한다.
// 자기 위에 있는 숫자와 자기 자신까지 이동함.
// 가장 큰 정수가 있는 곳으로 이동하는데, deque안의 값까지 포함해서 가장 큰 정수를 찾아야 한다.
void search(int& moveNum, vector<vector<Section>>& board)
{
	// 자신 기준 인접 8방향 탐색. 
	int tmpMaxNum = -1;
	for (int dir = 0; dir < 8; ++dir)
	{
		int nextRow = nums[moveNum].x + dx[dir];
		int nextCol = nums[moveNum].y + dy[dir];

		// 범위 내에 있다면. 가장 큰 값의 숫자를 찾는다. 
		if (isInRange(nextRow, nextCol))
		{
			// 큰 값을 찾았다면 갱신.
			if (tmpMaxNum < board[nextRow][nextCol].curMaxNum)
				tmpMaxNum = board[nextRow][nextCol].curMaxNum;
		}

	}

	// 큰 값의 위치가 있다면, 그것을 기준으로 자기 위의 숫자들과 자신을 옮겨준다.
	if(tmpMaxNum != -1) move(moveNum, tmpMaxNum, board);
}

int main() {
	cin >> n >> m;

	Section sec;
	sec.dq.clear();
	sec.s.clear();

	vector<vector<Section>> board(n, vector<Section>(n, sec));

	// 초기화 하는 부분
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int tmp = 0;
			cin >> tmp;
			nums[tmp].x = i;
			nums[tmp].y = j;

			board[i][j].curMaxNum = tmp;
			board[i][j].dq.push_back(tmp);
			board[i][j].s.insert(tmp);
		}
	}


	for (int i = 0; i < m; i++) {
		cin >> move_nums[i];
	}

	// m번 만큼 움직임.
	for (int i = 0; i < m; ++i)
	{
		// 이 안에 search + move까지 있음.
		search(move_nums[i], board); 

		// 변경된 최댓값 설정하기
		setMaxNumOnSections(board);
	}


	// 최종 board 출력.
	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			if (board[row][col].curMaxNum == -1)
				cout << "None";
			else 
			{
				while (!board[row][col].dq.empty())
				{
					int tmpNum = board[row][col].dq.front();
					board[row][col].dq.pop_front();
					cout << tmpNum << " ";
				}
			}
			cout << "\n";
		}
	}
	return 0;
}
