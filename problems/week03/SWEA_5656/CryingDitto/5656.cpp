// 5656. [모의 SW 역량테스트] 벽돌 깨기
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWXRQm6qfL0DFAUo&
// 폭발 로직 작성까지는 쉬웠는데 구슬 떨어뜨리는 위치 완전 탐색하는 부분 구현하는 게 초힘들었음ㅠㅠ

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Marble {
	int x;
	int y;
	int boomSize;
};

struct Record
{
	vector<vector<int>> myMap;
	vector<int> selectedColIdx;
	int remainBoxCnt;
};

vector<vector<int>> map;
int row, col;
int marbleCnt = 0; // 구슬 개수
int firstNonzeroRowIdx = 0;
int minBoxCnt = INT_MAX;

int dx[4] = { -1,1,0,0 };
int dy[4] = { 0,0, -1,1 };

Record finalResult;

// returns copied map after bomb
vector<vector<int>> cleanAfterBomb(vector<vector<int>> mapToCopy)
{
	vector<vector<int>> copy = mapToCopy;
	for (int i = row - 1; i >= 0; i--)
	{
		for (int j = 0; j < col; j++)
		{
			int upperRow = i - 1;
			while (copy[i][j] == 0 && upperRow >= 0)
			{
				if (copy[upperRow][j] > 0)
				{
					copy[i][j] = copy[upperRow][j];
					copy[upperRow][j] = 0;
					upperRow = -1;
				}
				// search upper rows until non-zero upper row value exists
				upperRow--;
			}
		}
	}
	return copy;
}

// simply count remaining boxes
// const 자료형& 변수명
// 읽기 전용 (함수 내에서 값 수정 안 할 거면 읽기 전용 하는 게 빠르대서...)
// 그냥 값 복사 해도 시간 내에 통과함
int countBoxes(const vector<vector<int>>& map)
{
	int boxCount = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] > 0)
			{
				boxCount++;
			}
		}
	}
	return boxCount;
}

vector<vector<int>> bomb(int x, int y, vector<vector<int>> mapCopy)
{
	queue<Marble> q;
	q.push({ x, y, mapCopy[x][y] });
	while (!q.empty())
	{
		Marble m = q.front();
		q.pop();
		mapCopy[m.x][m.y] = 0; // self bomb

		for (int i = 0; i < 4; i++)
		{
			for (int boom = 1; boom < m.boomSize; boom++)
			{
				int nx = m.x + boom * dx[i];
				int ny = m.y + boom * dy[i];
				// prevent out of range
				if (nx < 0 || ny < 0 || nx >= mapCopy.size() || ny >= mapCopy[0].size()) continue;
				if (mapCopy[nx][ny] > 1)
				{
					q.push({ nx,ny,mapCopy[nx][ny] });
				}
				// clean used area
				mapCopy[nx][ny] = 0;
			}
		}
	}
	mapCopy = cleanAfterBomb(mapCopy);
	return mapCopy;
}

// search all possible results dropping marbles
void findDropPlace(Record record, int marbleUsed)
{
	if (marbleUsed >= marbleCnt)
	{
		// ============ FOR DEBUG ==============
		/*if (minBoxCnt > record.remainBoxCnt)
		{
			for (int i : record.selectedColIdx)
			{
				cout << i << " ";
			}
			cout << "(min box count: " << record.remainBoxCnt << ")\n";
		}*/
		// ============ FOR DEBUG ==============
		minBoxCnt = minBoxCnt > record.remainBoxCnt ? record.remainBoxCnt : minBoxCnt;
		return;
	}
	if (record.remainBoxCnt == 0)
	{
		// 박스가 남지 않아서 더 탐색할 필요 없이 조기 종료
		minBoxCnt = 0;
		return;
	}

	for (int col = 0; col < record.myMap[0].size(); col++)
	{
		// search non-zero value from the top
		int row = 0;
		while (row < record.myMap.size() && record.myMap[row][col] == 0)
		{
			row++;
		}
		// non-zero value found!
		if (row < record.myMap.size())
		{
			vector<vector<int>> mapAfterBomb = bomb(row, col, record.myMap);
			vector<int> newSelectedColIdx = record.selectedColIdx;
			newSelectedColIdx.push_back(col);
			// create new Record to prevent contaminating current record
			Record nextRecord = { mapAfterBomb, newSelectedColIdx, countBoxes(mapAfterBomb) };
			findDropPlace(nextRecord, marbleUsed + 1);
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;
	for (int tc = 0; tc < T; tc++)
	{
		cin >> marbleCnt >> col >> row;

		minBoxCnt = INT_MAX;
		map.resize(row);
		int firstCheck = false;

		for (int i = 0; i < row; i++)
		{
			map[i].resize(col);
			for (int j = 0; j < col; j++)
			{
				cin >> map[i][j];
				if (!firstCheck && map[i][j] > 0)
				{
					firstCheck = true;
					firstNonzeroRowIdx = i;
				}
			}
		}
		// 사실 선택한 열은 기록할 필요 필요 없는데... 디버깅용으로 추적해봤음 ;ㅅ;
		vector<int> selectedCols;
		findDropPlace({ map, selectedCols, countBoxes(map) }, 0);
		cout << "#" << tc + 1 << " " << minBoxCnt << "\n";
	}
	return 0;
}