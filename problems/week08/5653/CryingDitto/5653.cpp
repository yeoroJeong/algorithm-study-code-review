#include <iostream>
using namespace std;

//5653. [모의 SW 역량테스트] 줄기세포배양
//https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AWXRJ8EKe48DFAUo

// 초기 상태 X인 줄기 세포
	// X시간 동안 비활성 상태
	// X 시간 지나는 순간 활성 상태
// 활성 상태에서 X시간 동안 살아있다가 죽게 됨
// 활성 상태에서 첫 1시간 동안 상, 하, 좌, 우 네 방향으로 동시에 번식
// 두 개 이상의 줄기 세포가 하나의 셀에 동시 번식하려고 할 경우 생명력 수치 높은 줄기 세포가 셀 차지

// T 테스트 케이스
// N, M, K (세로 크기, 가로 크기, 배양 시간)
	// K (1≤K≤300)
// N줄에 걸쳐 M개의 그리드 상태 정보 (1≤N≤50, 1≤M≤50)
	// 각 줄기 세포의 생명력 (1≤X≤10) 

int rowN, colM, timeK;
int half = 500;
int grid[1000][1000];
int born[1000][1000];
int life[1000][1000];
bool visited[1000][1000];
// 상, 하, 좌, 우
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0,-1, 1 };

void activate(int t)
{

	int startI = half - rowN / 2;
	int startJ = half - colM / 2;

	// 시간 지날 때마다 탐색 범위 확대 필요
	for (int i = startI - t; i < startI + rowN + t; i++)
	{
		for (int j = startJ - t; j < startJ + colM + t; j++)
		{
			if (born[i][j] + life[i][j] == t)
			{
				// activate
				for (int k = 0; k < 4; k++)
				{
					int nx = i + dx[k];
					int ny = j + dy[k];
					if (nx < 0 || ny < 0 || nx >= 1000 || ny >= 1000)
					{
						continue;
					}
					// 이미 세포 있음
					if (visited[nx][ny])
					{
						// 다음 번에 태어나는 세포라면 아직 업데이트 가능한 세포임
						if (born[nx][ny] == t + 1 && life[nx][ny] < life[i][j])
						{
							// 생명령 수치 높은 세포로 업데이트
							life[nx][ny] = life[i][j];
							grid[nx][ny] = t + 1 + life[i][j] * 2;
						}
						continue;
					}


					visited[nx][ny] = true;
					// 다음 번에 태어남
					born[nx][ny] = t + 1;
					life[nx][ny] = life[i][j];
					grid[nx][ny] = t + 1 + life[i][j] * 2;
				}
			}
		}
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);

	int T;
	cin >> T;
	for (int tc = 0; tc < T; tc++)
	{
		cin >> rowN >> colM >> timeK;

		for (int i = 0; i < 1000; i++)
		{

			fill(born[i], born[i] + 1000, 0);
			fill(grid[i], grid[i] + 1000, 0);
			fill(life[i], life[i] + 1000, 0);
			fill(visited[i], visited[i] + 1000, 0);
		}
		int startI = half - rowN / 2;
		int startJ = half - colM / 2;
		for (int i = startI; i < startI + rowN; i++)
		{
			for (int j = startJ; j < startJ + colM; j++)
			{
				cin >> life[i][j];
				if (life[i][j] > 0)
				{
					born[i][j] = 0;
					visited[i][j] = true;
					grid[i][j] = life[i][j] * 2;
				}
			}
		}

		for (int time = 1; time < timeK; time++)
		{
			activate(time);
		}

		int cnt = 0;
		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				if (grid[i][j] > timeK)
				{
					cnt++;
				}
			}
		}
		cout << "#" << tc + 1 << " " << cnt << "\n";
	}

	return 0;
}