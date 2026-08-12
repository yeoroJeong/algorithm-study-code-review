// 4013. [모의 SW 역량테스트] 특이한 자석
// https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWIeV9sKkcoDFAVH&
#include <iostream>
#include <vector>
using namespace std;
#define LEFT 6
#define RIGHT 2
int rotationCnt = 0; // K
int map[4][8]; // 톱니바퀴, column 0 for scoring, column 2 or 6 for rotation
vector<pair<int, int>> rotInfo; // wheel index, rotation (-1 counter clockwise / 1 clockwise)

int rotationVal[4];
bool visited[4];
/*
// 이렇게 하니깐 일부 테케 안 되네 ㅠ 뭔가 rotationVal 업데이트가 회전 전에 일어나는 부분이 있는 건지...
void simulate(int wheel, int dir)
{
	visited[wheel] = true;
	if (wheel < 3)
	{
		// compare rightside
		// how to get correct index of left, right touch side
		// if you rotate in clockwise direction(1), index to compare rightside will be (2-1=1)
		// if you rotate in counter clockwise direction (-1), index to compare rightside will be (2-(-1)=3)
		// if you rotate in clockwise direction(1), index to compare leftside will be (6-1=5)
		// if you rotate in counter clockwise direction(-1), index to compare leftside  will be (6-(-1)=7)
		int rIdx = RIGHT - rotationVal[wheel];
		int lIdx2 = LEFT - rotationVal[wheel + 1];
		if (map[wheel][rIdx] != map[wheel + 1][lIdx2])
		{
			if (!visited[wheel + 1])
			{
				simulate(wheel + 1, -dir);
			}
		}
	}
	if (wheel > 0)
	{
		// compare leftside
		int lIdx = LEFT - rotationVal[wheel];
		int rIdx2 = RIGHT - rotationVal[wheel - 1];
		if (map[wheel][lIdx] != map[wheel - 1][rIdx2])
		{
			if (!visited[wheel - 1])
			{
				simulate(wheel - 1, -dir);
			}
		}
	}
	rotationVal[wheel] += dir;
	visited[wheel] = false;
}
*/

// if you rotate in clockwise direction(1), index to compare rightside will be (2-1=1)
// if you rotate in counter clockwise direction (-1), index to compare rightside will be (2-(-1)=3)
// if you rotate in clockwise direction(1), index to compare leftside will be (6-1=5)
// if you rotate in counter clockwise direction(-1), index to compare leftside  will be (6-(-1)=7)
int getRight(int wheelIdx)
{
	int idx = (RIGHT - rotationVal[wheelIdx] % 8) % 8;
	if (idx < 0) idx += 8; // idx -1이면 8 더할 경우 7이라는 올바른 인덱스가 됨
	return idx;
}
int getLeft(int wheelIdx)
{
	int idx = (LEFT - rotationVal[wheelIdx] % 8) % 8;
	if (idx < 0) idx += 8;
	return idx;
}

void simulate(int wheel, int dir)
{
	visited[wheel] = true;
	// compare rightside
	if (wheel < 3 && !visited[wheel + 1])
	{
		int rIdx = getRight(wheel);
		int lIdx = getLeft(wheel + 1);
		if (map[wheel][rIdx] != map[wheel + 1][lIdx])
		{
			simulate(wheel + 1, -dir);
		}
	}
	// compare leftside
	if (wheel > 0 && !visited[wheel - 1])
	{
		int lIdx = getLeft(wheel);
		int rIdx = getRight(wheel - 1);
		if (map[wheel][lIdx] != map[wheel - 1][rIdx])
		{
			simulate(wheel - 1, -dir);
		}
	}
	// change rotaion value after the deepest search is done
	rotationVal[wheel] += dir;
	visited[wheel] = false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;

	for (int tc = 0; tc < T; tc++)
	{
		cin >> rotationCnt;
		rotInfo.resize(rotationCnt, { 0,0 });
		fill(rotationVal, rotationVal + 4, 0);
		fill(visited, visited + 4, false);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				cin >> map[i][j];
			}
		}
		for (int i = 0; i < rotationCnt; i++)
		{
			int target, dir;
			cin >> target >> dir;
			// Adjust wheel number as index
			rotInfo[i] = { target - 1, dir };
		}

		// do simulation
		for (int i = 0; i < rotationCnt; i++)
		{
			int wheel = rotInfo[i].first;
			int dir = rotInfo[i].second;
			simulate(wheel, dir);
		}

		// print result
		int result = 0;
		for (int i = 0; i < 4; i++)
		{
			int index = (8 - rotationVal[i] % 8) % 8;
			// 1 2 4 8 
			//result += map[i][index] * pow(2,i);

			if (map[i][index] == 1)
				result += map[i][index] * (1 << i);
		}
		cout << "#" << tc + 1 << " " << result << "\n";
	}
	return 0;
}