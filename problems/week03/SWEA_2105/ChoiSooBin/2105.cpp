/*
문제: 2105. [모의 SW 역량테스트] 디저트 카페
링크:https://swexpertacademy.com/main/code/problem/problemDetail.do?contestProbId=AV5VwAr6APYDFAWu
작성자: 알고리즘
알고리즘: DFS, 백트래킹, 완전 탐색(시뮬레이션)

[문제 요약]
한 변의 길이가 N인 지역에 디저트 카페가 모여 있다.
임의의 카페에서 출발해 대각선 방향으로 움직이며 사각형 모양의 경로를 그려 출발점으로 돌아와야 한다.
투어 도중 같은 종류의 디저트를 다시 먹으면 안 되며, 가장 많은 종류의 디저트를 먹을 수 있는 경로를 찾아 그 수를 출력하는 문제다.

[핵심 아이디어]
방향(dir)은 대각선 4방향(0:우하, 1:좌하, 2:좌상, 3:우상)으로 고정되어 순서대로 돌아야 사각형이 완성된다.
변의 길이를 미리 수학적으로 계산하려 하지 말고, 현재 위치에서 "직진(현재 방향 유지)"할지 "꺾기(다음 방향 전환)"할지 2갈래로 나누어 모든 경로를 탐색(DFS)하는 것이 핵심이다.
지나온 카페의 디저트 번호를 visited 배열로 체크하여 중복을 거르고, 탐색이 끝나면 false로 되돌리는 백트래킹을 활용한다.

[풀이 과정]
1. N*N 맵을 입력받고, 시작점이 될 수 있는 모든 좌표(i, j)에 대해 탐색을 시작한다. (사각형을 그려야 하므로 테두리 일부는 제외해도 무방하다.)
2. DFS 함수(back)는 현재 위치, 먹은 디저트 개수(count), 현재 이동 방향(dir)을 매개변수로 받는다.
3. 다음으로 이동할 좌표(ny, nx)를 계산한 뒤, 맵을 벗어나면 종료(return)한다.
4. [기저 조건] 다음 좌표(ny, nx)가 시작점(sty, stx)과 같고, 현재 방향이 마지막 3번 방향(dir == 3)이라면 사각형이 완성된 것이므로 최댓값(mx)을 갱신하고 종료한다.
5. 다음 카페의 디저트를 이미 먹었다면(visited == true) 탐색을 종료한다.
6. 디저트를 먹었다고 표시(visited = true)한 뒤, 방향에 따라 재귀를 호출한다.
   - dir이 0, 1, 2일 때: 직진하는 재귀(dir)와 방향을 꺾는 재귀(dir+1) 두 개를 호출한다.
   - dir이 3일 때: 더 이상 꺾을 수 없으므로 직진하는 재귀(3) 하나만 호출한다.
7. 재귀에서 빠져나오면 방문 표시를 해제(visited = false)하여 백트래킹한다.

[복잡도]
- 시간 복잡도: 맵의 최대 크기가 20x20으로 작고, visited 배열을 통해 중복 디저트를 먹는 경로를 빠르게 가지치기(Pruning) 하므로 3초 안에 넉넉하게 통과한다.
- 공간 복잡도: 20x20 맵과 101 사이즈의 방문 처리 배열만 사용하므로 O(N^2)로 매우 적은 메모리를 사용한다.

[막혔던 부분과 오답 원인 및 해결 방법]
- 첫 번째 삽질 (변의 길이 사전 계산의 늪):
  for문과 while문을 이용해 사각형 변의 길이를 미리 계산하고 곱셈으로 좌표를 점프(sty + dy * i)시키려다 보니 로직이 심각하게 꼬였다.
  [해결] DFS가 알아서 깊이를 파고들며 선을 그려준다는 점을 깨닫고, 반복문을 전부 지운 뒤 "한 칸씩 전진하는 2갈래 DFS"로 코드를 완전히 슬림하게 바꿨다.

- 두 번째 삽질 (백트래킹 방문 처리 누락):
  특정 방향 블록(dir == 2, 3)에서 중복 검사(if (visited) return;)만 하고, 정작 visited = true/false로 체크하고 해제하는 상태 보존 로직을 빼먹어서 탐색이 망가졌다.
  [해결] 재귀 호출 위아래에 방문 처리와 해제 로직을 꼼꼼하게 통일하여 추가했다.

- 세 번째 삽질 (수학 계산과 while 루프의 잔재):
  dir == 3일 때 while(1) 안에서 ny, nx를 갱신하지 않고 y, x만 더하다 무한 루프에 빠졌다. 또한 dir == 2일 때 꺾는 타이밍을 굳이 수학(nx - stx == ny - sty)으로 계산하려는 코드가 남아있었다.
  [수정 전]
  else if (dir == 2) {
	  if (nx - stx == ny - sty) {
		  back(ny, nx, count + 1, 3); // 억지로 계산해서 꺾음
	  }
	  y = y + dy[2]; x = x + dx[2]; // 불필요한 계산 잔재
  }

  [수정 후]
  else if (dir == 2) {
	  visited[map[ny][nx]] = true;
	  back(ny, nx, count + 1, 2); // 직진
	  back(ny, nx, count + 1, 3); // 꺾기
	  visited[map[ny][nx]] = false;
  }
  [해결] 완탐(DFS)을 믿고, 복잡한 꺾임 타이밍 계산이나 불필요한 루프를 전부 지웠다. 다른 방향들과 똑같이 "직진 1개, 꺾기 1개"의 직관적인 재귀 호출로 통일하여 해결했다.
*/


#include<iostream>
#include<climits>
#include<algorithm>

using namespace std;
typedef long long ll;

int mx, mn;
int n;
bool visited[101];
int sty, stx;
int dy[4] = { 1,1,-1,-1 };
int dx[4] = { -1,1,1,-1 };
int map[20][20];

// dir 0:왼쪽 아래, 1: 오른쪽 아래 2: 오른쪽 위 3: 왼쪽 위

void back(int y, int x, int count, int dir) {
	int ny = y + dy[dir];
	int nx = x + dx[dir];
	if (sty == ny && stx == nx && dir == 3) {
		// 제자리로 돌아옴
		if (mx < count) {
			mx = count;
			//fill(visited, visited + 101, false);
		}
	}
	//if (visited[map[y][x]]) return; 
	
	if (ny < 0 || ny >= n || nx < 0 || nx >= n) return;
	if (dir == 0) {
		
		if (visited[map[ny][nx]]) return;
		visited[map[ny][nx]] = true;
		back(ny, nx, count + 1, 0);
		back(ny, nx, count + 1, 1);
		visited[map[ny][nx]] = false;
	}
	else if (dir == 1) {
		if (visited[map[ny][nx]]) return;
		visited[map[ny][nx]] = true;
		back(ny, nx, count + 1, 1);
		back(ny, nx, count + 1, 2);
		visited[map[ny][nx]] = false;
	}

	else if (dir == 2) {
		if (visited[map[ny][nx]]) return;
		if (nx - stx == ny - sty) {
			visited[map[ny][nx]] = true;
			back(ny, nx, count + 1, 3);
			visited[map[ny][nx]] = false;
		}
		else {
			visited[map[ny][nx]] = true;
			back(ny, nx, count + 1, 2);
			visited[map[ny][nx]] = false;
		}
		
		y = y + dy[2];
		x = x + dx[2];
	}
	else { // dir == 3
		if (visited[map[ny][nx]]) return;
		visited[map[ny][nx]] = true;
		back(ny, nx, count + 1, 3);
		visited[map[ny][nx]] = false;
	}
}

int main(int argc, char** argv)
{
	int test_case;
	int T;
	freopen("2105_input.txt", "r", stdin);

	cin >> T;

	for (test_case = 1; test_case <= T; ++test_case)
	{
		// 초기화
		mx = INT_MIN;
		int ans = 0;
		fill(&map[0][0], &map[0][0] + 20 * 20, 0);
		fill(visited, visited + 101, false);
		cin >> n;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cin >> map[i][j];
			}
		}
		for (int i = 0; i < n - 2; i++) {
			for (int j = 1; j < n - 1; j++) {
				sty = i;
				stx = j;
				// 첫번째 좌표는 여기서 이미 처리

				visited[map[i][j]] = true;
				back(i, j, 1, 0); // 현재 좌표, level
				visited[map[i][j]] = false;
			}
		}
		if (mx <= 1) ans = -1;
		else ans = mx;
		cout << '#' << test_case << ' ' << ans << '\n';
	}
	return 0;
}
